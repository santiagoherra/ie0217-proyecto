#ifndef PRESTAMOS_HPP
#define PRESTAMOS_HPP
#include <iostream>

class Prestamos{
    public:
        double monto;
        double salario;
        double montoPrendario;

        int interesPersonalAnualColones = 0.2;
        int interesPersonalAnualDolar = 0.16;

        int interesPrendarioAnualColones = 0.155;
        int interesPrendarioAnualDolar = 0.125;

        int interesHipotecarioAnualColones = 0.12;
        int interesHipotecarioAnualDolar = 0.97;

        int tasaCompraDolarColones = 521.5; //Asumiendo que la gente compra dolar para pagar el prestamo.

        double calcularCoutas();//Funcion para calcular coutas

        void imprimirTablaInformacion(); //Funcion para imprimir la tabla de prestamos personalizadas

        void validacionPrestamo(); //Funcion para validar si si se puede dar prestamo


        Prestamos();//Constructor donde se hara toda el menu y se aplicaran los metodos

};

#endif