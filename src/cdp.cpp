/**
 * @file cdp.cpp
 * @version 1.0
 * @date 6/2024
 * @authors JoséIgnacioSáenzDíaz, SantiagoHerraCastro, KevinJiménezAcuña
 * @brief Implementación: Funciones asociadas a la posibilidad de añadir CDPs y verificar que el cliente saldo suficiente
*/
#include "cdp.hpp"
#include "funcionesGenerales.hpp"
#include "Operaciones.hpp"
#include <iostream>
#include <iostream>
#include <string>
#include <sqlite3.h>

// Función abre base de datos
sqlite3* conectarDB() {
    sqlite3* db;
    int rc = sqlite3_open("banco.db", &db);
    if (rc) {
        std::cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        exit(1);
    }
    return db;
}

// Función para agregar un Certificado de Depósito a Plazo
void CDP::agregarCDP() {
    // Conectar a la base de datos
    sqlite3* db = conectarDB();

    // Solicitar la cedula
    std::string cedula;
    std::cout << "Ingrese la cedula del cliente: ";
    std::cin >> cedula;
    if (!leerCedula2(cedula)) {
        cout << "La cedula introducida no es valida.";
        return; // Salir del programa si la cédula no es válida
    }

    // Verificar si el cliente existe
    bool clienteExiste = existeCliente(cedula);
    if (!clienteExiste) {
        std::cout << "El cliente no existe. No se puede agregar un CDP." << std::endl;
        sqlite3_close(db);
        return;
    }

    // Solicitar la denominación (1 para colones, 2 para dólares)
    int denominacion;
    std::string denominacion1;
    std::cout << "Seleccione la denominacion (1 para colones, 2 para dolares): ";
    std::cin >> denominacion;
    leerInt(denominacion);

    // Tabla de tasas e plazos
    const double tasasColones[] = {0.0519, 0.0559, 0.0569, 0.0604};
    const double tasasDolares[] = {0.0281, 0.0378, 0.0441, 0.0448};
    const int plazos[] = {6, 12, 18, 24};

    // Mostrar opciones según la denominación elegida
    if (denominacion == 1) {
        std::cout << "Opciones de plazo para colones:" << std::endl;
        for (int i = 0; i < 4; ++i) {
            std::cout << i+1 << ". Plazo: " << plazos[i] << " meses, Tasa: " << tasasColones[i] << std::endl;
        }
        denominacion1 = "colones";
    } else if (denominacion == 2) {
        std::cout << "Opciones de plazo para dolares:" << std::endl;
        for (int i = 0; i < 4; ++i) {
            std::cout << i+1 << ". Plazo: " << plazos[i] << " meses, Tasa: " << tasasDolares[i] << std::endl;
        }
        denominacion1 = "dolares";
    } else {
        std::cout << "Denominacion no valida." << std::endl;
        sqlite3_close(db);
        return;
    }

    // Solicitar el plazo
    int opcionPlazo;
    std::cout << "Seleccione el plazo (1-4): ";
    std::cin >> opcionPlazo;
    if (opcionPlazo < 1 || opcionPlazo > 4) {
        std::cout << "Opcion de plazo no valida." << std::endl;
        sqlite3_close(db);
        return;
    }
    int plazoSeleccionado = plazos[opcionPlazo - 1];
    double tasaSeleccionada = (denominacion == 1) ? tasasColones[opcionPlazo - 1] : tasasDolares[opcionPlazo - 1];

    // Solicitar el monto
    double monto;
    if (denominacion == 1) {
        std::cout << "Ingrese el monto en colones (entre 1,000,000 y 10,000,000): ";
        std::cin >> monto;
        if (monto < 1000000 || monto > 10000000) {
            std::cout << "Monto fuera de rango." << std::endl;
            sqlite3_close(db);
            return;
        }
    } else if (denominacion == 2) {
        std::cout << "Ingrese el monto en dolares (entre 2,000 y 20,000): ";
        std::cin >> monto;
        if (monto < 2000 || monto > 20000) {
            std::cout << "Monto fuera de rango." << std::endl;
            sqlite3_close(db);
            return;
        }
    }

    bool saldoSuficiente = verificarSaldoSuficiente(db, cedula, denominacion, monto);
    if (!saldoSuficiente) {
        sqlite3_close(db);
        return;
    }

    // Insertar el CDP en la base de datos
    std::string sql = "INSERT INTO certificados_de_deposito (denominacion, tasa, plazo_meses, monto_deposito, fecha_deposito, cliente_cedula) "
                      "VALUES (?, ?, ?, ?, DATE('now'), ?)";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        exit(1);
    }

    sqlite3_bind_text(stmt, 1, denominacion1.c_str(), -1 , SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, tasaSeleccionada);
    sqlite3_bind_int(stmt, 3, plazoSeleccionado);
    sqlite3_bind_double(stmt, 4, monto);
    sqlite3_bind_text(stmt, 5, cedula.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Error al insertar el CDP: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Certificado de Deposito agregado exitosamente." << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

bool CDP::verificarSaldoSuficiente(sqlite3* db, const std::string& cedula, int denominacion, double monto) {
    std::string tipoDenominacion = (denominacion == 1) ? "colones" : "dolares";

    // Construir el número de cuenta basado en la denominación y cédula del cliente
    std::string numeroCuentaStr;
    if (denominacion == 1) {
        numeroCuentaStr = "1" + cedula; // Cuenta en colones
    } else if (denominacion == 2) {
        numeroCuentaStr = "2" + cedula; // Cuenta en dólares
    }

    // Consultar el saldo de la cuenta
    std::string sql = "SELECT balance FROM cuentas WHERE numero_cuenta = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, numeroCuentaStr.c_str(), -1, SQLITE_STATIC);

    // Ejecutar la consulta
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        std::cerr << "Error al consultar el saldo de la cuenta." << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Obtener el saldo de la cuenta
    double saldo = sqlite3_column_double(stmt, 0);
    sqlite3_finalize(stmt);

    // Verificar si el saldo es suficiente
    if (saldo >= monto) {
        // Actualizar el saldo de la cuenta
        double nuevoSaldo = saldo - monto;
        sql = "UPDATE cuentas SET balance = ? WHERE numero_cuenta = ?";
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Error al preparar la consulta de actualizacion de saldo: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        sqlite3_bind_double(stmt, 1, nuevoSaldo);
        sqlite3_bind_text(stmt, 2, numeroCuentaStr.c_str(), -1, SQLITE_STATIC);

        // Ejecutar la consulta de actualización
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Error al actualizar el saldo de la cuenta." << std::endl;
            sqlite3_finalize(stmt);
            return false;
        }

        sqlite3_finalize(stmt);
        return true;
    } else {
        std::cout << "No se tienen fondos suficientes para adquirir este CDP." << std::endl;
        return false;
    }
}
