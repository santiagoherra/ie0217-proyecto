#include <iostream>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <chrono>
#include <ctime>
#include <fstream>
#include <cstdlib>


#include "BaseDeDatos.hpp"
#include "RegistrosGenerales.hpp"
#include "Operaciones.hpp"
#include "prestamos.hpp"
#include "funcionesGenerales.hpp"
#include "cdp.hpp"
#include "CuentaAhorros.hpp"
#include "menus.hpp"

using namespace std;

enum Opciones{ // Define la enumeracion de las opciones del menu
    INFORMACION = 1,
    ATENCION,
    SALIR
};

int main(){
    ifstream file("banco.db");
    
    if (!file) {
        std::cout << "Creando base de datos..." << std::endl;
        crearDB();
        llenarDB();
    } else {
        std::cout << "Utilizando la base de datos banco.db" << std::endl;
    }
    int opcion; // Switch del menu
    Operaciones op;
    RegistrosGenerales regs;

    // Datos para los registros
    int retorno;
    string tipoTransaccion;
    string fechaTransaccion;
    string denominacion;
    string clienteOrigenCedula; 
    string clienteDestinoCedula; 
    float montoBase;

    do {
        // Imprime el menu
        cout << "\n\nBienvenido a la aplicacion de banco, escoja el modo de uso: \n";
        cout << "1. Modulo de informacion general de prestamos\n";
        cout << "2. Modulo de atencion al cliente \n";
        cout << "3. Salir \n";
        cout << "Ingrese su opcion \n";
        
        // Verificar si la entrada del usuario es válida
        if (!(cin >> opcion)) {
            cout << "\n";
            cout << "Opcion no valida. Por favor, ingrese un numero." << endl;
            cin.clear(); // Limpiar el indicador de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar el resto de la línea
            continue; // Saltar al siguiente ciclo del bucle
        }
        
        switch(opcion) {
            case INFORMACION:
                menu_informacion();
                break;
            case ATENCION:
                menu_atencion();
                break;
            case SALIR:
                cout << "Saliendo del programa... \n";
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo" << endl;
                break;
        }
    } while (opcion != SALIR); // Ciclo mientras opcion sea diferente de salir

    return 0;
}