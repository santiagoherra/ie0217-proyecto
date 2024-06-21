#include "Operaciones.hpp"
#include "prestamos.hpp"
#include "funcionesGenerales.hpp"
#include <string>


//Este archivo consisten en las funciones de solicitud de un prestamo, apertura de un cdp, retiro de un cdp
//y gestion de ahorros.

using namespace std;

int Operaciones::solicitudPrestamos(){
    Prestamos prestamo;
    prestamo.menu();
    prestamo.seguirConPrestamo();
}

int Operaciones::gestionAhorros(){
    bool existe_cliente;

    std::string cedula;    

    cout << "Esta es la seccion de gestion de ahorros para los clientes.\n" << endl;

    cout << "Porfavor ingrese su numero de cedula para verificar si la persona esta ingresada en el sistema.\n" << endl; 
    getline(cin, cedula);

    existe_cliente = existeCliente(cedula);

    if(!existe_cliente){
        cout << "La persona de la cedula " << cedula << "no esta ingresada en el sistema" << endl;
        return 0;
    }

    std::cout << "Certificados de Deposito a Plazo asociados a la ID de cliente " << cedula << ".\n" << endl;

    //proceso de obtener los cdps de las personas

    sqlite3 *db;
    int rc;

    rc = sqlite3_open("banco.db", &db);
    if (rc) {
        std::cerr << "No se puede abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    const char* sqlConsulta = "SELECT * FROM certificados_de_deposito WHERE cliente_cedula = ?;";

    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sqlConsulta, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "No se puede preparar la declaración: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_text(stmt, 1, cedula.c_str(), -1, SQLITE_STATIC);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int cdp_id = sqlite3_column_int(stmt, 0);
        const unsigned char *denominacion = sqlite3_column_text(stmt, 1);
        double tasa = sqlite3_column_double(stmt, 2);
        int plazo_meses = sqlite3_column_int(stmt, 3);
        double monto_deposito = sqlite3_column_double(stmt, 4);

        std::cout << "CDP ID: " << cdp_id
                  << ", Denominación: " << denominacion
                  << ", Tasa: " << tasa
                  << ", Plazo (meses): " << plazo_meses
                  << ", Monto Depósito: " << monto_deposito;
    }

    sqlite3_finalize(stmt);


    cout << "En base al numero de ID del Certificado de Deposito a Plazo, elija el numero del CDP ID para poder ver"
            " los progreso generado hasta ahora." << endl;

    
};





