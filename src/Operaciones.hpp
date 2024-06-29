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

#include "funcionesGenerales.hpp"

class Operaciones {
    public:
        //
        int deposito();

        //
        int retiro(std::string &denominacion, std::string &clienteOrigenCedula, std::string &clienteDestinoCedula, float &montoBase);

        //
        int transferencias(std::string &denominacion, std::string &clienteOrigenCedula, std::string &clienteDestinoCedula, float &montoBase);

        //
        int abonosPrestamos(std::string &denominacion, std::string &clienteOrigenCedula, std::string &clienteDestinoCedula, float &montoBase);

        //Esta es la funcion para poder ver el progreso del cdp
        /**
        * @brief Accede a la información para observar el progreso de los cdps
        * @return Int que determina el funcionamiento de la función
        */
        int gestionAhorros();

        //Esta es la solicitud de prestamos
        /**
        * @brief Permite solicitar un préstamo
        * @return Int que determina el funcionamiento de la función
        */
        int solicitudPrestamos();


};

#endif