/**
 * @file Operaciones.cpp
 * @version 1.0
 * @date 6/2024
 * @authors JoséIgnacioSáenzDíaz, SantiagoHerraCastro, KevinJiménezAcuña
 * @brief Implementación: Retiro, depósito, abono, transferencia y ahorros parte 2.
*/
#include "Operaciones.hpp"
#include "prestamos.hpp"
#include "funcionesGenerales.hpp"
#include <string>
#include <ctime>
#include <iomanip>



//Este archivo consisten en las funciones de solicitud de un prestamo, apertura de un cdp, retiro de un cdp
//y gestion de ahorros.

using namespace std;

int Operaciones::solicitudPrestamos(){
    Prestamos prestamo;
    prestamo.menu();
    prestamo.seguirConPrestamo();

    return 0;
}

int Operaciones::gestionAhorros(){
    bool existe_cliente;

    std::string cedula;    

    cout << "Esta es la seccion de gestion de ahorros para los clientes.\n" << endl;

    //Se ingresa la cedula para ver los CDPs de la persona asociada
    cout << "Porfavor ingrese su numero de cedula para verificar si la persona esta ingresada en el sistema.\n" << endl; 
    getline(cin, cedula);

    leerCedula(cedula);

    existe_cliente = existeCliente(cedula);

    if(!existe_cliente){
        cout << "La persona de la cedula " << cedula << "no esta ingresada en el sistema" << endl;
        return 0;
    }

    std::cout << "Certificados de Deposito a Plazo asociados a la ID de cliente\n" << cedula << ".\n" << endl;

    //Ahora se hara una consuta a la base de datos para obtener los CDPs de la persona de la cedula

    sqlite3 *db;
    int rc;

    //Se abre la base de datos
    rc = sqlite3_open("banco.db", &db);
    if (rc) {
        std::cerr << "No se puede abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    
    const char* sqlConsulta = "SELECT * FROM certificados_de_deposito WHERE cliente_cedula = ?;";

    sqlite3_stmt *stmt;

    //Se prepara la consulta
    rc = sqlite3_prepare_v2(db, sqlConsulta, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "No se puede preparar la declaracion: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    //Se arreglan prints de los numeros para que no salgan en potencia
    cout << std::fixed << std::setprecision(2); 

    //Se inserta la cedula en la consulta 
    sqlite3_bind_text(stmt, 1, cedula.c_str(), -1, SQLITE_STATIC);

    //Con el bucle se obtiene todos los valores de la consulta y se van imprimiendo uno por uno
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int cdp_id = sqlite3_column_int(stmt, 0);
        const unsigned char *denominacion = sqlite3_column_text(stmt, 1);
        double tasa = sqlite3_column_double(stmt, 2);
        int plazo_meses = sqlite3_column_int(stmt, 3);
        double monto_deposito = sqlite3_column_double(stmt, 4);
        const unsigned char *fecha_deposito = sqlite3_column_text(stmt, 5);

        cout << "\nCDP ID: " << cdp_id
                  << "\nDenominacion: " << denominacion
                  << "\nTasa: " << tasa
                  << "\nPlazo (meses): " << plazo_meses
                  << "\nMonto Deposito: " << monto_deposito
                  << "\nFecha de deposito: " << fecha_deposito << endl;
    }

    sqlite3_finalize(stmt);

    //Despues de ver cada uno de los cdps de la perosona, se debe de escoger cual es el que se quiere ver en especifico
    //y la cantidad de plata que lleva hasta el momento, entonces se escoge, despues se busca en la base de datos y se
    //imprime el cdp deseado


    int cdp_elegido;

    cout << "\nEn base al numero de ID del Certificado de Deposito a Plazo, elija el numero del CDP ID para poder ver"
            "los progreso generado hasta ahora.\n" << endl;

    cin >> cdp_elegido; //Es el id del cdp

    leerInt(cdp_elegido);

    const char* recuperarCDPsql = "SELECT cdp_id, denominacion, tasa, plazo_meses, monto_deposito"
                                ", fecha_deposito FROM certificados_de_deposito WHERE cdp_id = ?;";


    rc = sqlite3_prepare_v2(db, recuperarCDPsql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "No se puede preparar la declaracion: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_bind_int(stmt, 1, cdp_elegido);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int cdp_id = sqlite3_column_int(stmt, 0);
        const unsigned char* denominacion = sqlite3_column_text(stmt, 1);
        float tasa = sqlite3_column_double(stmt, 2);
        int plazo = sqlite3_column_int(stmt, 3);
        double monto_deposito = sqlite3_column_double(stmt, 4);
        const unsigned char* fecha_deposito = sqlite3_column_text(stmt, 5);

        // Guardar los valores en variables locales
        std::string denominacion_str = reinterpret_cast<const char*>(denominacion);
        std::string fecha_deposito_str = reinterpret_cast<const char*>(fecha_deposito);

        cout << "+---------------------------------+\n" << endl;
        cout << "|       CDP ID      |    " << cdp_id << "\n" << endl;
        cout << "+---------------------------------+\n" << endl;
        cout << "|    Denominacion   |   " << denominacion_str << "\n" << endl;
        cout << "+---------------------------------+\n" << endl;
        cout << "|        Tasa       |    " << tasa << "\n" << endl;
        cout << "+---------------------------------+\n" << endl;
        cout << "|       Plazo       |    " << plazo << "\n" <<  endl;
        cout << "+---------------------------------+\n" << endl;
        cout << "|       Monto       |   " << monto_deposito << "\n" << endl;
        cout << "+---------------------------------+\n" << endl;
        cout << "| Fecha de Deposito |   " << fecha_deposito_str << "\n" << endl; 
        cout << "+---------------------------------+\n" << endl;

        std::tm tm_fecha_deposito = string_a_fecha(fecha_deposito_str);

        //se consigue la fecha de hoy
        std::time_t t = std::time(nullptr);
        std::tm tm_actual = *std::localtime(&t);

        // Convertir tm a time_t para calcular la diferencia
        std::time_t tiempo_deposito = std::mktime(&tm_fecha_deposito);
        std::time_t tiempo_actual = std::mktime(&tm_actual);

        //me da la diferencia de tiempo en segundos y se pasa a dias
        std::time_t diferencia_tiempo = difftime(tiempo_actual, tiempo_deposito) / (60 * 60 * 24);

        //asi se obtiene el interes acumulados hasta el momento
        float interes_acumulados = tasa * diferencia_tiempo * monto_deposito;
        cout << "\n\n+------------------------------------+\n" << endl;
        cout << "| Intereses hasta ahora   | " << interes_acumulados << " |\n" << endl;
        cout << "+------------------------------------+\n" << endl;

    //Del lado contrario el usuario no escoge el CDP ID deseado no se encontrara el CDP designado
    }else{
        cout << "No se encontro ningun Certificado de Deposito a plazo con ese ID.\n" << endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);


    return 0;    
};





