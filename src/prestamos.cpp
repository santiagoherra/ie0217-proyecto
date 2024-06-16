#include "prestamos.hpp"
#include <cmath>
#include <sqlite3.h>
#include <stdexcept>

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

//Utilizando la bilbioteca cmath para hacer la funcion.
int Prestamos::interesAnualaMensual(int interesAnual){
    int interes_mensual = pow(1 + interesAnual, 1/12) - 1;
    return interes_mensual;
}

//Calcula las cuotas personzalidas para el usuario
std::vector<int> Prestamos::calcularCoutas(int interes, std::vector<int> meses, double monto, double monto_prendario = NULL){

    if(monto_prendario != NULL){

        monto = monto_prendario * 0.8;
    }

    int tem = interesAnualaMensual(interes);
    
    std::vector<int> cuotas_personalizadas;

    //las cuotas se da en un formato de contenedor vector
    for(int mes : meses){

        int cuota_mes = monto * (pow(tem * (1 + tem), mes) / pow(1 + tem, mes) - 1);

        cuotas_personalizadas.push_back(cuota_mes);
    }

    return cuotas_personalizadas;
}

//Funcion para validar un prestamo del usuario y agregar a la base de datos.
bool Prestamos::validacionPrestamo(std::vector<int> meses, std::vector<int> cuotas_dolar, std::vector<int> cuotas_colon, double salario, int tipoMoneda){
    int moneda_prestamo;
    int continuidad_laboral;
    int opcion_cuotas_meses;
    int cuota_validar;

    std::cout << "Usted posee mas de 6 meses de continuidad laboral?\n1) Si\n2) No" << std::endl;
    std::cin >> continuidad_laboral;

    if(continuidad_laboral == 0){
        return false;
    }

    std::cout << "Ahora indique la opcion de plazo de meses y cuotas quiere elegir." << std::endl;
    
    if(moneda_prestamo = 1){
        for(int i = 0; i < 3; i++){
        std::cout << i << ") " << cuotas_dolar[i] << "/" << meses[i] << "\n" << std::endl;
        }
    }else{
        for(int i = 0; i < 3; i++){
        std::cout << "1) " << cuotas_colon[i] << "/" << meses[i] << "\n" << std::endl;
        }
    }

    std::cout << "Ingrese el valor de la opcion que desea escoger?\n1) 2) 3)" << std::endl;
    std::cin >> opcion_cuotas_meses;

    informacionPrestamoNuevo.push_back(meses[opcion_cuotas_meses]);

    if(moneda_prestamo = 1){

        cuota_validar = cuotas_dolar[opcion_cuotas_meses];

        informacionPrestamoNuevo.push_back(cuota_validar);

        if(cuota_validar > (salario*tasaCompraDolarColones*0.7)){
            return false;
        }else{
            return true;
        }
    }else{

        cuota_validar = cuotas_colon[opcion_cuotas_meses];

        informacionPrestamoNuevo.push_back(cuota_validar);

        if(cuota_validar > (salario*0.7)){
            return false;
        }else{
            return true;
        }
    }

}


int Prestamos::agregarPrestamoBaseDatos(){
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

    //agregar el prestamo en la base de datos
    
}


//Esta es la funcion que imprime la tabla personalizada de prestamos para que la persona pueda elegir
void Prestamos::imprimirTablaInformacion(int interesColon, int interesDolar, std::vector<int> cuotas_dolar, std::vector<int> cuotas_colon, std::vector<int> meses){
    int decision;
    bool prestamo_valido;

    std::cout << "La tabla personalizada de datos para el prestamo elegido es la siguiente.\n" << std::endl; 

    std::cout << "|---------------------------------------\n" << std::endl;
    std::cout << "| Intereses anuales | " << meses[0] << " meses | " << meses[1] << " meses | " << meses[2] << " meses | Tipo de Moneda |\n" << std::endl;
    std::cout << "|---------------------------------------\n" << std::endl;
    std::cout << "| " << interesColon*100 << "%" << "₡ | " << cuotas_colon[0] << "₡ | " << cuotas_colon[1] << "₡ | " << cuotas_colon[2] << "₡ | Colones |\n" << std::endl;
    std::cout << "|---------------------------------------\n" << std::endl;
    std::cout << "| " << interesDolar*100 << "%" << " | " << cuotas_dolar[0] << "$ | " << cuotas_dolar[1] << "$ | " << cuotas_dolar[2] << "$ | Dolares |\n" << std::endl;
    std::cout << "|---------------------------------------\n" << std::endl;

    //Decision esta hecha para que la persona elija si quiere elegir un prestamo en este momento.
    std::cout << "Desea optar por un prestamo en este momento?\n1) Si\n2) No " << std::endl;
    std::cin >> decision;

    //Se planteo que el sistema sepa cual prestamos escogio el usuario en base al interes que se le de
    //a la funcion
    if(decision == 1){

        double salario;
        int moneda_prestamo;

        std::cout << "Salario mensual de la persona que optara por el prestamo (valor en colones)\n" << std::endl;
        std::cin >> salario;

        std::cout << "Indique el tipo de moneda en que quiere hacer el prestamo\n1) Dolares\n2) Colones" << std::endl;
        std::cin >> moneda_prestamo;

        informacionPrestamoNuevo.push_back(salario);

        if(moneda_prestamo = 1){
            informacionPrestamoNuevo.push_back("Dolares");
        }else{
            informacionPrestamoNuevo.push_back("Colones");
        }

        prestamo_valido = validacionPrestamo(meses, cuotas_dolar, cuotas_colon, salario, moneda_prestamo);

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
Prestamos::Prestamos(){
    int opcion_prestamo;

    double monto;
    double monto_prendario;

    std::vector<int> cuotas_personalizadas_colon;

    std::vector<int> cuotas_personalizadas_dolar;

    std::cout << "Bienvenido a la seccion de informacion de prestamos\n" << std::endl;

    std::cout << "Elija el tipo de prestamo por el que desearia optar.\n 1) Personal 2) Prendario 3) Hipotecario" << std::endl;
    std::cin >> opcion_prestamo;

    std::cout << "Para continuar porfavor indique la siguiente informacion:\n Monto por el que sea optar (valor en colones):\n" << std::endl;
    std::cin >> monto;

    if(opcion_prestamo == PERSONAL){

        informacionPrestamoNuevo.push_back(monto);

        informacionPrestamoNuevo.push_back("Personal");

        cuotas_personalizadas_dolar = calcularCoutas(interesPersonalAnualDolar, mesesPersonal, monto);

        cuotas_personalizadas_colon = calcularCoutas(interesPersonalAnualColones, mesesPersonal, monto);

        imprimirTablaInformacion(interesPersonalAnualColones,interesPersonalAnualDolar, cuotas_personalizadas_dolar, cuotas_personalizadas_colon, mesesPersonal);

    }else if(opcion_prestamo == PRENDARIO){

        informacionPrestamoNuevo.push_back(monto*0.8);

        informacionPrestamoNuevo.push_back("Prendario");
        
        std::cout << "Indique el monto del objeto que pondra de colateral para el prestamo." << std::endl;
        std::cin >> monto_prendario;

        cuotas_personalizadas_dolar = calcularCoutas(interesPrendarioAnualDolar, mesesPrendario, monto, monto_prendario);

        cuotas_personalizadas_colon = calcularCoutas(interesPrendarioAnualColones, mesesPrendario, monto, monto_prendario);

        imprimirTablaInformacion(interesPrendarioAnualColones, interesPrendarioAnualDolar, cuotas_personalizadas_dolar, cuotas_personalizadas_colon, mesesPrendario);

    }else if(opcion_prestamo == HIPOTECARIO){

        informacionPrestamoNuevo.push_back(monto);

        informacionPrestamoNuevo.push_back("Hipotecario");

        cuotas_personalizadas_dolar = calcularCoutas(interesHipotecarioAnualDolar, mesesHipotecario, monto);

        cuotas_personalizadas_colon = calcularCoutas(interesHipotecarioAnualColones, mesesHipotecario, monto);

        imprimirTablaInformacion(interesHipotecarioAnualColones, interesHipotecarioAnualDolar, cuotas_personalizadas_dolar, cuotas_personalizadas_colon, mesesHipotecario);
        
    }
};

