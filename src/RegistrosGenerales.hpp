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

/* En esta clase se encontrarán los distintos métodos de actualización (es decir,
ingresar registros) y los distintos tipos de visualización implementados para los
registros (actualmente la visualización completa del registro y la visualización
con filtro). Para utilizar cada uno de estos métodos es esencial contar con el
motor mySQL, una base de datos salvada en una nube y el conector C++ propio de
mySQL.*/
class RegistrosGenerales {
    public:
        // Este metodo se encarga de almacenar los datos mas importantes acerca de todas las transaccioes realizadas
        void actualizarRegistro(std::string tipo_transaccion, std::string fecha_transaccion, std::string denominacion,
                                std::string cliente_origen_cedula, std::string cliente_destino_cedula, float monto_base);

        // Este metodo permite visualizar el historial general de transacciones
        void verRegistro() const;

        // Este metodo permite visualizar el registro de un solo tipo de transaccion (depositos, retiros, prestamos...)
        void filtrarRegistro () const;

        // Este metodo imprime el registro de transacciones de un cliente
        void registroPersonal () const;
};

#endif