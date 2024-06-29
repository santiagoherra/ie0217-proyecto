/**
 * @file Operaciones.hpp
 * @version 1.0
 * @date 6/2024
 * @authors JoséIgnacioSáenzDíaz, SantiagoHerraCastro, KevinJiménezAcuña
 * @brief Retiro, depósito, abono, transferencia y ahorros
*/

#ifndef OPERACIONES_HPP
#define OPERACIONES_HPP

#include <iostream>
#include <stdexcept>
#include <string>
#include <limits>
#include <sstream>
#include <iomanip>
#include <sqlite3.h>
#include <thread>
#include <chrono>

class Operaciones {
    public:
        //
        int deposito(std::string &denominacion, std::string &clienteOrigenCedula, std::string &clienteDestinoCedula, float &montoBase);

        //
        int retiro(std::string &denominacion, std::string &clienteOrigenCedula, std::string &clienteDestinoCedula, float &montoBase);

        //
        int transferencias(std::string &denominacion, std::string &clienteOrigenCedula, std::string &clienteDestinoCedula, float &montoBase);

        //
        int abonosPrestamos(std::string &denominacion, std::string &clienteOrigenCedula, std::string &clienteDestinoCedula, float &montoBase);

        //Esta es la funcion para poder ver el progreso del cdp
        int gestionAhorros();

        //Esta es la solicitud de prestamos
        int solicitudPrestamos();


};

#endif