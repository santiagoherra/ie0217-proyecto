/**
 * @file RegistrosGenerales.cpp
 * @version 1.0
 * @date 6/2024
 * @authors JoséIgnacioSáenzDíaz, SantiagoHerraCastro, KevinJiménezAcuña
 * @brief Implementación: Clase registro, manejo y despliegue de estos.
*/

#include "RegistrosGenerales.hpp"

using namespace std;

// Funcion de callback
static int callback(void *data, int argc, char**argv, char **azColName){
    for(int i = 0; i < argc; i++){
        cout << azColName[i] << (argv[i] ? argv[i] : "NULL") << endl;
    }
    return 0;
}

void RegistrosGenerales::actualizarRegistro(std::string tipo_transaccion, std::string fecha_transaccion, std::string denominacion,
                                            string cliente_origen_cedula, string cliente_destino_cedula, float monto_base)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg = 0;
    int rc;
    const char *data = "Callback function called";

    // Conexion a la base de datos y mensaje de repuesta
    rc = sqlite3_open("banco.db", &db);
    if (rc)
    {
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return;
    }
    else
    {
        cout << "Ingreso correcto a la base de datos" << endl;
    }

    // Inicio de la transacción
    rc = sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error al iniciar la transacción: " << errMsg << endl;
        sqlite3_close(db);
        return;
    }


    // Anadir nueva transaccion al registro
    const char *anadirRegistro = "INSERT INTO registros (tipo_transaccion, fecha_transaccion, denominacion, cliente_origen_cedula, cliente_destino_cedula, monto_base)"
                                 "VALUES (?, ?, ?, ?, ?, ?)";

    // Preparar la consulta parametrizada
    rc = sqlite3_prepare_v2(db, anadirRegistro, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Parametros para la consulta
    sqlite3_bind_text(stmt, 1, tipo_transaccion.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, fecha_transaccion.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, denominacion.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, cliente_origen_cedula.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, cliente_destino_cedula.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 6, monto_base);

    // Realizar la consulta parametrizada
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
    }
    else
    {
        cout << "Transacción guardada correctamente en el registro general" << endl;
    }

    // Confirmar la transacción
    rc = sqlite3_exec(db, "COMMIT", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error al confirmar la transacción: " << errMsg << endl;
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, &errMsg);
        sqlite3_close(db);
        return;
    }

    // Liberar memoria y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);
};

void RegistrosGenerales::verRegistro() const {
    int ID;
    string tipo_transaccion;
    string fecha_transaccion;
    string denominacion;
    string cliente_origen_cedula;
    string cliente_destino_cedula;
    double monto_base;

    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg = 0;
    int rc;
    const char* data = "Callback function called";

    // Conexion a la base de datos y mensaje de repuesta
    rc = sqlite3_open("banco.db", &db);
    if(rc){
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return;
    } else {
        cout << "Ingreso correcto a la base de datos" << endl;
    }

    // Mostrar todo el registro
    const char *mostrarRegistro = "SELECT * FROM registros";

    // Preparar la consulta parametrizada
    rc = sqlite3_prepare_v2(db, mostrarRegistro, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }   

    cout << "\nRegistro General de transacciones " << endl;
    // Con este bucle se obtienen las cuentas que cumplen las caracteristicas
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        ID = sqlite3_column_int(stmt, 0);
        tipo_transaccion = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        fecha_transaccion = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        denominacion = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        cliente_origen_cedula = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        cliente_destino_cedula = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        monto_base = sqlite3_column_double(stmt, 6);

        cout << "Transacción ID: " << ID << " Tipo de transacción: " << tipo_transaccion << " Fecha: " <<
                fecha_transaccion << " Denominación: " << denominacion << " Cliente origen: " <<
                cliente_origen_cedula << " Cliente destino: " << cliente_destino_cedula <<
                " Monto: " << monto_base << endl;
    } 

    if (rc != SQLITE_DONE) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);  // Cerrar la base de datos
};

void RegistrosGenerales::filtrarRegistro () const {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg = 0;
    int rc;
    string filtro;
    const char* data = "Callback function called";

    // Conexion a la base de datos y mensaje de repuesta
    rc = sqlite3_open("banco.db", &db);
    if(rc){
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return;
    } else {
        cout << "Ingreso correcto a la base de datos" << endl;
    }

    // Usuario ingresa el filtro
	cout << "Por favor ingrese el tipo de transacción para el cual desea visualizar las transacciones:" << std::endl;
	getline(cin, filtro);

    // Mostrar todo el registro
    const char *mostrarRegistroFiltrado = "SELECT * FROM registros "
                                  "WHERE tipo_transaccion = ?";

    // Preparar la consulta parametrizada
    rc = sqlite3_prepare_v2(db, mostrarRegistroFiltrado, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Parametros para la consulta
    sqlite3_bind_text(stmt, 1, filtro.c_str(), -1, SQLITE_STATIC);

    // Realizar la consulta parametrizada
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << "Registro de transacciones para " << filtro << endl;
    }
    
    // Liberar memoria y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);
};

void RegistrosGenerales::registroPersonal() const {
    string cedula;
    string nombre;
    string apellido;

    unsigned int cuentaColones;
    unsigned int cuentaDolares;
    unsigned int numero_cuenta;
    double balance;
    float tasa;
    string denominacion;

    string tipo_transaccion;
    string fecha_transaccion;
    string cliente_origen_cedula;
    string cliente_destino_cedula;
    float monto_base;

    int prestamo_id;
    string tipo_prestamo;
    float monto_total;
    int plazo_meses;
    int plazo_restante;
    int cuota_mensual;

    int cdp_id;
    float monto_deposito;
    string fecha_deposito;

    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg = 0;
    int rc;
    const char *data = "Callback function called";

    // Conexion a la base de datos y mensaje de repuesta
    rc = sqlite3_open("banco.db", &db);
    if (rc)
    {
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return;
    }
    else
    {
        cout << "Ingreso correcto a la base de datos" << endl;
    }

    // Solicitar datos para obtener registro personal
    cout << "Por favor ingrese el número de cedula del cliente" << endl;
    getline(cin, cedula);

    // Crear consulta para obtener informacion sobre las cuentas registradas para el cliente
    const char *regPersonalClientes = "SELECT nombre, apellido, cuenta_colones, cuenta_dolares "
                                      "FROM clientes "
                                      "WHERE cedula = ?";

    // Preparar la consulta parametrizada
    rc = sqlite3_prepare_v2(db, regPersonalClientes, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Parametro para la consulta
    sqlite3_bind_text(stmt, 1, cedula.c_str(), -1, SQLITE_STATIC);

    // Realizar la consulta y obtener los datos
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        nombre = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        apellido = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        cuentaColones = sqlite3_column_int(stmt, 2);
        cuentaDolares = sqlite3_column_int(stmt, 3);

        //CHECKPOINT
        cout << "CuentaColones: " << cuentaColones << endl;
        cout << "CuentaDolares: " << cuentaDolares << endl;
    } else if (rc == SQLITE_DONE) {
        cout << "No existe ninguna cuenta asociada a este número de cédula" << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    } else {
        cerr << "Error de SQL." << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    cout << "\n\nCliente: " << nombre << " " << apellido << endl;

    sqlite3_finalize(stmt);  // Finalizar statement

    // Crear consulta para obtener la informacion de las cuentas del cliente
    const char *regPersonalCuentas = "SELECT * FROM  cuentas "
                                     "WHERE numero_cuenta = ? OR numero_cuenta = ? ";

    // Preparar la consulta parametrizada
    rc = sqlite3_prepare_v2(db, regPersonalCuentas, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }       

    // Parametros para la consulta
    sqlite3_bind_double(stmt, 1, cuentaColones);
    sqlite3_bind_double(stmt, 2, cuentaDolares); 

    cout << "\nCuentas asociadas al cliente " << endl;
    // Con este bucle se obtienen las cuentas que cumplen las caracteristicas
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            numero_cuenta = sqlite3_column_int(stmt, 0);
            balance = sqlite3_column_double(stmt, 1);
            tasa = sqlite3_column_double(stmt, 2);
            denominacion = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

            cout << "Número de cuenta: " << numero_cuenta << " Balance: " << std::fixed <<
            std::setprecision(2) << balance << " Tasa: " << tasa << " Denominación: " << denominacion << endl;
    }                        

    // Crear la consulta para obtener un registro con las transacciones en las que se involucra al cliente
    const char *regPersonalTransacciones = "SELECT * FROM registros " 
                                           "WHERE cliente_origen_cedula = ? OR "
                                           "cliente_destino_cedula = ?";

    // Preparar la consulta parametrizada
    rc = sqlite3_prepare_v2(db, regPersonalTransacciones, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Parametro para la consulta
    sqlite3_bind_text(stmt, 1, cedula.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, cedula.c_str(), -1, SQLITE_STATIC);

    cout << "\nRegistro de transacciones asociadas al cliente " << endl;
    // A partir de esta consulta, se despliegan todos los movimientos relacionados con el cliente
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        tipo_transaccion = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        fecha_transaccion = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        denominacion = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        cliente_origen_cedula = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        cliente_destino_cedula = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        monto_base = sqlite3_column_int(stmt, 6);

        cout << " Tipo transacción: " << tipo_transaccion << " Fecha transacción: " << fecha_transaccion <<
        " Denominación: " << denominacion << " Cliente origen: " << cliente_origen_cedula << " Cliente destino: "
        << cliente_destino_cedula << " Monto: " << std::fixed << std::setprecision(2) << monto_base << endl;
    }

    if (rc != SQLITE_DONE) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    sqlite3_finalize(stmt);  // Finalizar statement

    // Crear la consulta para obtener un registro de todos los prestamos del cliente
    const char *regPersonalPrestamos = "SELECT * FROM prestamos "
                            "WHERE cliente_id = ?";


    // Preparar la consulta parametrizada
    rc = sqlite3_prepare_v2(db, regPersonalPrestamos, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }
    // Parametros para la consulta
    sqlite3_bind_text(stmt, 1, cedula.c_str(), -1, SQLITE_STATIC);

    cout << "\nPréstamos asociados al cliente: " << endl;
    // A partir de esta consulta, se despliegan todos los prestamos relacionados con el cliente
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        prestamo_id = sqlite3_column_int(stmt, 0);
        denominacion = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        tipo_prestamo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        monto_total = sqlite3_column_double(stmt, 3);
        plazo_meses = sqlite3_column_int(stmt, 4);
        plazo_restante = sqlite3_column_int(stmt, 5);
        cuota_mensual = sqlite3_column_int(stmt, 6);
        tasa = sqlite3_column_double(stmt, 7);

        cout << " Préstamo ID: "<< prestamo_id << " Denominación: " << denominacion << " Tipo de Préstamo: "
        << tipo_prestamo << " Monto total: " << std::fixed << std::setprecision(2) << monto_total << " Plazo (meses): " << plazo_meses <<
        " Plazo restante (cuotas):" << plazo_restante << " Cuota mensual: " << std::fixed << std::setprecision(2) << cuota_mensual << " Tasa: " << tasa << endl;
    }

    if (rc != SQLITE_DONE) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    sqlite3_finalize(stmt);  // Finalizar statement

    // Crear consulta para obtener el registro de todos los CDPS del cliente
    const char *regPersonalCDPS = "SELECT * FROM certificados_de_deposito "
                            "WHERE cliente_cedula = ?";

    // Preparar la consulta parametrizada
    rc = sqlite3_prepare_v2(db, regPersonalCDPS, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Parametros para la consulta
    sqlite3_bind_text(stmt, 1, cedula.c_str(), -1, SQLITE_STATIC);

    cout << "\nCréditos de depósito a plazo asociados al cliente: " << endl;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        cdp_id = sqlite3_column_int(stmt, 0);
        denominacion = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        tasa = sqlite3_column_double(stmt, 2);
        plazo_meses = sqlite3_column_int(stmt, 3);
        monto_deposito = sqlite3_column_double(stmt, 4);
        fecha_deposito = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

        cout << "CDP ID: " << cdp_id << " Denominación: " << denominacion << " Tasa: " << tasa <<
        " Plazo (meses): " << plazo_meses << " Monto depósito: " << std::fixed << std::setprecision(2) <<
        monto_deposito << " Fecha depósito: " << fecha_deposito << endl;
    }

    if (rc != SQLITE_DONE) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    sqlite3_finalize(stmt);  // Finalizar statement
    sqlite3_close(db);
};