#include "CuentaAhorros.hpp"
#include <iostream>
#include <string>



int main(){

    std::string cedula = "6969696";

    CuentaAhorros cuenta;
    bool existe;
    existe = cuenta.existeCliente(cedula);

    std::cout << existe << std::endl;  
    

    return 0;
}