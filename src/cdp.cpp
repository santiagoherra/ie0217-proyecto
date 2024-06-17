#include "cdp.hpp"
#include <iostream>


void CDP::inicializarDatos(){

    informacionIntereses[30] = {0.0301, 0.0161};
    informacionIntereses[59] = {0.0301, 0.0161};
    informacionIntereses[60] = {0.0345, 0.0166};
    informacionIntereses[89] = {0.0345, 0.0166};
    informacionIntereses[90] = {0.0388, 0.0196}; 
    informacionIntereses[119] = {0.0388, 0.0196}; 
}

int CDP::calculoIntereses(){
    int interes_banco;

    interes_banco = interes * plazoTiempo * monto;

    return interes_banco;
}

void CDP::interesAcumulados(){
    std::string cedula;
    std::cout <<  "Ingrese el numero de cedula al cual esta asociado el certificado de plazo." << std::endl;
    std::cin >> cedula;
    


}