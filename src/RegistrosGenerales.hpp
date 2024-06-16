/**
 * @file RegistrosGenerales.hpp
 * @brief En este headerfile se declaran los métodos de la clase RegistrosGenerales,
 * utilizada para todas aquellas acciones requeridas para el registro general de
 * transacciones de la aplicación.
**/
#ifndef REGISTROS_GENERALES_HPP
#define REGISTROS_GENERALES_HPP

#include <string>

/* En esta clase se encontrarán los distintos métodos de actualización (es decir,
ingresar registros) y los distintos tipos de visualización implementados para los
registros (actualmente la visualización completa del registro y la visualización
con filtro). Para utilizar cada uno de estos métodos es esencial contar con el
motor mySQL, una base de datos salvada en una nube y el conector C++ propio de
mySQL.*/
class RegistrosGenerales {
    public:
        // Este metodo se encarga de almacenar los datos mas importantes acerca de todas las transaccioes realizadas
        void actualizarRegistro(int ID, std::string tipo_transaccion, std::string fecha_transaccion, std::string denominacion,
                                int cliente_origen_cedula, int cliente_destino_cedula, float monto_base);

        // Este metodo permite visualizar el historial general de transacciones
        void verRegistro() const;

        // Este metodo permite visualizar el registro de un solo tipo de transaccion (depositos, retiros, prestamos...)
        void filtrarRegistro () const;
};

#endif