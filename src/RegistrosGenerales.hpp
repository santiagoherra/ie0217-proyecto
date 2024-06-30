/**
 * @file RegistrosGenerales.cpp
 * @version 1.0
 * @date 6/2024
 * @authors JoséIgnacioSáenzDíaz, SantiagoHerraCastro, KevinJiménezAcuña
 * @brief Clase registro, manejo y despliegue de estos.
*/
#ifndef REGISTROS_GENERALES_HPP
#define REGISTROS_GENERALES_HPP

#include <string>
#include <iostream>
#include <sqlite3.h>
#include <iomanip>
#include "funcionesGenerales.hpp"

/**
 * @class RegistrosGenerales
 * @brief En esta clase se encuentran los métodos utilizados para todo lo relacionado con registros generales y personales.
 * 
 * Los métodos de esta clase acceden a la base de datos para actualizar y obtener de forma detallada, toda la información referente al
 * historial de transacciones, préstamos, créditos de depósito a plazo, y cuentas de clientes que han sido registradas en la base
 * de datos del banco. De esta forma, se ofrece un historial de acciones que puede ser útil para el personal del banco y también sus
 * clientes.
 */
class RegistrosGenerales {
    public:
        /**
        * @brief Este método accede a la tabla registros de la base de datos para guardar las transacciones exitosas
        * realizadas en el banco.
        * 
        * @param tipo_transaccion Indica que tipo de transacción se efectuó (retiro, depósito, etc)
        * @param fecha_transaccion Fecha en la que se realizó la transacción
        * @param denominacion Indica el tipo de moneda que se utilizó para realizar la transacción
        * @param cliente_origen_cedula Número de cédula del cliente que emitió la transferencia
        * @param cliente_destino_cedula Número de cédula del cliente que recibió la transferencia (en algunos casos es el mismo que el cliente origen)
        * @param montoBase Monto que estuvo involucrado en la transferencia
        */
        void actualizarRegistro(std::string tipo_transaccion, std::string fecha_transaccion, std::string denominacion,
                                std::string cliente_origen_cedula, std::string cliente_destino_cedula, float monto_base);

        /**
         * @brief Este método accede a la tabla registros de la base de datos y permite visualizar el historial completo de transacciones del banco.
         */
        void verRegistro() const;

        /**
         * @brief Este método accede a la tabla registros y obtiene el historial de transacciones de un tipo de transacción en específico.
         */
        void filtrarRegistro () const;

        /**
         * @brief Este método realiza varias consultas para obtener así, toda la nformación relevante de: las cuentas, préstamos, créditos de depośito
         * a plazo y transacciones registradas con su número de cédula.
         */
        void registroPersonal () const;
};

#endif