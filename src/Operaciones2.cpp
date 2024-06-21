#include "Operaciones.hpp"
#include <prestamos.hpp>

//Este archivo consisten en las funciones de solicitud de un prestamo, apertura de un cdp, retiro de un cdp
//y gestion de ahorros.

int Operaciones::solicitudPrestamos(){
    Prestamos prestamo;
    prestamo.menu();
    prestamo.seguirConPrestamo();
}

