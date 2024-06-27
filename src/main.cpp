#include <iostream>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <chrono>
#include <ctime>


#include "BaseDeDatos.hpp"
#include "RegistrosGenerales.hpp"
#include "Operaciones.hpp"
#include "prestamos.hpp"
#include "funcionesGenerales.hpp"

using namespace std;

enum Opciones{ // Define la enumeracion de las opciones del menu
    INFORMACION = 1,
    DEPOSITO,
    RETIRO,
    TRANSFERENCIA,
    ABONO,
    REGISTRO_PERSONAL,
    SALIR
};

int main(){
    crearDB();
    llenarDB();
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
        cout << "\n\nBienvenido a la aplicacion de banco: \n";
        cout << "1. Mostrar informacion de prestamos \n";
        cout << "2. Realizar deposito \n";
        cout << "3. Realizar retiro \n";
        cout << "4. Realizar una transferencia \n";
        cout << "5. Abonar a un prestamo \n";
        cout << "6. Registro personal \n";
        cout << "7. Salir \n";
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
                //if(retorno) {
                    //tipoTransaccion = "retiro";
                    //fechaTransaccion = obtenerFechaString();  // Fecha en forma de string

                    // Insertar la transaccion al registro general
                    //regs.actualizarRegistro(tipoTransaccion, fechaTransaccion, denominacion,
                                       //clienteOrigenCedula, clienteDestinoCedula, montoBase);
                //}
                break;
            case TRANSFERENCIA:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                retorno = op.transferencias(denominacion, clienteOrigenCedula, clienteDestinoCedula, montoBase);
                    //if(retorno) {
                        //tipoTransaccion = "transferencia";
                        //fechaTransaccion = obtenerFechaString();  // Fecha en forma de string

                        // Insertar la transaccion al registro general
                        //regs.actualizarRegistro(tipoTransaccion, fechaTransaccion, denominacion,
                                       //clienteOrigenCedula, clienteDestinoCedula, montoBase);
                //}
                break;
            case ABONO:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                retorno = op.abonosPrestamos(denominacion, clienteOrigenCedula, clienteDestinoCedula, montoBase);

                    //if(retorno) {
                        //tipoTransaccion = "abono";
                        //fechaTransaccion = obtenerFechaString();  // Fecha en forma de string

                        // Insertar la transaccion al registro general
                        //regs.actualizarRegistro(tipoTransaccion, fechaTransaccion, denominacion,
                                       //clienteOrigenCedula, clienteDestinoCedula, montoBase);
                //}
                
                break;
            case REGISTRO_PERSONAL:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                regs.registroPersonal();
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