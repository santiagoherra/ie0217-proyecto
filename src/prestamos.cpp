#include "prestamos.hpp"
#include "funcionesGenerales.hpp"
#include <cmath>
#include <sqlite3.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>


enum OpcionesPrestamos{
    PERSONAL = 1,
    PRENDARIO,
    HIPOTECARIO,
};

static int callback(void *data, int argc, char**argv, char **azColName){
    for(int i = 0; i < argc; i++){
        std::cout << azColName[i] << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    return 0;
}

void executeSQL(sqlite3 *db, const char *sql, int (*callback)(void*,int,char**,char**), void *data) {
    char *errMsg = 0;
    int rc = sqlite3_exec(db, sql, callback, data, &errMsg);
    if(rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Operacion realizada con éxito" << std::endl;
    }
}

static int getLastPrestamoId(void *data, int argc, char **argv, char **azColName) {
    int *lastPrestamoId = (int*)data;
    if (argc > 0 && argv[0]) {
        *lastPrestamoId = std::stoi(argv[0]);
    } else {
        *lastPrestamoId = 0;  // Si no hay filas, iniciar con 0
    }
    return 0;
}

//Utilizando la bilbioteca cmath para hacer la funcion.
float Prestamos::interesAnualaMensual(float interesAnual){
    float interes_mensual = pow(1 + interesAnual, 1.0/12.0) - 1;

    return interes_mensual;
}

//Calcula las cuotas personzalidas para el usuario
std::vector<int> Prestamos::calcularCoutas(float interes, std::vector<int> meses, double monto){

    float tem = interesAnualaMensual(interes);
    
    std::vector<int> cuotas_personalizadas;

    //las cuotas se da en un formato de contenedor vector
    for(int mes : meses){

        float cuota_mes = (monto * tem * pow(1 + tem, mes)) / (pow(1 + tem, mes) - 1);



        cuotas_personalizadas.push_back(cuota_mes);
    }

    return cuotas_personalizadas;
}

//Funcion para validar un prestamo del usuario y agregar a la base de datos.
bool Prestamos::validacionPrestamo(double salario, int tipoMoneda){
    int continuidad_laboral;
    int opcion_cuotas_meses;
    int cuota_validar;

    std::cout << "Usted posee mas de 6 meses de continuidad laboral?\n1) Si\n2) No" << std::endl;
    std::cin >> continuidad_laboral;

    leerInt(continuidad_laboral);  

    if(continuidad_laboral == 0){
        return false;
    }

    std::cout << "Ahora indique la opcion de plazo de meses y cuotas quiere elegir." << std::endl;
    
    if(tipoMoneda == 1){
        for(int i = 0; i < 3; i++){
        std::cout << i+1 << ") " << cuotas_dolar_cliente[i] << "$ /" << meses_cliente[i] << " meses\n" << std::endl;
        }
    }else{
        for(int i = 0; i < 3; i++){
        std::cout << i+1 << ") " << cuotas_colon_cliente[i] << " /" << meses_cliente[i] << " meses\n" << std::endl;
        }
    }

    std::cout << "Ingrese el valor de la opcion que desea escoger?\n1) 2) 3)" << std::endl;
    std::cin >> opcion_cuotas_meses;

    leerInt(opcion_cuotas_meses);

    //Se agrega el plazo de meses que el cliente elijio
    plazo_meses_agregar = meses_cliente[opcion_cuotas_meses-1];

    //Despues se escoge la cuota dependiendo de si es en colones o dolares y la escogencia del cliente
    //Opcion si es en colones
    if(tipoMoneda == 2){

        cuota_validar = cuotas_colon_cliente[opcion_cuotas_meses-1];

        cuotas_agregar = cuota_validar;

        if(cuota_validar > (salario*0.7)){
            return false;
        }else{
            return true;
        }
    }else{//Opcion si es en dolares

        cuota_validar = cuotas_dolar_cliente[opcion_cuotas_meses-1];

        cuotas_agregar = cuota_validar;

        if(cuota_validar > ((salario*0.7)/tasaCompraDolarColones)){
            return false;
        }else{
            return true;
        }
    }

}


int Prestamos::agregarPrestamoBaseDatos(){
    std::string cedula;

    sqlite3 *db;
    char *errMsg = 0;
    int rc;
    const char* data = "Callback function called";

    rc = sqlite3_open("banco.db", &db);
    if(rc){
        std::cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        return (0);
    } else {
        std::cout << "Ingreso correcto a la base de datos" << std::endl;
    }

    std::cin.ignore();
    std::cout << "Porfavor ingrese su numero de cedula a la cual quiere asociar el prestamo.\n" << std::endl;
    getline(std::cin, cedula);

    leerCedula(cedula);

    cedula_agregar = cedula;

    int lastPrestamoId = 0;
    const char* sqlGetLastPrestamoId = "SELECT MAX(prestamo_id) FROM prestamos;";
    executeSQL(db, sqlGetLastPrestamoId, getLastPrestamoId, &lastPrestamoId);

    int nuevoIdPrestamo = lastPrestamoId + 1;


    // Insertar un nuevo préstamo y asociarlo a un cliente existente
    std::ostringstream oss;
    oss << "INSERT INTO prestamos (prestamo_id, denominacion, tipo, monto_total, plazo_meses, cuota_mensual, tasa, cliente_id) "
    << "VALUES ("
    << nuevoIdPrestamo << ", " // Asume que nuevoIdPrestamo es un número
    << "'" << denominacion_agregar << "', " // Asume que denominacion_agregar es un string
    << "'" << tipo_agregar << "', " // Asume que tipo_agregar es un string
    << std::fixed << std::setprecision(2) << monto_agregar << ", " // Asume que monto_agregar es un número
    << plazo_meses_agregar << ", " // Asume que plazo_meses_agregar es un número
    << std::fixed << std::setprecision(2) << cuotas_agregar << ", " // Asume que cuotas_agregar es un número
    << std::fixed << std::setprecision(3) << tasa_agregar << ", " 
    << "'" << cedula_agregar << "');"; // Asume que cedula_agregar es un string

    std::string sqlInsertPrestamo = oss.str();

    executeSQL(db, sqlInsertPrestamo.c_str(), nullptr, nullptr);

    // Cerrar la base de datos

    sqlite3_close(db);
    return 0;
    
}


//Esta es la funcion que imprime la tabla personalizada de prestamos para que la persona pueda elegir
void Prestamos::imprimirTablaInformacion(float interesColon, float interesDolar, std::vector<int> cuotas_dolar,
                                         std::vector<int> cuotas_colon, std::vector<int> meses){

    //imprime la informacion personalizada
    if(tipo_agregar == "Prendario"){
        std::cout << "\nComo el tipo de prestamo deseado es de tipo prendario el banco le ofreceria "
                    "ofrecer un monto de prestamo de " << monto_agregar << " colones, que equivale a "
                    "el 80% del monto del objeto colateral." << std::endl;
    }

    std::cout << "\nLa tabla personalizada de datos para el prestamo elegido es la siguiente.\n" << std::endl; 

    std::cout << "|-------------------------------------------------------------------------\n" << std::endl;
    std::cout << "| Intereses anuales | " << meses[0] << " meses | " << meses[1] << " meses | " << meses[2] << " meses | Tipo de Moneda |\n" << std::endl;
    std::cout << "|-------------------------------------------------------------------------\n" << std::endl;
    std::cout << "|         " << interesColon*100 << "%" << "      |   " << cuotas_colon[0] << "    |   " << cuotas_colon[1] << "    |     " << cuotas_colon[2] << "       | Colones |\n" << std::endl;
    std::cout << "|-------------------------------------------------------------------------\n" << std::endl;
    std::cout << "|         " << interesDolar*100 << "%" << "      |       " << cuotas_dolar[0] << "$     |      " << cuotas_dolar[1] << "$    |    " << cuotas_dolar[2] << "$   | Dolares |\n" << std::endl;
    std::cout << "|-------------------------------------------------------------------------\n" << std::endl;
    
}

//Esta funcion esta hecha para cuando el cliente quiere seguir obteniendo el prestamo
void Prestamos::seguirConPrestamo(){

    int decision;
    bool prestamo_valido;

    //Decision esta hecha para que la persona elija si quiere elegir un prestamo en este momento.
    std::cout << "Desea optar por un prestamo en este momento?\n1) Si\n2) No " << std::endl;
    std::cin >> decision;

    leerInt(decision);

    //Se planteo que el sistema sepa cual prestamos escogio el usuario en base al interes que se le de
    //a la funcion
    if(decision == 1){

        double salario;
        int moneda_prestamo;

        std::cout << "Salario mensual de la persona que optara por el prestamo (valor en colones)\n" << std::endl;
        std::cin >> salario;
        std::cin.ignore();

        leerInt(salario);

        std::cout << "Indique el tipo de moneda en que quiere hacer el prestamo\n1) Dolares\n2) Colones" << std::endl;
        std::cin >> moneda_prestamo;

        leerInt(moneda_prestamo);

        //condiciones para obtener datos para agregar el prestamo

        if(moneda_prestamo == 1){
            denominacion_agregar = "Dolares";

            //ajustando el monto a dolares
            monto_agregar = monto_agregar / tasaCompraDolarColones;

            if (tipo_agregar == "Personal"){

                tasa_agregar = interesPersonalAnualDolar;

            } else if (tipo_agregar == "Prendario") {

                tasa_agregar = interesPrendarioAnualDolar;

            } else if (tipo_agregar == "Hipotecario") {

                tasa_agregar = interesHipotecarioAnualDolar;
            }

        }else{
            denominacion_agregar = "Colones";

            if (tipo_agregar == "Personal"){

                tasa_agregar = interesPersonalAnualColones;

            } else if (tipo_agregar == "Prendario") {

                tasa_agregar = interesPrendarioAnualColones;

            } else if (tipo_agregar == "Hipotecario") {

                tasa_agregar = interesHipotecarioAnualColones;
            }
        }

        prestamo_valido = validacionPrestamo(salario, moneda_prestamo);

        if(prestamo_valido){

            agregarPrestamoBaseDatos();

            std::cout << "Felicidades! Usted ha sido apto para el prestamo deseado." << std::endl;
        }else{
            std::cout << "Se han realizo la valoracion para su prestamo y lamentamos "
            "informarlo que usted no fue apto para el prestamo\nQue tenga un buen dia." << std::endl;
        }
    }

}

//El constructor de prestamos sera utilizado para implementar la gestion de informacion del para el usuario.
void Prestamos::menu(){

    //Esta son variables locales necesarias para elegir opciones y guardar imformacion importante
    int opcion_prestamo;

    bool validarInt;

    double monto;
    double monto_prendario;

    std::vector<int> cuotas_personalizadas_colon;

    std::vector<int> cuotas_personalizadas_dolar;

    std::cout << "Bienvenido a la seccion de informacion de prestamos\n" << std::endl;

    std::cout << "Elija el tipo de prestamo por el que desearia optar.\n 1) Personal 2) Prendario 3) Hipotecario" << std::endl;
    std::cin >> opcion_prestamo;
    
    leerInt(opcion_prestamo);

    std::cout << "Para continuar porfavor indique la siguiente informacion:\nMonto por el que sea optar (valor en colones):\n" << std::endl;
    std::cin >> monto;

    leerInt(monto);

    //En cada condicion, se guarda informacion en que sera necesaria para agregar el prestamo del usuario
    //y se modifica la informacion para calcular la cuotas personalizadas
    if(opcion_prestamo == PERSONAL){

        //se guarda el monto en colon, si el cliente desea que sea en dolares se cambia el monto total
        monto_agregar = monto;

        tipo_agregar = "Personal";

        double monto_dolar = monto / tasaCompraDolarColones;

        cuotas_personalizadas_dolar = calcularCoutas(interesPersonalAnualDolar, mesesPersonal, monto_dolar);

        cuotas_personalizadas_colon = calcularCoutas(interesPersonalAnualColones, mesesPersonal, monto);

        cuotas_colon_cliente = cuotas_personalizadas_colon;

        cuotas_dolar_cliente = cuotas_personalizadas_dolar;

        meses_cliente = mesesPersonal;

        imprimirTablaInformacion(interesPersonalAnualColones,interesPersonalAnualDolar, cuotas_personalizadas_dolar, cuotas_personalizadas_colon, mesesPersonal);

    }else if(opcion_prestamo == PRENDARIO){

        tipo_agregar = "Prendario";
        
        std::cout << "Indique el monto del objeto que pondra de colateral para el prestamo." << std::endl;
        std::cin >> monto_prendario;

        //Se debe de hacer un cambio para el monto prendario porque el banco solo ofrece el 80% del monto prendario.
        double monto_prendario_banco_dolar = monto_prendario*0.8 / tasaCompraDolarColones;

        double monto_prendario_banco_colon = monto_prendario * 0.8;

        //Se agrega el monto en colones
        monto_agregar = monto_prendario_banco_colon;

        cuotas_personalizadas_dolar = calcularCoutas(interesPrendarioAnualDolar, mesesPrendario, monto_prendario_banco_dolar);

        cuotas_personalizadas_colon = calcularCoutas(interesPrendarioAnualColones, mesesPrendario, monto_prendario_banco_colon);

        cuotas_colon_cliente = cuotas_personalizadas_colon;

        cuotas_dolar_cliente = cuotas_personalizadas_dolar;

        meses_cliente = mesesPrendario;

        imprimirTablaInformacion(interesPrendarioAnualColones, interesPrendarioAnualDolar, cuotas_personalizadas_dolar, cuotas_personalizadas_colon, mesesPrendario);

    }else if(opcion_prestamo == HIPOTECARIO){

        //Se agrega el monto en colones
        monto_agregar = monto;

        tipo_agregar = "Hipotecario";

        double monto_dolar = monto / tasaCompraDolarColones;

        cuotas_personalizadas_dolar = calcularCoutas(interesHipotecarioAnualDolar, mesesHipotecario, monto_dolar);

        cuotas_personalizadas_colon = calcularCoutas(interesHipotecarioAnualColones, mesesHipotecario, monto);

        cuotas_colon_cliente = cuotas_personalizadas_colon;

        cuotas_dolar_cliente = cuotas_personalizadas_dolar;

        meses_cliente = mesesHipotecario;

        imprimirTablaInformacion(interesHipotecarioAnualColones, interesHipotecarioAnualDolar, cuotas_personalizadas_dolar, cuotas_personalizadas_colon, mesesHipotecario);
        
    }
};

