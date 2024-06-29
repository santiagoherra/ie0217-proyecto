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
    OPERACIONES = 1,
    PRESTAMO,
    CDPS,
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
        cout << "\nEscoja una de las siguientes opciones \n";
        cout << "1. Tramites Generales \n";
        cout << "2. Solicitud y abono de prestamos\n";
        cout << "3. Apertura o retiro de CDPs \n";
        cout << "4. Salir \n";
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

            case OPERACIONES:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                menu_operaciones();
                break;

            case PRESTAMO:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                menu_prestamos();
                break;
                
            case CDPS:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
    CALCULADORA_INTERESES,
    INFORMACION_CDP,
    INFORMACION_CUENTAS,
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
        cout << "2. Calculadora de interes \n";
        cout << "3. Informacion CDP \n";
        cout << "4. Informacion cuentas de ahorro \n";
        cout << "5. Salir \n";
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
            case CALCULADORA_INTERESES:
                cuenta.calculadoraIntereses();
                break;
            case INFORMACION_CDP:
                cuenta.printInfoCDP();
                break;
            case INFORMACION_CUENTAS:
                cuenta.printInfoCuentaAhorroRegular();
            case SALIR2:
                cout << "Saliendo del modulo de informacion \n";
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo" << endl;
                break;
        }
    } while (opcion != SALIR2); // Ciclo mientras opcion sea diferente de salir

    return 0;
}

enum Opciones3{ // Define la enumeracion de las opciones del menu
    SOLICITAR = 1,
    ABONAR,
    IMPRIMIR,
    SALIR_PRESTAMO
};

int menu_prestamos(){
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
        cout << "\nSeleccione la accion que desea realizar\n";
        cout << "1. Solicitar un prestamo\n";
        cout << "2. Abonar a un prestamo \n";
        cout << "3. Imprimir archivo con prestamos\n";
        cout << "4. Salir \n";
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
            case SOLICITAR:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                prestamo.menu();
                prestamo.seguirConPrestamo();
                break;

            case ABONAR:
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

            case IMPRIMIR:
                prestamo.infoPrestamos();
                break;

            case SALIR_PRESTAMO:
                cout << "Saliendo del modulo de prestamos \n";
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo" << endl;
                break;
        }
    } while (opcion != SALIR_PRESTAMO); // Ciclo mientras opcion sea diferente de salir

    return 0;
}


enum Opciones4{ // Define la enumeracion de las opciones del menu
    DEPOSITO = 1,
    RETIRO,
    TRANSFERENCIA,
    REGISTRO_PERSONAL,
    REGISTRO_GENERAL,
    SALIR_OPERACIONES
};

int menu_operaciones(){
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
        cout << "\nEscoja una de las siguientes opciones \n";
        cout << "1. Deposito \n";
        cout << "2. Retiro\n";
        cout << "3. Transferencia \n";
        cout << "4. Registro Personal \n";
        cout << "5. Registro General \n";
        cout << "6. Salir \n";
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

            case REGISTRO_PERSONAL:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                regs.registroPersonal();
                break;
            case REGISTRO_GENERAL:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                regs.verRegistro();
                break;
            case SALIR_OPERACIONES:
                cout << "Saliendo del modulo de operaciones \n";
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo" << endl;
                break;
        }
    } while (opcion != SALIR_OPERACIONES); // Ciclo mientras opcion sea diferente de salir

    return 0;
}

enum Opciones5{ // Define la enumeracion de las opciones del menu
    PEDIR = 1,
    RETIRAR,
    GESTION,
    SALIR_CDP
};

int menu_CDP(){
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
        cout << "\nEscoja una de las siguientes opciones \n";
        cout << "1. Solicitar un CDP \n";
        cout << "2. Retirar un CDP\n";
        cout << "3. Observar informacion asociada a un CDP \n";
        cout << "4. Salir \n";
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
            case PEDIR:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                regs.registroPersonal();
                break;
            case RETIRAR:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                regs.verRegistro();
                break;
            case GESTION:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                op.gestionAhorros();
            case SALIR_CDP:
                cout << "Saliendo del modulo de operaciones \n";
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo" << endl;
                break;
        }
    } while (opcion != SALIR_CDP); // Ciclo mientras opcion sea diferente de salir

    return 0;
}