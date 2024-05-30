#include "CuentaAhorros.hpp"
#include <iostream>
#include <fstream>
#include <string>

#define TASACOLONES 0.0521
#define TASADOLARES 0.0292

void CuentaAhorros::printInfoCuentaAhorroRegular() const{
    // Obtener archivo de texto
    std::string cuentaAhorroRegularTexto = "txt/CuentaAhorroRegular.txt";

    // Objeto de tipo fstream
    std::fstream info(cuentaAhorroRegularTexto);

    // Mensaje de error en caso de que no se abra el archivo
    if(!info.is_open()) {
        std::cerr << "Error: no fue posible desplegar la información." << std::endl;
    }

    // Bucle para leer todas las lineas
    std::string linea;
    while(std::getline(info, linea)) {
        std::cout << linea << std::endl;
    }

    info.close();  // Cerrar el archivo de texto
};

void CuentaAhorros::printInfoCDP() const{
    // Obtener archivo de texto
    std::string cdpTexto = "txt/CDP.txt";

    // Objeto de tipo fstream
    std::fstream info(cdpTexto);

    // Mensaje de error en caso de que no se abra el archivo
    if(!info.is_open()) {
        std::cerr << "Error: no fue posible desplegar la información." << std::endl;
    }

    // Bucle para leer todas las lineas
    std::string linea;
    while(std::getline(info, linea)) {
        std::cout << linea << std::endl;
    }

    info.close();  // Cerrar el archivo de texto
};

void CuentaAhorros::calculadoraIntereses() const{
    int montoPrincipal;
    char moneda;
    int tiempo;
    int interesesGanados;
    char otroCalculo;

    // Bucle para que el usuario pueda calcular los intereses ganados varias veces
    do{
        std::cout << "CALCULADORA DE INTERESES" << std::endl;
        std::cout << "Ingrese el monto principal que desea abonar:" << std::endl;
        std::cin >> montoPrincipal;

        std::cout << "Ingrese el tiempo en años que desea para el CDP:" << std::endl;
        std::cin >> tiempo;

        std::cout << "Ingrese el tipo de moneda que desea utilizar:" << std::endl;
        std::cout << "1) Colones" << std::endl;
        std::cout << "2) Dólares" << std::endl;
        std::cin >> moneda;

        // Dependiendo de la moneda seleccionada, se utiliza una tasa diferente
        switch (moneda)
        {
        case '1':
            // Verificar que montoPrincipal no sea menor que el monto minimo en colones
            try {
                if (montoPrincipal < 50000) {
                    throw std::runtime_error("Un CDP en colones no puede contar con un monto mínimo de 50000₡");
                }

                // Calcular los intereses ganados e imprimirlo
                interesesGanados = montoPrincipal * TASACOLONES * tiempo;
                std::cout << "Intereses ganados = " << interesesGanados << std::endl << std::endl;

            }   catch(const std::runtime_error& e){
                std::cout << e.what() << std::endl;
            }
            break;
        case '2':
            // Verificar que montoPrincipal no sea menor que el monto minimo en dolares
            try {
                if (montoPrincipal < 100) {
                    throw std::runtime_error("Un CDP en dólares no puede contar con un monto mínimo de 100$");
                }

                // Calcular los intereses ganados e imprimirlo
                interesesGanados = montoPrincipal * TASADOLARES * tiempo;
                std::cout << "Intereses ganados = " << interesesGanados << std::endl << std::endl;

            }   catch(const std::runtime_error& e){
                std::cout << e.what() << std::endl;
            }
            break;
        }

        std::cout << "¿Desea obtener otro cálculo?" << std::endl;
        std::cout << "Presione cualquier tecla para realizar otro cálculo, presione 's' para salir." << std::endl;
        std::cin >> otroCalculo;

    } while(otroCalculo != 's');

};