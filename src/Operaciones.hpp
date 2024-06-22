/**
 * @file Operaciones.hpp
 * @brief Headerfile en donde se definen los metodos que realizan las operaciones.
**/

#ifndef OPERACIONES_HPP
#define OPERACIONES_HPP

class Operaciones {
    public:
        //
        int deposito();

        //
        int retiro();

        //
        int transferencias();

        //
        int abonosPrestamos();

        //Esta es la funcion para poder ver el progreso del cdp
        int gestionAhorros();

        //Esta es la solicitud de prestamos
        int solicitudPrestamos();


};

#endif