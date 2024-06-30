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

/**
 * @class Operaciones
 * @brief En esta clase se encuentran los métodos que permiten a los clientes realizar acciones.
 * 
 * Cada uno de estos métodos, acceden a la base de datos para obtener información y en algunos casos modificar los registros para de esta 
 * forma, realizar las operaciones deseadas por el cliente. Es necesario mencionar que algunos de estos métodos reciben parámetros por
 * referencia, esto resulta útil ya que de esta forma es posible modificar ciertas variables que después serán utilizadas para actualizar
 * la tabla de registros de la base de datos. 
 */
class Operaciones {
    public:
        
        /**
        * @brief Este método interactua con la base de datos para realizar depósitos.
        * 
        * @param denominacion Registra el tipo de denominación que se utilizó para realizar la transacción
        * @param clienteOrigenCedula Registra el número de cédula del cliente que emite la transacción (el cliente que deposita)
        * @param clienteDestinoCedula Registra el número de cédula del cliente que recibe la transacción (en el caso de depósito, es el mismo que la emite)
        * @param montoBase Registra el monto involucrado en la transacción
        * @return Este entero determina si la transacción fue exitosa o no
        */
        int deposito(std::string &denominacion, std::string &clienteOrigenCedula, std::string &clienteDestinoCedula, float &montoBase);

        /**
        * @brief Método que interactua con la base de datos para realizar retiros.
        * 
        * @param denominacion Registra el tipo de denominación que se utilizó para realizar la transacción
        * @param clienteOrigenCedula Registra el número de cédula del cliente que emite la transacción (el cliente que retira)
        * @param clienteDestinoCedula Registra el número de cédula del cliente que recibe la transacción (en el caso de retiro, es el mismo que la emite)
        * @param montoBase Registra el monto involucrado en la transacción
        * @return Este entero determina si la transacción fue exitosa o no
        */
        int retiro(std::string &denominacion, std::string &clienteOrigenCedula, std::string &clienteDestinoCedula, float &montoBase);

        /**
        * @brief Este método interactua con la base de datos para realizar transferencias bancarias entre clientes.
        * 
        * @param denominacion Registra el tipo de denominación que se utilizó para realizar la transacción
        * @param clienteOrigenCedula Registra el número de cédula del cliente que emite la transferencia
        * @param clienteDestinoCedula Registra el número de cédula del cliente que recibe la transferencia
        * @param montoBase Registra el monto involucrado en la transacción
        * @return Este entero determina si la transacción fue exitosa o no
        */
        int transferencias(std::string &denominacion, std::string &clienteOrigenCedula, std::string &clienteDestinoCedula, float &montoBase);

        //Esta es la funcion para poder ver el progreso del cdp
        /**
        * @brief Este método accede a la base de datos para cancelar cuotas de préstamos registrados en el banco.
        * 
        * @param denominacion Registra el tipo de denominación que se utilizó para realizar la transacción
        * @param clienteOrigenCedula Registra el número de cédula del cliente que emite la transacción (cliente que realiza el abono)
        * @param clienteDestinoCedula Registra el número de cédula del cliente que recibe la transacción (en el caso de préstamo, es el mismo que realiza el abono)
        * @param montoBase Registra el monto involucrado en la transacción
        * @return Este entero determina si la transacción fue exitosa o no
        */
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