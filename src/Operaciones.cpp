/**
 * @file Operaciones.cpp
 * @brief .
 */

#include "Operaciones.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
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
    string numero_cuenta;
    string denominacion;
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
        return (0);
    } else {
        cout << "Ingreso correcto a la base de datos" << endl;
    }

    // Solicitar datos para realizar el deposito
    cout << "Por favor ingrese el número de cedula" << endl;
    getline(cin, cedula);

    do {
        cout << "¿Desea realizar el depósito en la cuenta de dólares o colones?" << endl;
        cout << "1) Dólares" << std::endl;
        cout << "2) Colones" << std::endl;
        cin >> cuenta_op;

        switch (cuenta_op)
        {
        case 1:
            denominacion = dolares;

            // Primera consulta
            const char *clientes = "SELECT cuenta_colones from clientes WHERE cedula = ?";
            break;
        case 2:
            denominacion = colones;
            
            // Segunda consulta
            const char *clientes = "SELECT cuenta_dolares from clientes WHERE cedula = ?"
            break;
        default:
            cout << "¡La opción ingresada no es válida! Por favor, inténtelo de nuevo."
            break;
        }
    } while (cuenta_op != '1' && cuenta_op != '2');

    // Preparar consulta parametrizada
    rc = sqlite3_prepare_v2(db, clientes, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Parametro para la consulta
    sqlite3_bind_text(stmt, 1, cedula.c_str(), -1, SQLITE_STATIC);

    // Realizar primera consulta para obtener el numero de cuenta (puede ser dolares o colones)
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        numero_cuenta = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    } else if (rc == SQLITE_DONE) {
        cout << "No existe ninguna cuenta asociada a este número de cédula" << id << endl;
    } else {
        cerr << "Error de SQL." << sqlite3_errmsg(db) << endl;
    }
    
    // Crear la segunda consulta
    const char *cuentas = "";
    
    // Liberar memoria y cerrar la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);
};  