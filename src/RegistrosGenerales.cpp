/**
 * @file RegistrosGenerales.cpp
 * @brief En este archivo de código fuente se definen los métodos actualizarRegistro,
 * verRegistro y filtrarRegistro. Es necesario destacar que se utiliza una consulta
 * parametrizada para reducir el riesgo de un SQL Injection; a pesar de esto, aún es
 * necesario mejorar la robustez del código.
 */

#include "RegistrosGenerales.hpp"
#include <iostream>
#include <sqlite3.h>

using namespace std;

// Funcion de callback
static int callback(void *data, int argc, char**argv, char **azColName){
    for(int i = 0; i < argc; i++){
        cout << azColName[i] << (argv[i] ? argv[i] : "NULL") << endl;
    }
    return 0;
}

void RegistrosGenerales::actualizarRegistro(int ID, std::string tipo_transaccion, std::string fecha_transaccion, std::string denominacion,
                                int cliente_origen_cedula, int cliente_destino_cedula, float monto_base) {
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

    // Anadir nueva transaccion al registro
    const char *anadirRegistro = "INSERT INTO registros (ID, tipo_transaccion, fecha_transaccion, denominacion, cliente_origen_cedula, cliente_destino_cedula, monto_base)"
                                  "VALUES (?, ?, ?, ?, ?, ?, ?)";

    // Preparar la consulta parametrizada
    rc = sqlite3_prepare_v2(db, anadirRegistro, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Parametros para la consulta
    sqlite3_bind_int(stmt, 1, ID);
    sqlite3_bind_text(stmt, 2, tipo_transaccion.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, fecha_transaccion.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, denominacion.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, cliente_origen_cedula);
    sqlite3_bind_int(stmt, 6, cliente_destino_cedula);
    sqlite3_bind_double(stmt, 7, monto_base);

    // Realizar la consulta parametrizada
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Error de SQL: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << "Transacción guardada correctamente en el registro general" << endl;
    }
    
    // Liberar memoria y cerrar el programa
    sqlite3_finalize(stmt);
    sqlite3_close(db);
};

void RegistrosGenerales::verRegistro() const {

};

void RegistrosGenerales::filtrarRegistro () const {

};