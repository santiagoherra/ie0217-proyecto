/**
 * @file cdp.hpp
 * @version 1.0
 * @date 6/2024
 * @authors JoséIgnacioSáenzDíaz, SantiagoHerraCastro, KevinJiménezAcuña
 * @brief Funciones asociadas a la posibilidad de añadir CDPs y verificar que el cliente saldo suficiente
*/

#ifndef CDPS_HPP
#define CDPS_HPP
#include <string>
#include <sqlite3.h>

#include <unordered_map>

struct Cuotas{
    float tasaColon;
    float tasaDolar;
};

class CDP{
    public:

        void inicializarDatos();

        void agregarCDP();

        bool verificarSaldoSuficiente(sqlite3* db, const std::string& cedula, int denominacion, double monto);

    private:
        //Se agregara segun la cantidad de dias, el interes que la persona obtendra.
        std::unordered_map<int, Cuotas> informacionIntereses;

};

#endif