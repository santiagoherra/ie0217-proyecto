/**
 * @file prestamos.hpp
 * @author Santiago Herra santiago.herra@ucr.ac.cr
 * @brief Este es el archivo header para el despliegue de informacion de los prestamos personalizados
 * ademas posee una implementacion que puede agregar prestamos a usuarios.
 * @version 0.1
 * @date 2024-06-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef PRESTAMOS_HPP
#define PRESTAMOS_HPP
#include <iostream>
#include <vector>
#include <any>

class Prestamos{
    public:
        //Aqui se pondra toda la informacion esencial cuando el cliente sea valido para el prestamo
        std::vector<std::any> informacionPrestamoNuevo;

        //Este es la cantidad de meses que se daran como opcion para los prestamos personales
        std::vector<int> mesesPersonal = {12, 36, 48};

        //ESta es la cantidad de meses que se daran como opcion para los prestamos prendarios
        std::vector<int> mesesPrendario = {72, 84, 96};

        //Esta es la cantidad de meses que se daran como opcion para los prestamos hipotecarios
        std::vector<int> mesesHipotecario = {120, 180, 240};

        //Estos son los intereses anuales para los prestamos personales
        float interesPersonalAnualColones = 0.2;
        float interesPersonalAnualDolar = 0.16;

        //Estos son los intereses anuales para los prestamos prendarios
        float interesPrendarioAnualColones = 0.155;
        float interesPrendarioAnualDolar = 0.125;

        //Estos son los intereses anuales para los prestmos hipotecarios
        float interesHipotecarioAnualColones = 0.12;
        float interesHipotecarioAnualDolar = 0.07;

        float tasaCompraDolarColones = 521.5; //Asumiendo que la gente compra dolar para pagar el prestamo.

        //Funcion que cambia el interes anual a mensual, necesario para dar las cuotas mensuales
        float interesAnualaMensual(float interes);

        //Calcula las cuotas utilizadas para la tabla personalizada
        std::vector<int> calcularCoutas(float interes ,std::vector<int> meses, double monto, double monto_prendario = 0);

        //Imprime la informacion en una tabla para poder desplegar las valores de las cuotas
        void imprimirTablaInformacion(float interesColon, float interesDolar, std::vector<int> cuotas_dolar,std::vector<int> cuotas_colon, std::vector<int> meses ); 

        //Valida el si la persona es apta para el prestamo.
        bool validacionPrestamo(std::vector<int> meses, std::vector<int> cuotas_dolar, std::vector<int> cuotas_colon, double salario, int tipoMoneda); 

        //Funcion para agregar el prestamo a la base de datos.
        int agregarPrestamoBaseDatos();


        Prestamos();//Constructor donde se hara toda el menu y se aplicaran los metodos

};

#endif