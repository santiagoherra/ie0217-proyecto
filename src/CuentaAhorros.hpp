/**
 * @file CuentaAhorros.hpp
 * @version 1.0
 * @date 6/2024
 * @authors JoséIgnacioSáenzDíaz, SantiagoHerraCastro, KevinJiménezAcuña
 * @brief Métodos para el despliegue de información de cuentas y CDPs
*/

#ifndef CUENTA_DE_AHORROS_HPP
#define CUENTA_DE_AHORROS_HPP
#include <string>


/*Esta clase contiene los métodos necesarios para mostrar toda la
información necesaria acerca de las cuentas de ahorros y los certificados
de pago a plazos; para los certificados de pago a plazos también se cuenta
con un método que calcula los intereses ganados de acuerdo al monto, tiempo
y tipo de moneda con el que se realice*/
class CuentaAhorros {
    public:
        // Este metodo imprime la informacion referente a una cuenta de ahorro regular
        void printInfoCuentaAhorroRegular() const;

        // Este metodo imprime la informacion detallada acerca de los CDPS
        void printInfoCDP() const;

        // Este metodo permite calcular los intereses ganados para un CDP
        void calculadoraIntereses() const;
};

#endif