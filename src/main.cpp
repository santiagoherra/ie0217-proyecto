#include <iostream>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <limits>
#include <cctype>


#include "BaseDeDatos.hpp"
#include "RegistrosGenerales.hpp"
#include "Operaciones.hpp"
#include "prestamos.hpp"

using namespace std;

enum Opciones{ // Define la enumeracion de las opciones del menu
    INFORMACION = 1,
    DEPOSITO,
    RETIRO,
    TRANSFERENCIA,
    ABONO, 
    SALIR
};

int main(){
    int opcion; // Switch del menu
    Prestamos prestamo;


    do {
        // Imprime el menu
        cout << "Bienvenido a la aplicacion de banco: \n";
        cout << "1. Mostrar informacion de prestamos \n";
        cout << "2. Realizar deposito \n";
        cout << "3. Realizar retiro \n";
        cout << "4. Realizar una transferencia \n";
        cout << "5. Abonar a un prestamo \n";
        cout << "7. Salir \n";
        cout << "Ingrese su opcion \n";
        
        // Verificar si la entrada del usuario es válida
        if (!(cin >> opcion)) {
            cout << "Opcion no valida. Por favor, ingrese un numero." << endl;
            cin.clear(); // Limpiar el indicador de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar el resto de la línea
            continue; // Saltar al siguiente ciclo del bucle
        }
        
        switch(opcion) {
            // Switch del menu
            case INFORMACION:
    
                break;
            case DEPOSITO:
                 
                break;
            case RETIRO:
                
                break;
            case TRANSFERENCIA:
    
                break;
            case ABONO:
    
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

