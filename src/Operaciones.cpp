/**
 * @file Operaciones.cpp
 * @brief .
 */

// Este valor se puede cambiar cada vez que se utilice el programa
#define TASA_DE_CAMBIO 525.95

#include "Operaciones.hpp"


using namespace std;

enum OpcionesOperaciones{ // Define la enumeracion de las opciones del menu
    DOLARES = 1,
    COLONES
};

static int callback(void *data, int argc, char**argv, char **azColName){
    for(int i = 0; i < argc; i++){
        cout << azColName[i] << (argv[i] ? argv[i] : "NULL") << endl;
    }
    return 0;
}

int Operaciones::deposito() {
    
    //variables para realizar el deposito
    string denominacion;
    string cedula;
    string clienteOrigenCedula;
    string clienteDestinoCedula;
    float montoBase;


    //variables que no se que son
    unsigned int numero_cuenta;
    double montoDepositar;
    int cuenta_op;

    //variables sql
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg = 0;
    int rc;
    const char* data = "Callback function called";

    // String de consulta que va a variar dependiendo de la denominacion de la cuenta
    const char *clientes;

    // Conexion a la base de datos y mensaje de repuesta
    rc = sqlite3_open("banco.db", &db);
    if(rc){
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return 0;
    } else {
        cout << "Ingreso correcto a la base de datos" << endl;
    }

    // Inicio de la transacción
    rc = sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error al iniciar la transacción: " << errMsg << endl;
        sqlite3_close(db);
        return 0;
    }

    // Solicitar datos para realizar el deposito
    cout << "Por favor ingrese el número de cedula del cliente al que se va a depositar" << endl;
    getline(cin, cedula);

    do {
        cout << "¿Desea realizar el depósito en la cuenta de dólares o colones?" << endl;
        cout << "1) Dólares" << std::endl;
        cout << "2) Colones" << std::endl;

        // Verificar si la entrada del usuario es válida
        if (!(cin >> cuenta_op)) {
            cout << "\n";
            cout << "Opcion no valida. Por favor, ingrese un numero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar el resto de la línea
            continue;
        }

        switch (cuenta_op)
        {
        case DOLARES:
            denominacion = "dolares";

            // Primera consulta
            clientes = "SELECT cuenta_dolares from clientes WHERE cedula = ?";
            break;
        case COLONES:
            denominacion = "colones";
            
            // Primera consulta
            clientes = "SELECT cuenta_colones from clientes WHERE cedula = ?";
            break;
        default:
            cout << "¡La opción ingresada no es válida! Por favor, inténtelo de nuevo.";
            break;
        }
    } while (cuenta_op != DOLARES && cuenta_op != COLONES);

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, clientes, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }

    // Parametro para la consulta
    sqlite3_bind_text(stmt, 1, cedula.c_str(), -1, SQLITE_STATIC);

    // Realizar primera consulta para obtener el numero de cuenta (puede ser dolares o colones)
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        numero_cuenta = sqlite3_column_int(stmt, 0);
        cout << numero_cuenta << endl;  // CHECKPOINT
    } else if (rc == SQLITE_DONE) {
        cout << "No existe ninguna cuenta asociada a este número de cédula" << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    } else {
        cerr << "Error de SQL." << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    // Crear la segunda consulta para actualizar balance de la cuenta
    const char *cuentas = "UPDATE cuentas "
                          "SET balance = balance + ? "
                          "WHERE numero_cuenta = ?";

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, cuentas, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
    }

    // Preguntar el monto que desea depositar
    cout << "Por favor, ingrese el monto que desea depositar: " << endl;
    cin >> montoDepositar;
    cin.ignore();

    // Verifiacion monto
    if (montoDepositar <= 0) {
        cout << "¡El monto a depositar no es válido!";
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Parametro para la consulta
    sqlite3_bind_double(stmt, 1, montoDepositar);
    sqlite3_bind_double(stmt, 2, numero_cuenta);

    // Realizar la segunda consulta
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        cout << "¡El depósito fue realizado de manera exitosa!" << endl;
    } else {
        cerr << "Error de SQL." << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Guardar los datos para el registro
    clienteOrigenCedula = cedula;
    clienteDestinoCedula = cedula;
    montoBase = montoDepositar;

    // Confirmar la transacción
    rc = sqlite3_exec(db, "COMMIT", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error al confirmar la transacción: " << errMsg << endl;
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, &errMsg);
        sqlite3_close(db);
        return 0;
    }

    // Liberar memoria y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 1;
}; 

int Operaciones::retiro(std::string &denominacion, std::string &clienteOrigenCedula, std::string &clienteDestinoCedula, float &montoBase) {
    string cedula;
    unsigned int numero_cuenta;
    double balance;
    double montoRetirar;
    int cuenta_op;
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg = 0;
    int rc;
    const char* data = "Callback function called";

    // String de consulta que va a variar dependiendo de la denominacion de la cuenta}
    const char *clientes;

    // Conexion a la base de datos y mensaje de repuesta
    rc = sqlite3_open("banco.db", &db);
    if(rc){
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return 0;
    } else {
        cout << "Ingreso correcto a la base de datos" << endl;
    }

    // Inicio de la transacción
    rc = sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error al iniciar la transacción: " << errMsg << endl;
        sqlite3_close(db);
        return 0;
    }

    // Solicitar datos para realizar el retiro
    cout << "Por favor ingrese el número de cedula del cliente que va a realizar el retiro" << endl;
    getline(cin, cedula);

    do {
        cout << "¿Desea realizar el depósito en la cuenta de dólares o colones?" << endl;
        cout << "1) Dólares" << std::endl;
        cout << "2) Colones" << std::endl;

        // Verificar si la entrada del usuario es válida
        if (!(cin >> cuenta_op)) {
            cout << "\n";
            cout << "Opcion no valida. Por favor, ingrese un numero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar el resto de la línea
            continue;
        }

        switch (cuenta_op)
        {
        case DOLARES:
            denominacion = "dolares";

            // Primera consulta
            clientes = "SELECT cuenta_dolares from clientes WHERE cedula = ?";
            break;
        case COLONES:
            denominacion = "colones";
            
            // Primera consulta
            clientes = "SELECT cuenta_colones from clientes WHERE cedula = ?";
            break;
        default:
            cout << "¡La opción ingresada no es válida! Por favor, inténtelo de nuevo.";
            break;
        }
    } while (cuenta_op != DOLARES && cuenta_op != COLONES);

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, clientes, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }

    // Parametro para la consulta
    sqlite3_bind_text(stmt, 1, cedula.c_str(), -1, SQLITE_STATIC);

    // Realizar primera consulta para obtener el numero de cuenta (puede ser dolares o colones)
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        numero_cuenta = sqlite3_column_int(stmt, 0);
        cout << numero_cuenta << endl;  // CHECKPOINT
    } else if (rc == SQLITE_DONE) {
        cout << "No existe ninguna cuenta asociada a este número de cédula" << endl;
        return 0;
    } else {
        cerr << "Error de SQL." << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_finalize(stmt);  // Finalizar consulta

    // Crear una consulta extra para obtener el balance y compararlo con el monto
    const char *cuentasExt = "SELECT balance FROM cuentas WHERE numero_cuenta = ?";

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, cuentasExt, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }

    // Parametros de la consulta
    sqlite3_bind_double(stmt, 1, numero_cuenta);

    // Realizar la consulta
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        balance = sqlite3_column_double(stmt, 0);
        cout << "El valor del balance asociado fue accesado correctamente" << endl;
    } else if (rc == SQLITE_DONE) {
        cout << "No fue posible obtener el valor del balance asociado a esta cuenta" << endl;
        return 0;
    } else {
        cerr << "Error de SQL." << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    } 

    sqlite3_finalize(stmt);  // Finalizar consulta

    // Crear la tercera consulta para actualizar balance de la cuenta
    const char *cuentas = "UPDATE cuentas "
                          "SET balance = balance - ? "
                          "WHERE numero_cuenta = ?";


    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, cuentas, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }

    // Preguntar el monto que desea retirar
    cout << "Por favor, ingrese el monto que desea retirar: " << endl;
    cin >> montoRetirar;
    cin.ignore();

    // Verifiacion monto
    if (montoRetirar <= 0) {
        cout << "¡El monto a retirar no es válido!" << endl;
        return 0;
    } else if (balance < montoRetirar) {
        cout << "La cuenta no presenta el saldo suficiente para retirar este monto" << endl;
        return 0;
    }

    // Parametros de la consulta
    sqlite3_bind_double(stmt, 1, montoRetirar);
    sqlite3_bind_double(stmt, 2, numero_cuenta);

    // Realizar la consulta final
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        cout << "Retiro realizado correctamente." << endl;
    } else if (rc != SQLITE_DONE) {
        // Hubo un error en la consulta
        cerr << "No fue posible realizar el retiro: " << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Confirmar la transacción
    rc = sqlite3_exec(db, "COMMIT", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error al confirmar la transacción: " << errMsg << endl;
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, &errMsg);
        sqlite3_close(db);
        return 0;
    }

    // Guardar los datos para el registro
    clienteOrigenCedula = cedula;
    clienteDestinoCedula = cedula;
    montoBase = montoRetirar;

    // Liberar memoria y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
};

int Operaciones::transferencias(std::string &denominacion, std::string &clienteOrigenCedula, std::string &clienteDestinoCedula, float &montoBase) {
    string cedula;
    unsigned int numero_cuenta;
    unsigned int numeroCuentaReceptor;
    string denominacionReceptor;
    double montoTransferencia;
    double balance;
    int cuenta_op;
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg = 0;
    int rc;
    const char* data = "Callback function called";

    // String de consulta que va a variar dependiendo de la denominacion de la cuenta
    const char *clientes;

    // Conexion a la base de datos y mensaje de repuesta
    rc = sqlite3_open("banco.db", &db);
    if(rc){
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return 0;
    } else {
        cout << "Ingreso correcto a la base de datos" << endl;
    }

    // Solicitar datos acerca del emisor de la transferencia
    cout << "Por favor ingrese el número de cedula del cliente que va a realizar al transferencia" << endl;
    getline(cin, cedula);

    do {
        cout << "¿Desea transferir desde la cuenta en colones o en dólares?" << endl;
        cout << "1) Dólares" << std::endl;
        cout << "2) Colones" << std::endl;

        // Verificar si la entrada del usuario es válida
        if (!(cin >> cuenta_op)) {
            cout << "\n";
            cout << "Opcion no valida. Por favor, ingrese un numero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar el resto de la línea
            continue;
        }

        switch (cuenta_op)
        {
        case DOLARES:
            denominacion = "dolares";

            // Primera consulta
            clientes = "SELECT cuenta_dolares from clientes WHERE cedula = ?";
            break;
        case COLONES:
            denominacion = "colones";
            
            // Primera consulta
            clientes = "SELECT cuenta_colones from clientes WHERE cedula = ?";
            break;
        default:
            cout << "¡La opción ingresada no es válida! Por favor, inténtelo de nuevo.";
            break;
        }
    } while (cuenta_op != DOLARES && cuenta_op != COLONES);

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, clientes, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }

    // Parametro para la consulta
    sqlite3_bind_text(stmt, 1, cedula.c_str(), -1, SQLITE_STATIC);

    // Realizar primera consulta para obtener el numero de cuenta (puede ser dolares o colones)
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        numero_cuenta = sqlite3_column_int(stmt, 0);
    } else if (rc == SQLITE_DONE) {
        cout << "No existe ninguna cuenta asociada a este número de cédula" << endl;
        return 0;
    } else {
        cerr << "Error de SQL." << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_finalize(stmt);  // Finalizar consulta

    // Crear una consulta extra para obtener el balance y compararlo con el monto
    const char *cuentasExt = "SELECT balance FROM cuentas WHERE numero_cuenta = ?";

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, cuentasExt, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }

    // Parametros de la consulta
    sqlite3_bind_double(stmt, 1, numero_cuenta);

    // Realizar la consulta
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        balance = sqlite3_column_double(stmt, 0);
        cout << "El valor del balance asociado fue accesado correctamente" << endl;
    } else if (rc == SQLITE_DONE) {
        cout << "No fue posible obtener el valor del balance asociado a esta cuenta" << endl;
        return 0;
    } else {
        cerr << "Error de SQL." << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_finalize(stmt);  // Finalizar consulta

    // Crear la tercera consulta para actualizar balance de la cuenta
    const char *cuentas = "UPDATE cuentas "
                          "SET balance = balance - ?"
                          "WHERE numero_cuenta = ?";

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, cuentas, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }

    // Preguntar el monto que desea transferir
    cout << "Por favor, ingrese el monto que desea transferir: " << endl;
    cin >> montoTransferencia;
    cin.ignore();

    // Verificacion monto
    if (montoTransferencia <= 0) {
        cout << "¡Este monto no es válido!";
        return 0;
    } else if (balance < montoTransferencia) {
        cout << "La cuenta no presenta el saldo suficiente para transferir este monto" << endl;
        return 0;
    }

    // Parametros de la consulta
    sqlite3_bind_double(stmt, 1, montoTransferencia);
    sqlite3_bind_double(stmt, 2, numero_cuenta);

    // Consulta para rebajar el monto de la cuenta emisora
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        cout << "Retiro realizado correctamente." << endl;
    } else if (rc != SQLITE_DONE) {
        // Hubo un error en la consulta
        cerr << "No fue posible realizar el retiro: " << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_finalize(stmt);  // Finalizar consulta

    // Solicitar el numero de cuenta del receptor de la transferencia
    cout << "Por favor, ingrese el número de cuenta al que desea realizar la transferencia: " << endl;
    cin >> numeroCuentaReceptor;
    cin.ignore();

    /* Es necesario hacer una consulta a la tabla de cuentas para identificar la denominacion de la cuenta
    receptora, ya que si las cuentas involucradas dentro de una transaccion no tienen el mismo tipo de
    denominacion es necesario ejecutar una conversion de la moneda*/
    const char *cuentasDos = "SELECT denominacion from cuentas WHERE numero_cuenta = ?";

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, cuentasDos, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }

    // Parametro para la consulta
    sqlite3_bind_double(stmt, 1, numeroCuentaReceptor);

    // Realizar la consulta para obtener la denominacion de la cuenta receptora
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Es necesario declarar este puntero ya que SQL utiliza este tipo de variable y se necesita un string
        const unsigned char *denominacionUnsChar = sqlite3_column_text(stmt, 0);
        if (denominacionUnsChar) {
            denominacionReceptor = reinterpret_cast<const char*>(denominacionUnsChar);
        } else {
            cout << "No fue posible obtener la denominación de la cuenta receptora" << std::endl;
            return 0;
        }
    } else if (rc == SQLITE_DONE) {
        cout << "No existe ninguna cuenta asociada a este número de identificación." << endl;
        return 0;
    } else {
        cerr << "Error de SQL." << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_finalize(stmt);  // Finalizar consulta

    // Ahora se realiza una comparacion de las denominaciones de ambas cuentas
    if (denominacion != denominacionReceptor) {
        // Primer caso, la demominacion del emisor estara en colones y la del receptor en dolares
        if (denominacion == "colones") {
            montoTransferencia /= TASA_DE_CAMBIO;
        } else {
            // En este segundo caso la denominacion del emisor estara en dolares y la del receptor en colones
            montoTransferencia *= TASA_DE_CAMBIO;
        }
    }

    // Una vez convertido el monto de la transferencia (en caso de ser necesario), se crea la ultima consulta
    const char *cuentasTres = "UPDATE cuentas "
                          "SET balance = balance + ? "
                          "WHERE numero_cuenta = ?";

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, cuentasTres, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }

    // Parametro para la consulta
    sqlite3_bind_double(stmt, 1, montoTransferencia);
    sqlite3_bind_double(stmt, 2, numeroCuentaReceptor);

    // Realizar la segunda consulta
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        cout << "¡La transferencia fue realizada de manera exitosa!" << endl;
    } else {
        cerr << "Error de SQL." << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_finalize(stmt);  // Finalizar statement

    // Crear una consulta final para obtener la cedula del cliente destino
    const char *clientesDos = "SELECT cedula FROM clientes "
                              "WHERE cuenta_colones = ? OR "
                              "cuenta_dolares = ?";

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, clientesDos, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }

    // Parametros
    sqlite3_bind_double(stmt, 1, numeroCuentaReceptor);
    sqlite3_bind_double(stmt, 2, numeroCuentaReceptor);    

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Se obtiene la cedula del cliente destino de la transaccion
        clienteDestinoCedula = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    } else if (rc == SQLITE_DONE) {
        cerr << "No se encontró ninguna cedula para las cuentas dadas." << endl;
    } else {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
    } 

    // Guardar los datos para el registro
    clienteOrigenCedula = cedula;
    montoBase = montoTransferencia;

    // Liberar memoria y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
};

int Operaciones::abonosPrestamos(std::string &denominacion, std::string &clienteOrigenCedula, std::string &clienteDestinoCedula, float &montoBase) {
    string cliente_id;

    unsigned int prestamo_id;
    string denominacionPrestamo;
    string tipoPrestamo;
    unsigned int numero_cuenta;
    unsigned int cuota_mensual;
    double monto_total;
    int plazo_meses;
    int plazo_restante;
    float tasa;

    double balance;
    int cuenta_op;
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg = 0;
    int rc;
    const char* data = "Callback function called";

    // Conexion a la base de datos y mensaje de repuesta
    rc = sqlite3_open("banco.db", &db);
    if(rc){
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return 0;
    } else {
        cout << "Ingreso correcto a la base de datos" << endl;
    }

    // Solicitar cedula (cliente_id) para buscar en la tabla los prestamos asociados
    cout << "Por favor ingrese el número de cedula del cliente que desea realizar el abono" << endl;
    getline(cin, cliente_id);

    // Crear consulta para desplegar los prestamos que tiene asociados el cliente
    const char *prestamos = "SELECT * FROM prestamos "
                            "WHERE cliente_id = ?";

    // Preparar la consulta parametrizada
    rc = sqlite3_prepare_v2(db, prestamos, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }

    // Parametros para la consulta
    sqlite3_bind_text(stmt, 1, cliente_id.c_str(), -1, SQLITE_STATIC);

    cout << "Préstamos asociados al cliente: " << endl;
    // A partir de esta consulta, se despliegan todos los prestamos relacionados con el cliente
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        prestamo_id = sqlite3_column_int(stmt, 0);
        denominacionPrestamo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        tipoPrestamo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        monto_total = sqlite3_column_double(stmt, 3);
        plazo_meses = sqlite3_column_int(stmt, 4);
        plazo_restante = sqlite3_column_int(stmt, 5);
        cuota_mensual = sqlite3_column_int(stmt, 6);
        tasa = sqlite3_column_double(stmt, 7);

        cout << " Préstamo ID: "<< prestamo_id << " Denominación: " << denominacionPrestamo << " Tipo de Préstamo: "
        << tipoPrestamo << " Monto total: " << std::fixed << std::setprecision(2) << monto_total << " Plazo (meses): " << plazo_meses <<
        " Plazo restante (cuotas):" << plazo_restante << " Cuota mensual: " << std::fixed << std::setprecision(2) << cuota_mensual << " Tasa: " << tasa << endl;
    }
if (rc != SQLITE_DONE) {
    cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

    sqlite3_finalize(stmt);  // Finalizar statement

    /* Ahora el cliente debe escoger a cual prestamo realizar el abono y cual cuenta
    va a utilizar para pagar la cuota.*/

    cout << "Por favor, ingrese el número de identificación del préstamo al que desea abonar" << endl;
    cin >> prestamo_id;
    cin.ignore();

    // Se tiene que hacer una consulta para obtener justamente, la cuota mensual de este prestamo
    const char *prestamosDos = "SELECT denominacion, cuota_mensual, cliente_id from prestamos WHERE prestamo_id = ?";

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, prestamosDos, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }

    // Parametros para la consulta
    sqlite3_bind_double(stmt, 1, prestamo_id);

    // Realiza la consulta en donde obtiene la cuota
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        denominacionPrestamo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        cuota_mensual = sqlite3_column_double(stmt, 1);

        // Se obtiene el cliente destino (ya que puede ser diferente al cliente origen)
        clienteDestinoCedula = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        cout << "¡El valor de la cuota mensual se obtuvo correctamente!" << endl;
    } else if(rc == SQLITE_DONE){
        cerr << "No se encontró un préstamo con este ID" << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    } else {
        cerr << "Error de SQL (step): " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;      
    }

    sqlite3_finalize(stmt);  // Finalizar consulta

    // String de consulta que va a variar dependiendo de la denominacion de la cuenta
    const char *clientes;

    do {
        cout << "¿Desea abonar desde la cuenta en colones o en dólares?" << endl;
        cout << "1) Dólares" << std::endl;
        cout << "2) Colones" << std::endl;

        // Verificar si la entrada del usuario es válida
        if (!(cin >> cuenta_op)) {
            cout << "\n";
            cout << "Opcion no valida. Por favor, ingrese un numero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar el resto de la línea
            continue;
        }

        switch (cuenta_op)
        {
        case DOLARES:
            denominacion = "dolares";

            // Primera consulta
            clientes = "SELECT cuenta_dolares from clientes WHERE cedula = ?";
            break;
        case COLONES:
            denominacion = "colones";
            
            // Primera consulta
            clientes = "SELECT cuenta_colones from clientes WHERE cedula = ?";
            break;
        default:
            cout << "¡La opción ingresada no es válida! Por favor, inténtelo de nuevo.";
            break;
        }
    } while (cuenta_op != DOLARES && cuenta_op != COLONES);

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, clientes, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }

    // Parametro para la consulta
    sqlite3_bind_text(stmt, 1, cliente_id.c_str(), -1, SQLITE_STATIC);

    // Realiza segunda consulta para obtener el numero de cuenta
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        numero_cuenta = sqlite3_column_int(stmt, 0);
    } else if (rc == SQLITE_DONE) {
        cout << "No existe ninguna cuenta asociada a este número de cédula" << endl;
        return 0;
    } else {
        cerr << "Error de SQL." << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_finalize(stmt);  // Finalizar consulta

    // Anadir consulta extra para obtener el balance
    const char *cuentasExt = "SELECT balance FROM cuentas WHERE numero_cuenta = ?";

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, cuentasExt, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }

    // Parametros de la consulta
    sqlite3_bind_double(stmt, 1, numero_cuenta);

    // Realizar la consulta
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        balance = sqlite3_column_double(stmt, 0);
        cout << "El valor del balance asociado fue accesado correctamente" << endl;
    } else if (rc == SQLITE_DONE) {
        cout << "No fue posible obtener el valor del balance asociado a esta cuenta" << endl;
        return 0;
    } else {
        cerr << "Error de SQL." << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    } 

    sqlite3_finalize(stmt);  // Finalizar consulta

    // El programa debe manejar casos en que se utilice una denominacion diferente para pagar el prestamo
    // Ahora se realiza una comparacion de las denominaciones de ambas cuentas
    if (denominacion != denominacionPrestamo) {
        // Primer caso, el prestamo en colones se cancela con una cuenta en dolares
        if (denominacionPrestamo == "colones") {
            cuota_mensual /= TASA_DE_CAMBIO;
        } else {
            // Segundo caso, el prestamo en dolares se cancela con una cuenta en colones
            cuota_mensual *= TASA_DE_CAMBIO;
        }
    }

    // Con el numero de cuenta, se realiza una consulta para realizar el rebajo a la cuenta del cliente
    const char *cuentas = "UPDATE cuentas "
                          "SET balance = balance - ? "
                          "WHERE numero_cuenta = ?";

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, cuentas, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }

    // Revisar que el balance no sea menor a la cuota mensual
    if (balance < cuota_mensual) {
        cout << "El saldo no es suficiente para pagar la cuota" << endl;
        return 0;
    } 

    // Parametros de la consulta
    sqlite3_bind_double(stmt, 1, cuota_mensual);
    sqlite3_bind_double(stmt, 2, numero_cuenta);

    // Realizar la segunda consulta para modificar el balance en caso de que se pueda pagar la cuota
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        cout << "Rebajo realizado exitosamente." << endl;
    } else if (rc != SQLITE_DONE) {
        // Hubo un error en la consulta
        cerr << "No fue posible sustraer el dinero de la cuenta." << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_finalize(stmt);  // Finalizar consulta

    // Ultima consulta para reducir plazo restante del prestamo
    const char *prestamosTres = "UPDATE prestamos "
                                "SET plazo_restante = plazo_restante - 1 "
                                "WHERE prestamo_id = ? ";

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, prestamosTres, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }    

    // Parametros de la consulta
    sqlite3_bind_double(stmt, 1, prestamo_id);

    // Hacer la ultima consulta
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        cout << "Abono realizado correctamente." << endl;
    } else if (rc != SQLITE_DONE) {
        // Hubo un error en la consulta
        cerr << "No fue posible realizar el abono." << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Guardar los datos para el registro
    clienteOrigenCedula = cliente_id;
    montoBase = cuota_mensual;

    // Liberar memoria y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
};
