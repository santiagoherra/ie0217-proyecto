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
    DEPOSITO = 1,
    RETIRO,
    TRANSFERENCIA,
    ABONO,
    REGISTRO_PERSONAL,
    REGISTRO_GENERAL,
    SOLICITAR_PRESTAMO,
    APERTURA_CDP,
    RETIRO_CDP,
    SALIR
};

int menu_atencion(){
    int opcion; // Switch del menu
    Operaciones op;
    RegistrosGenerales regs;
    Prestamos prestamo;

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
        cout << "\n\nBienvenido a la aplicacion de banco: \n";
        cout << "1. Realizar deposito \n";
        cout << "2. Realizar retiro \n";
        cout << "3. Realizar una transferencia \n";
        cout << "4. Abonar a un prestamo \n";
        cout << "5. Registro personal \n";
        cout << "6. Registro general \n";
        cout << "7. Solicitar prestamo \n";
        cout << "8. Apertura CDP \n";
        cout << "9. Retirar CDP \n";
        cout << "10. Salir \n";
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
            // Switch del menu
            case DEPOSITO:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                retorno = op.deposito(denominacion, clienteOrigenCedula, clienteDestinoCedula, montoBase);

                // Todos los metodos de operaciones devuelven 1 si la transaccion fue exitosa
                //if(retorno) {
                    //tipoTransaccion = "deposito";
                    //fechaTransaccion = obtenerFechaString();  // Fecha en forma de string

                    // Insertar la transaccion al registro general
                    //regs.actualizarRegistro(tipoTransaccion, fechaTransaccion, denominacion,
                                       //clienteOrigenCedula, clienteDestinoCedula, montoBase);
                //}
                break;
            case RETIRO:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                retorno = op.retiro(denominacion, clienteOrigenCedula, clienteDestinoCedula, montoBase);
                if(retorno) {
                    tipoTransaccion = "retiro";
                    fechaTransaccion = obtenerFechaString();  // Fecha en forma de string

                    // Insertar la transaccion al registro general
                    regs.actualizarRegistro(tipoTransaccion, fechaTransaccion, denominacion,
                                       clienteOrigenCedula, clienteDestinoCedula, montoBase);
                }
                break;
            case TRANSFERENCIA:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                retorno = op.transferencias(denominacion, clienteOrigenCedula, clienteDestinoCedula, montoBase);
                    if(retorno) {
                        tipoTransaccion = "transferencia";
                        fechaTransaccion = obtenerFechaString();  // Fecha en forma de string

                        // Insertar la transaccion al registro general
                        regs.actualizarRegistro(tipoTransaccion, fechaTransaccion, denominacion,
                                       clienteOrigenCedula, clienteDestinoCedula, montoBase);
                }
                break;
            case ABONO:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                retorno = op.abonosPrestamos(denominacion, clienteOrigenCedula, clienteDestinoCedula, montoBase);

                    if(retorno) {
                        tipoTransaccion = "abono";
                        fechaTransaccion = obtenerFechaString();  // Fecha en forma de string

                        // Insertar la transaccion al registro general
                        regs.actualizarRegistro(tipoTransaccion, fechaTransaccion, denominacion,
                                       clienteOrigenCedula, clienteDestinoCedula, montoBase);
                }
                
                break;
            case REGISTRO_PERSONAL:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                regs.registroPersonal();
                break;
            case REGISTRO_GENERAL:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                regs.verRegistro();
                break;
            case SOLICITAR_PRESTAMO:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                prestamo.menu();
                prestamo.seguirConPrestamo();
                break;
            case APERTURA_CDP:

                break;
            case RETIRO_CDP:

                break;
            case SALIR:
                cout << "Saliendo del modulo de atencion al cliente \n";
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo" << endl;
                break;
        }
    } while (opcion != SALIR); // Ciclo mientras opcion sea diferente de salir

    return 0;
}

enum Opciones2{ // Define la enumeracion de las opciones del menu
    INFORMACION_PRESTAMOS = 1,
    INFORMACION_CDP,
    SALIR2
};

int menu_informacion(){
    Prestamos prestamo;
    CuentaAhorros cuenta;
    int opcion;
    do {
        // Imprime el menu
        cout << "\nBienvenido al modulo de informacion general, escoja una de las siguientes opciones: \n";
        cout << "1. Informacion de prestamos\n";
        cout << "2. Modulo de cdps \n";
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
            case INFORMACION_PRESTAMOS:
                prestamo.menu();
                break;
            case INFORMACION_CDP:
                cuenta.calculadoraIntereses();
                break;
            case SALIR2:
                cout << "Saliendo del modulo de atencion a clientes \n";
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo" << endl;
                break;
        }
    } while (opcion != SALIR2); // Ciclo mientras opcion sea diferente de salir

    return 0;
}