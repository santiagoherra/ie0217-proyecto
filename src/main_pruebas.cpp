#include "CuentaAhorros.hpp"
#include <iostream>
#include <string>



int main(){

    std::string cedula = "6969696";

    CuentaAhorros cuenta;
    bool existe;
    existe = cuenta.existeCliente(cedula);

    std::cout << existe << std::endl;  

    cuenta.agregarCliente(cedula, "santiago", "herra", 69696969, 1234567);

    return 0;
}