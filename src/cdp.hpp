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