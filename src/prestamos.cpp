#include "prestamos.hpp"
#include <cmath>

enum OpcionesPrestamos{
    PERSONAL = 1,
    PRENDARIO,
    HIPOTECARIO,
};

int Prestamos::interesAnualaMensual(int interesAnual){
    int interes_mensual = pow(1 + interesAnual, 1/12) - 1;
    return interes_mensual;
}

std::vector<int> Prestamos::calcularCoutas(int interes, std::vector<int> meses, double monto, double monto_prendario = NULL){

    if(monto_prendario != NULL){

        monto = monto_prendario * 0.8;
    }

    int tem = interesAnualaMensual(interes);
    
    std::vector<int> cuotas_personalizadas;

    for(int mes : meses){

        int cuota_mes = monto * (pow(tem * (1 + tem), mes) / pow(1 + tem, mes) - 1);

        cuotas_personalizadas.push_back(cuota_mes);
    }

    return cuotas_personalizadas;
}

void Prestamos::validacionPrestamo(std::vector<int> cuotas_dolar, std::vector<int> cuotas_colon, std::vector<int> meses){
    int moneda_prestamo;
    double salario;

    std::cout << "Salario mensual de la persona que optara por el prestamo (valor en colones)\n" << std::endl;
    std::cin >> salario;

    std::cout << "Indique el tipo de moneda en la que se hara el prestamo.\n" << std::endl;
    std::cin >> moneda_prestamo; 

}

void Prestamos::imprimirTablaInformacion(int interesColon, int interesDolar, std::vector<int> cuotas_dolar, std::vector<int> cuotas_colon, std::vector<int> meses){
    int desicion;

    std::cout << "La tabla personalizada de datos para el prestamo elegido es la siguiente.\n" << std::endl; 

    std::cout << "|---------------------------------------\n" << std::endl;
    std::cout << "| Intereses anuales | " << meses[0] << " meses | " << meses[1] << " meses | " << meses[2] << " meses | Tipo de Moneda |\n" << std::endl;
    std::cout << "|---------------------------------------\n" << std::endl;
    std::cout << "| " << interesColon << "%" << "₡ | " << cuotas_colon[0] << "₡ | " << cuotas_colon[1] << "₡ | " << cuotas_colon[2] << "₡ | Colones |\n" << std::endl;
    std::cout << "|---------------------------------------\n" << std::endl;
    std::cout << "| " << interesDolar << "%" << " | " << cuotas_dolar[0] << "$ | " << cuotas_dolar[1] << "$ | " << cuotas_dolar[2] << "$ | Dolares |\n" << std::endl;
    std::cout << "|---------------------------------------\n" << std::endl;

    std::cout << "Desea optar por un prestamo en este momento?\n1) Si\n2) No " << std::endl;
    std::cin >> desicion;

    if(desicion == 1){
        validacionPrestamo();
    }

}

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

        cuotas_personalizadas_dolar = calcularCoutas(interesPersonalAnualDolar, mesesPersonal, monto);

        cuotas_personalizadas_colon = calcularCoutas(interesPersonalAnualColones, mesesPersonal, monto);

        imprimirTablaInformacion(interesPersonalAnualColones,interesPersonalAnualDolar, cuotas_personalizadas_dolar, cuotas_personalizadas_colon, mesesPersonal);

    }else if(opcion_prestamo == PRENDARIO){
        
        std::cout << "Indique el monto del objeto que pondra de colateral para el prestamo." << std::endl;
        std::cin >> monto_prendario;

        cuotas_personalizadas_dolar = calcularCoutas(interesPrendarioAnualDolar, mesesPrendario, monto, monto_prendario);

        cuotas_personalizadas_colon = calcularCoutas(interesPrendarioAnualColones, mesesPrendario, monto, monto_prendario);

        imprimirTablaInformacion(interesPrendarioAnualColones, interesPrendarioAnualDolar, cuotas_personalizadas_dolar, cuotas_personalizadas_colon, mesesPrendario);

    }else if(opcion_prestamo == HIPOTECARIO){

        cuotas_personalizadas_dolar = calcularCoutas(interesHipotecarioAnualDolar, mesesHipotecario, monto);

        cuotas_personalizadas_colon = calcularCoutas(interesHipotecarioAnualColones, mesesHipotecario, monto);

        imprimirTablaInformacion(interesHipotecarioAnualColones, interesHipotecarioAnualDolar, cuotas_personalizadas_dolar, cuotas_personalizadas_colon, mesesHipotecario);
        
    }
};

