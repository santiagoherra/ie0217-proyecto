#include "CuentaAhorros.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

// En estas constantes se almacenan las tasas establecidas para la calculadora de intereses
#define TASACOLONES 0.0521
#define TASADOLARES 0.0292

enum OpcionesCuentaAhorrosInfo{ // Define la enumeracion de las opciones del menu
    COLONES = 1,
    DOLARES
};


void CuentaAhorros::printInfoCuentaAhorroRegular() const{
    // Obtener archivo de texto
    string cuentaAhorroRegularTexto = "txt/CuentaAhorroRegular.txt";

    // Objeto de tipo fstream
    fstream info(cuentaAhorroRegularTexto);

    // Mensaje de error en caso de que no se abra el archivo
    if(!info.is_open()) {
        cerr << "Error: no fue posible desplegar la información." << endl;
    }

    // Bucle para leer todas las lineas
    string linea;
    while(getline(info, linea)) {
        cout << linea << endl;
    }

    info.close();  // Cerrar el archivo de texto
};

void CuentaAhorros::printInfoCDP() const{
    // Obtener archivo de texto
    string cdpTexto = "txt/CDP.txt";

    // Objeto de tipo fstream
    fstream info(cdpTexto);

    // Mensaje de error en caso de que no se abra el archivo
    if(!info.is_open()) {
        cerr << "Error: no fue posible desplegar la información." << endl;
    }

    // Bucle para leer todas las lineas
    string linea;
    while(getline(info, linea)) {
        cout << linea << endl;
    }

    info.close();  // Cerrar el archivo de texto
};

void CuentaAhorros::calculadoraIntereses() const{
    int montoPrincipal;
    int moneda;
    int tiempo;
    int interesesGanados;
    char otroCalculo;

    // Bucle para que el usuario pueda calcular los intereses ganados varias veces
    do{
        cout << "CALCULADORA DE INTERESES" << endl;
        cout << "Ingrese el monto principal que desea abonar:" << endl;
        cin >> montoPrincipal;

        cout << "Ingrese el tiempo en anios que desea para el CDP:" << endl;
        cin >> tiempo;

        cout << "Ingrese el tipo de moneda que desea utilizar:" << endl;
        cout << "1) Colones" << endl;
        cout << "2) Dolares" << endl;

        // Verificar si la entrada del usuario es válida
        if (!(cin >> moneda)) {
            cout << "\n";
            cout << "Opcion no valida. Por favor, ingrese un numero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar el resto de la línea
            continue;
        }

        // Dependiendo de la moneda seleccionada, se utiliza una tasa diferente
        switch (moneda)
        {
        case COLONES:
            // Verificar que montoPrincipal no sea menor que el monto minimo en colones
            try {
                if (montoPrincipal < 50000) {
                    throw std::runtime_error("Un CDP en colones no puede contar con un monto minimo de 50000₡");
                }

                // Calcular los intereses ganados e imprimirlo
                interesesGanados = montoPrincipal * TASACOLONES * tiempo;
                cout << "Intereses ganados = " << interesesGanados << endl << endl;

            }   catch(const std::runtime_error& e){
                std::cout << e.what() << std::endl;
            }
            break;
        case DOLARES:
            // Verificar que montoPrincipal no sea menor que el monto minimo en dolares
            try {
                if (montoPrincipal < 100) {
                    throw std::runtime_error("Un CDP en dolares no puede contar con un monto minimo de 100$");
                }

                // Calcular los intereses ganados e imprimirlo
                interesesGanados = montoPrincipal * TASADOLARES * tiempo;
                cout << "Intereses ganados = " << interesesGanados << endl << endl;

            }   catch(const std::runtime_error& e){
                cout << e.what() << endl;
            }
            break;
        }

        cout << "¿Desea obtener otro calculo?" << endl;
        cout << "Presione cualquier tecla para realizar otro calculo, presione 's' para salir." << endl;
        cin >> otroCalculo;

    } while(otroCalculo != 's');

};
