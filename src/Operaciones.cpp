/**
 * @file Operaciones2.cpp
 * @version 1.0
 * @date 6/2024
 * @authors JoséIgnacioSáenzDíaz, SantiagoHerraCastro, KevinJiménezAcuña
 * @brief Implementación: Retiro, depósito, abono, transferencia y ahorros
*/

// Este valor se puede cambiar cada vez que se utilice el programa
#define TASA_DE_CAMBIO 525.95

#include "Operaciones.hpp"
#include "funcionesGenerales.hpp"


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

int Operaciones::deposito(std::string &denominacion, std::string &clienteOrigenCedula, std::string &clienteDestinoCedula, float &montoBase) {
    // Variables para realizar el depósito
    std::string cedula;

    // Variables que no se que son
    unsigned int numero_cuenta;
    double montoDepositar;
    int cuenta_op;

    // Variables SQL
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg = 0;
    int rc;

    // String de consulta que va a variar dependiendo de la denominación de la cuenta
    const char* clientes;

    // Conexión a la base de datos y mensaje de respuesta
    rc = sqlite3_open("banco.db", &db);
    if(rc != SQLITE_OK){
        std::cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        return 0;
    } else {
        std::cout << "Ingreso correcto a la base de datos" << std::endl;
    }

    // Iniciar la transacción
    rc = sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error al iniciar la transaccion: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 0;
    }

    // Solicitar datos para realizar el depósito
    std::cout << "Por favor ingrese el numero de cedula del cliente al que se va a depositar" << std::endl;
    std::getline(std::cin, cedula);

    if (!leerCedula2(cedula)) {
        cout << "La cedula introducida no es valida.";
        return 0; // Salir del programa si la cédula no es válida
    }

    std::cout << "¿Desea realizar el deposito en la cuenta de dolares o colones?" << std::endl;
    std::cout << "1) Dolares" << std::endl;
    std::cout << "2) Colones" << std::endl;
    std::cin >> cuenta_op;

    leerInt(cuenta_op, 1, 2);

    if(cuenta_op == DOLARES){
        denominacion = "dolares";
        clientes = "SELECT cuenta_dolares FROM clientes WHERE cedula = ?";
    } else if(cuenta_op == COLONES){
        denominacion = "colones";
        clientes = "SELECT cuenta_colones FROM clientes WHERE cedula = ?";
    }

    rc = sqlite3_prepare_v2(db, clientes, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "No se puede preparar la declaracion: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, &errMsg);
        sqlite3_close(db);
        return 1;
    }

    // Parámetro para la consulta
    sqlite3_bind_text(stmt, 1, cedula.c_str(), -1, SQLITE_STATIC);

    // Realizar primera consulta para obtener el número de cuenta (puede ser dólares o colones)
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        numero_cuenta = sqlite3_column_int(stmt, 0);
        std::cout << "El numero de cuenta del cliente en " << denominacion << " es " << numero_cuenta << std::endl;  // CHECKPOINT
    } else if (rc == SQLITE_DONE) {
        std::cout << "No existe ninguna cuenta asociada a este numero de cedula" << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, &errMsg);
        sqlite3_close(db);
        return 0;
    } else {
        std::cerr << "Error de SQL." << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, &errMsg);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_finalize(stmt);

    // Crear la segunda consulta para actualizar balance de la cuenta
    const char *cuentas = "UPDATE cuentas SET balance = balance + ? WHERE numero_cuenta = ?";

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, cuentas, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error de SQL: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, &errMsg);
        sqlite3_close(db);
    }

    // Preguntar el monto que desea depositar
    std::cout << "Por favor, ingrese el monto que desea depositar: " << std::endl;
    std::cin >> montoDepositar;

    leerInt(montoDepositar);

    // Verificación monto
    if (montoDepositar <= 0) {
        std::cout << "¡El monto a depositar no es valido!" << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, &errMsg);
        sqlite3_close(db);
        return 0;
    }

    // Parámetros para la consulta
    sqlite3_bind_double(stmt, 1, montoDepositar);
    sqlite3_bind_int(stmt, 2, numero_cuenta);

    // Realizar la segunda consulta
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        std::cout << "¡El deposito fue realizado de manera exitosa!" << std::endl;
    } else {
        std::cerr << "Error de SQL: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, &errMsg);
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
        std::cerr << "Error al confirmar la transaccion: " << errMsg << std::endl;
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, &errMsg);
        sqlite3_close(db);
        return 0;
    }

    // Liberar memoria y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    std::cin.ignore();

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
        cerr << "Error al iniciar la transaccion: " << errMsg << endl;
        sqlite3_close(db);
        return 0;
    }

    // Solicitar datos para realizar el retiro
    cout << "Por favor ingrese el numero de cedula del cliente que va a realizar el retiro" << endl;
    getline(cin, cedula);

    if (!leerCedula2(cedula)) {
        cout << "La cedula introducida no es valida.";
        return 0; // Salir del programa si la cédula no es válida
    }

    do {
        cout << "¿Desea realizar el deposito en la cuenta de dolares o colones?" << endl;
        cout << "1) Dolares" << std::endl;
        cout << "2) Colones" << std::endl;
        cin >> cuenta_op;

        leerInt(cuenta_op, 1, 2);

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
            cout << "La opcion ingresada no es valida! Por favor, intentelo de nuevo.";
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
        cout << "No existe ninguna cuenta asociada a este numero de cedula" << endl;
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

    // Verifiacion entrada
    leerInt(montoRetirar);

    // Verificar monto
    if (montoRetirar <= 0) {
        cout << "El monto a retirar no es valido!" << endl;
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
        cerr << "Error al confirmar la transaccion: " << errMsg << endl;
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
    cout << "Por favor ingrese el numero de cedula del cliente que va a realizar al transferencia" << endl;
    getline(cin, cedula);

    if (!leerCedula2(cedula)) {
        cout << "La cedula introducida no es valida.";
        return 0; // Salir del programa si la cédula no es válida
    }

    do {
        cout << "¿Desea transferir desde la cuenta en colones o en dolares?" << endl;
        cout << "1) Dolares" << std::endl;
        cout << "2) Colones" << std::endl;
        cin >> cuenta_op;

        // Verificar si la entrada del usuario es válida
        leerInt(cuenta_op, 1, 2);

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
            cout << "La opcion ingresada no es valida! Por favor, inténtelo de nuevo.";
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
        cout << "No existe ninguna cuenta asociada a este numero de cedula" << endl;
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

    // Verificacion de entrada
    leerInt(montoTransferencia);

    // Verificacion monto
    if (montoTransferencia <= 0) {
        cout << "Este monto no es valido!";
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
    cout << "Por favor, ingrese el numero de cuenta al que desea realizar la transferencia: " << endl;
    cin >> numeroCuentaReceptor;
    cin.ignore();

    // Verificacion de entrada
    leerInt(numeroCuentaReceptor);

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
            cout << "No fue posible obtener la denominacion de la cuenta receptora" << std::endl;
            return 0;
        }
    } else if (rc == SQLITE_DONE) {
        cout << "No existe ninguna cuenta asociada a este numero de identificacion." << endl;
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
        cout << "La transferencia fue realizada de manera exitosa!" << endl;
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
        cerr << "No se encontro ninguna cedula para las cuentas dadas." << endl;
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
    cout << "Por favor ingrese el numero de cedula del cliente que desea realizar el abono" << endl;
    getline(cin, cliente_id);

    if (!leerCedula2(cliente_id)) {
        cout << "La cedula introducida no es valida.";
        return 0; // Salir del programa si la cédula no es válida
    }

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

    cout << "Prestamos asociados al cliente: " << endl;
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

        cout << " Prestamo ID: "<< prestamo_id << " Denominacion: " << denominacionPrestamo << " Tipo de Prestamo: "
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

    cout << "Por favor, ingrese el numero de identificacion del prestamo al que desea abonar" << endl;
    cin >> prestamo_id;
    cin.ignore();

    leerInt(prestamo_id);

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
        cout << "El valor de la cuota mensual se obtuvo correctamente!" << endl;
    } else if(rc == SQLITE_DONE){
        cerr << "No se encontro un prestamo con este ID" << endl;
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
        cout << "¿Desea abonar desde la cuenta en colones o en dolares?" << endl;
        cout << "1) Dolares" << std::endl;
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
            cout << "La opcion ingresada no es valida! Por favor, intentelo de nuevo.";
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
        cout << "No existe ninguna cuenta asociada a este numero de cedula" << endl;
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

int Operaciones::gestionAhorros() {
    bool existe_cliente;
    std::string cedula;
    
    std::cout << "Esta es la seccion de gestion de ahorros para los clientes." << std::endl;
    std::cout << "Porfavor ingrese su numero de cedula para verificar si la persona esta ingresada en el sistema." << std::endl;
    std::getline(std::cin, cedula);

    if (!leerCedula2(cedula)) {
        cout << "La cedula introducida no es valida.";
        return 0; // Salir del programa si la cédula no es válida
    }
    existe_cliente = existeCliente(cedula);

    if (!existe_cliente) {
        std::cout << "La persona de la cedula " << cedula << " no esta ingresada en el sistema." << std::endl;
        return 0;
    }

    std::cout << "Certificados de Deposito a Plazo asociados a la ID de cliente " << cedula << std::endl;

    sqlite3 *db;
    int rc;

    rc = sqlite3_open("banco.db", &db);
    if (rc) {
        std::cerr << "No se puede abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    const char* sqlConsulta = "SELECT * FROM certificados_de_deposito WHERE cliente_cedula = ?;";

    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sqlConsulta, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "No se puede preparar la declaracion: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    std::cout << std::fixed << std::setprecision(2);

    sqlite3_bind_text(stmt, 1, cedula.c_str(), -1, SQLITE_STATIC);

    bool tieneCDP = false;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        tieneCDP = true;
        int cdp_id = sqlite3_column_int(stmt, 0);
        const unsigned char *denominacion = sqlite3_column_text(stmt, 1);
        double tasa = sqlite3_column_double(stmt, 2);
        int plazo_meses = sqlite3_column_int(stmt, 3);
        double monto_deposito = sqlite3_column_double(stmt, 4);
        const unsigned char *fecha_deposito = sqlite3_column_text(stmt, 5);

        std::cout << "\nCDP ID: " << cdp_id
                  << "\nDenominacion: " << denominacion
                  << "\nTasa: " << tasa
                  << "\nPlazo (meses): " << plazo_meses
                  << "\nMonto Deposito: " << monto_deposito
                  << "\nFecha de deposito: " << fecha_deposito << std::endl;
    }

    sqlite3_finalize(stmt);

    if (!tieneCDP) {
        std::cout << "\nEl cliente no tiene ningun Certificado de Deposito a Plazo asociado." << std::endl;
        sqlite3_close(db);
        return 0;
    }

    int cdp_elegido;

    std::cout << "\nEn base al numero de ID del Certificado de Deposito a Plazo, elija el numero del CDP ID para poder ver los progreso generado hasta ahora." << std::endl;

    std::cin >> cdp_elegido; // Es el id del cdp

    const char* recuperarCDPsql = "SELECT cdp_id, denominacion, tasa, plazo_meses, monto_deposito, fecha_deposito "
                                  "FROM certificados_de_deposito WHERE cdp_id = ? AND cliente_cedula = ?;";

    rc = sqlite3_prepare_v2(db, recuperarCDPsql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "No se puede preparar la declaracion: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_int(stmt, 1, cdp_elegido);
    sqlite3_bind_text(stmt, 2, cedula.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int cdp_id = sqlite3_column_int(stmt, 0);
        const unsigned char* denominacion = sqlite3_column_text(stmt, 1);
        double tasa = sqlite3_column_double(stmt, 2);
        int plazo = sqlite3_column_int(stmt, 3);
        double monto_deposito = sqlite3_column_double(stmt, 4);
        const unsigned char* fecha_deposito = sqlite3_column_text(stmt, 5);

        // Guardar los valores en variables locales
        std::string denominacion_str = reinterpret_cast<const char*>(denominacion);
        std::string fecha_deposito_str = reinterpret_cast<const char*>(fecha_deposito);

        std::cout << "+---------------------------------+" << std::endl;
        std::cout << "|       CDP ID      |    " << cdp_id << std::endl;
        std::cout << "+---------------------------------+" << std::endl;
        std::cout << "|    Denominacion   |   " << denominacion_str << std::endl;
        std::cout << "+---------------------------------+" << std::endl;
        std::cout << "|        Tasa       |    " << tasa << std::endl;
        std::cout << "+---------------------------------+" << std::endl;
        std::cout << "|       Plazo       |    " << plazo << std::endl;
        std::cout << "+---------------------------------+" << std::endl;
        std::cout << "|       Monto       |   " << monto_deposito << std::endl;
        std::cout << "+---------------------------------+" << std::endl;
        std::cout << "| Fecha de Deposito |   " << fecha_deposito_str << std::endl; 
        std::cout << "+---------------------------------+" << std::endl;

        std::tm tm_fecha_deposito = string_a_fecha(fecha_deposito_str);

        // Obtener la fecha actual
        std::time_t t = std::time(nullptr);
        std::tm tm_actual = *std::localtime(&t);

        // Convertir tm a time_t para calcular la diferencia
        std::time_t tiempo_deposito = std::mktime(&tm_fecha_deposito);
        std::time_t tiempo_actual = std::mktime(&tm_actual);

        // Calcular la diferencia de tiempo en días
        double diferencia_tiempo = difftime(tiempo_actual, tiempo_deposito) / (10000 * 60 * 60 * 24.0); // Diferencia en días

        // Calcular intereses acumulados
        double interes_acumulados = tasa * diferencia_tiempo * monto_deposito;
        std::cout << "\n+------------------------------------+" << std::endl;
        std::cout << "| Intereses hasta ahora   | " << std::fixed << std::setprecision(2) << interes_acumulados << " |" << std::endl;
        std::cout << "+------------------------------------+" << std::endl;

    } else {
        std::cout << "No se encontro ningun Certificado de Deposito a plazo con ese ID asociado al cliente." << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
