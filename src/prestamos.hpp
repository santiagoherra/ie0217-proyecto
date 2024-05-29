#ifndef PRESTAMOS_HPP
#define PRESTAMOS_HPP
#include <iostream>
#include <vector>

class Prestamos{
    public:
        std::vector<int> mesesPersonal = {12, 36, 48};

        std::vector<int> mesesPrendario = {72, 84, 96};

        std::vector<int> mesesHipotecario = {120, 180, 240};

        int interesPersonalAnualColones = 0.2;
        int interesPersonalAnualDolar = 0.16;

        int interesPrendarioAnualColones = 0.155;
        int interesPrendarioAnualDolar = 0.125;

        int interesHipotecarioAnualColones = 0.12;
        int interesHipotecarioAnualDolar = 0.97;

        int tasaCompraDolarColones = 521.5; //Asumiendo que la gente compra dolar para pagar el prestamo.

        std::vector<int> calcularCoutas(double monto, double montoPrendario = NULL);//Funcion para calcular coutas

        void imprimirTablaInformacion(std::vector<int> cuotas); //Funcion para imprimir la tabla de prestamos personalizadas

        void validacionPrestamo(); //Funcion para validar si si se puede dar prestamo


        Prestamos();//Constructor donde se hara toda el menu y se aplicaran los metodos

};

#endif