/**
 * @file prestamos.hpp
 * @version 1.0
 * @date 6/2024
 * @authors JoséIgnacioSáenzDíaz, SantiagoHerraCastro, KevinJiménezAcuña
 * @brief Clase préstamo y métodos para añadir, calcular tasas, desplegar info y validarlos
*/
#ifndef PRESTAMOS_HPP
#define PRESTAMOS_HPP
#include <iostream>
#include <vector>
#include <any>

/**
 * @brief Esta es la clase de prestamos la cual amneja el ingreso de prestamos del usuario e 
 * imprime la informacion personalizada de las tasas de interes para los montos personalizados 
 * que ingresa el usuario.
 * 
 */
class Prestamos{
    public:
        /**
         * @brief Este es la cantidad de meses que se daran como opcion para los prestamos personales
         */
        std::vector<int> mesesPersonal = {12, 36, 48};

        /**
         * @brief Esta es la cantidad de meses que se daran como opcion para los prestamos prendario
         * 
         */
        std::vector<int> mesesPrendario = {72, 84, 96};

        /**
         * @brief Esta es la cantidad de meses que se daran como opcion para los prestamos hipotecarios
         * 
         */
        std::vector<int> mesesHipotecario = {120, 180, 240};

        /**
         * @brief Estos son los intereses anuales para los prestamos personales
         * 
         */
        float interesPersonalAnualColones = 0.2;
        float interesPersonalAnualDolar = 0.16;

        /**
         * @brief Estos son los intereses anuales para los prestamos prendarios
         * 
         */
        float interesPrendarioAnualColones = 0.155;
        float interesPrendarioAnualDolar = 0.125;

        /**
         * @brief Estos son los intereses anuales para los prestmos hipotecarios
         * 
         */
        float interesHipotecarioAnualColones = 0.12;
        float interesHipotecarioAnualDolar = 0.07;

        /**
         * @brief Asumiendo que la gente compra dolar para pagar el prestamo.
         * 
         */
        float tasaCompraDolarColones = 521.5; 

        /**
         * @brief Funcion que cambia el interes anual a mensual, necesario para dar las cuotas mensuales
         * 
         * @param interes interes anual
         * @return float Devuelve el interes mensual
         */
        float interesAnualaMensual(float interes);

        /**
         * @brief Calcula las cuotas utilizadas para la tabla personalizada
         * 
         * @param interes Manda el interes mensual utilizado
         * @param meses Manda las opciones de meses que se puede utilizar
         * @param monto Manda el monto total que la persona eligio
         * @return std::vector<int> Calculas las cuotas mensual para los meses que dan de opcion
         */
        std::vector<int> calcularCoutas(float interes ,std::vector<int> meses, double monto);

        /**
         * @brief Imprime la tabla de opciones personalizadas del prestamo que la persona eligio
         * 
         * @param interesColon Interes el prestamo elegido en colones
         * @param interesDolar Interes del prestamo elegido en dolares
         * @param cuotas_dolar cuotas en dolares dependiendo del mes
         * @param cuotas_colon cuotas en colones dependiendo del mes
         * @param meses meses de opciones para el usuario
         */
        void imprimirTablaInformacion(float interesColon, float interesDolar, std::vector<int> cuotas_dolar,
                                     std::vector<int> cuotas_colon, std::vector<int> meses); 

        /**
         * @brief Valida el si la persona es apta para el prestamo.
         * 
         * @param salario Salario de la persona que desea optar por el prestamo
         * @param tipoMoneda Tipo de moneda para el prestamo
         * @return true Si la persona es apta para el prestamo
         * @return false Si la persona no es apta para el prestamo
         */
        bool validacionPrestamo(double salario, int tipoMoneda); 

        /**
         * @brief Funcion para agregar el prestamo a la base de datos.
         * 
         * @return int devuelve 1 o 0 dependiendo si se logro agregar el prestamo
         */
        int agregarPrestamoBaseDatos();

        /**
         * @brief Funcion para seguir con el prestamo despues de que la persona ve la tabla de informacion.
         * 
         */
        void seguirConPrestamo();

        /**
         * @brief Funcion que comienza para dar paso hacia la la tabla personalizada del usuario
         * 
         */
        void menu();

        /**
         * @brief Funcion para imprimir en un archivo de texto la informacion asociada a los prestamos de un cliente
         * 
         * @return int 
         */
        int infoPrestamos();

    /**
     * @brief Informacion del cliente para despues agregar el prestamo en la base de datos 
     * 
     */
    private:
        std::string denominacion_agregar;
        std::string tipo_agregar;
        double monto_agregar;
        int plazo_meses_agregar;
        int cuotas_agregar;
        std::string cliente_id;
        std::string cedula_agregar;
        float tasa_agregar;
        //Estos son las cuotas y meses personalizados que se ocupa para agregar el prestamo dependiendo
        //de la informacion que elija el cliente
        std::vector<int> cuotas_dolar_cliente;
        std::vector<int> cuotas_colon_cliente;
        std::vector<int> meses_cliente;
};

#endif