#include "prestamos.hpp"

enum OpcionesPrestamos{
    PERSONAL = 1,
    PRENDARIO,
    HIPOTECARIO,
};

Prestamos::Prestamos(){
    int opcion_prestamo;

    double monto;
    double salario;
    double monto_prendario;

    std::vector<int> cuotas_personalizadas;

    std::cout << "Bienvenido a la seccion de informacion de prestamos\n" << std::endl;

    std::cout << "Elija el tipo de prestamo por el que desearia optar.\n 1) Personal 2) Prendario 3) Hipotecario" << std::endl;
    std::cin >> opcion_prestamo;

    std::cout << "Para continuar porfavor indique la siguiente informacion:\n Monto por el que sea optar (valor en colones):\n" << std::endl;
    std::cin >> monto;

    std::cout << "Salario mensual de la persona que optara por el prestamo (valor en colones)\n" << std::endl;
    std::cin >> salario;

    if(opcion_prestamo == PERSONAL){

        cuotas_personalizadas = calcularCoutas(monto);
        imprimirTablaInformacion(cuotas_personalizadas);

    }else if(opcion_prestamo == PRENDARIO){
        
        //preguntar por monto del prendario
        cuotas_personalizadas = calcularCoutas(monto, monto_prendario);
        imprimirTablaInformacion(cuotas_personalizadas);

    }else if(opcion_prestamo == HIPOTECARIO){

    }
}