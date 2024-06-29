/**
 * @file cdp.hpp
 * @version 1.0
 * @date 6/2024
 * @authors JoséIgnacioSáenzDíaz, SantiagoHerraCastro, KevinJiménezAcuña
 * @brief Funciones asociadas a la posibilidad de añadir CDPs y verificar que el cliente saldo suficiente
*/

#ifndef CDPS_HPP
#define CDPS_HPP
#include <string>
#include <sqlite3.h>

#include <unordered_map>

class CDP{
    public:
        /**
        * @brief Agrega un CDP para un cliente dado
        * @return Función tipo Void
        */
        void agregarCDP();
        /**
        * @brief Verifica si el saldo es suficiente como para obtener un CDP
        * @param db Base de datos con la que se trabaja
        * @param cedula Cédula del cliente
        * @param denominacion Colones o Dólares
        * @param monto Monto del CDP
        * @return Bool que indica si se tienen o no los fondos
        */
        bool verificarSaldoSuficiente(sqlite3* db, const std::string& cedula, int denominacion, double monto);
};

#endif