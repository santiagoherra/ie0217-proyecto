/**
 * @file Operaciones.cpp
 * @brief .
 */

// Este valor se puede cambiar cada vez que se utilice el programa
#define TASA_DE_CAMBIO 525.95

#include "Operaciones.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iomanip>
#include <sqlite3.h>

using namespace std;

static int callback(void *data, int argc, char**argv, char **azColName){
    for(int i = 0; i < argc; i++){
        cout << azColName[i] << (argv[i] ? argv[i] : "NULL") << endl;
    }
    return 0;
}

int Operaciones::deposito() {
    string cedula;
    int numero_cuenta;
    string denominacion;
    float montoDepositar;
    char cuenta_op;
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

    // Solicitar datos para realizar el deposito
    cout << "Por favor ingrese el número de cedula del cliente al que se va a depositar" << endl;
    getline(cin, cedula);

    do {
        cout << "¿Desea realizar el depósito en la cuenta de dólares o colones?" << endl;
        cout << "1) Dólares" << std::endl;
        cout << "2) Colones" << std::endl;
        cin >> cuenta_op;
        cin.ignore();

        switch (cuenta_op)
        {
        case '1':
            denominacion = "dolares";

            // Primera consulta
            clientes = "SELECT cuenta_colones from clientes WHERE cedula = ?";
            break;
        case '2':
            denominacion = "colones";
            
            // Primera consulta
            clientes = "SELECT cuenta_dolares from clientes WHERE cedula = ?";
            break;
        default:
            cout << "¡La opción ingresada no es válida! Por favor, inténtelo de nuevo.";
            break;
        }
    } while (cuenta_op != '1' && cuenta_op != '2');

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
        return 0;
    }

    // Parametro para la consulta
    sqlite3_bind_double(stmt, 1, montoDepositar);
    sqlite3_bind_int(stmt, 2, numero_cuenta);

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

    // Liberar memoria y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
}; 

int Operaciones::retiro() {
    string cedula;
    int numero_cuenta;
    string denominacion;
    float balance;
    float montoRetirar;
    char cuenta_op;
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

    // Solicitar datos para realizar el retiro
    cout << "Por favor ingrese el número de cedula del cliente que va a realizar el retiro" << endl;
    getline(cin, cedula);

    do {
        cout << "¿Desea realizar el depósito en la cuenta de dólares o colones?" << endl;
        cout << "1) Dólares" << std::endl;
        cout << "2) Colones" << std::endl;
        cin >> cuenta_op;
        cin.ignore();

        switch (cuenta_op)
        {
        case '1':
            denominacion = "dolares";

            // Primera consulta
            clientes = "SELECT cuenta_colones from clientes WHERE cedula = ?";
            break;
        case '2':
            denominacion = "colones";
            
            // Primera consulta
            clientes = "SELECT cuenta_dolares from clientes WHERE cedula = ?";
            break;
        default:
            cout << "¡La opción ingresada no es válida! Por favor, inténtelo de nuevo.";
            break;
        }
    } while (cuenta_op != '1' && cuenta_op != '2');

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
        balance /= 100;  // Conversion necesaria por formato SQL
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
    sqlite3_bind_int(stmt, 2, numero_cuenta);

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

    // Liberar memoria y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
};

int Operaciones::transferencias() {
    string cedula;
    int numero_cuenta;
    int numeroCuentaReceptor;
    string denominacion;
    string denominacionReceptor;
    float montoTransferencia;
    float balance;
    char cuenta_op;
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
        cin >> cuenta_op;
        cin.ignore();

        switch (cuenta_op)
        {
        case '1':
            denominacion = "dolares";

            // Primera consulta
            clientes = "SELECT cuenta_colones from clientes WHERE cedula = ?";
            break;
        case '2':
            denominacion = "colones";
            
            // Primera consulta
            clientes = "SELECT cuenta_dolares from clientes WHERE cedula = ?";
            break;
        default:
            cout << "¡La opción ingresada no es válida! Por favor, inténtelo de nuevo.";
            break;
        }
    } while (cuenta_op != '1' && cuenta_op != '2');

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
        balance /= 100;
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
    sqlite3_bind_int(stmt, 2, numero_cuenta);

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
    sqlite3_bind_int(stmt, 1, numeroCuentaReceptor);

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
    sqlite3_bind_int(stmt, 2, numeroCuentaReceptor);

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

    // Liberar memoria y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
};

int Operaciones::abonosPrestamos() {
    string cliente_id;
    int prestamo_id;
    string descripcion;
    string denominacion;
    int numero_cuenta;
    int cuota_mensual;
    double monto_total;
    float balance;
    char cuenta_op;
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
        int prestamo_id = sqlite3_column_int(stmt, 0);
        std::string descripcion = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        double monto_total = sqlite3_column_double(stmt, 2);
        double cuota_mensual = sqlite3_column_double(stmt, 3);

    	cout << "Préstamo ID: " << prestamo_id << ", Descripción: " << descripcion << ", Monto: " << monto_total <<
                ", Cuota Mensual: " << std::fixed << std::setprecision(2) << cuota_mensual << endl;
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
    const char *prestamosDos = "SELECT cuota_mensual from prestamos WHERE prestamo_id = ?";

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, prestamosDos, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }

    // Parametros para la consulta
    sqlite3_bind_int(stmt, 1, prestamo_id);

    // Realiza la consulta en donde obtiene la cuota
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        cuota_mensual = sqlite3_column_int(stmt, 0);
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
        cin >> cuenta_op;
        cin.ignore();

        switch (cuenta_op)
        {
        case '1':
            denominacion = "dolares";

            // Primera consulta
            clientes = "SELECT cuenta_colones from clientes WHERE cedula = ?";
            break;
        case '2':
            denominacion = "colones";
            
            // Primera consulta
            clientes = "SELECT cuenta_dolares from clientes WHERE cedula = ?";
            break;
        default:
            cout << "¡La opción ingresada no es válida! Por favor, inténtelo de nuevo.";
            break;
        }
    } while (cuenta_op != '1' && cuenta_op != '2');

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
        balance /= 100;
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
    sqlite3_bind_int(stmt, 2, numero_cuenta);

    // Realizar la segunda consulta para modificar el balance en caso de que se pueda pagar la cuota
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

    // Liberar memoria y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
};
