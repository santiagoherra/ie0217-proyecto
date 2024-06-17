#ifndef CDPS_HPP
#define CDPS_HPP

#include <unordered_map>

struct Cuotas{
    int tasaDolar;
    int tasaColon;
};

class CDP{
    public:
        
        double monto;
        int plazoTiempo;
        int interes;

        void inicializarDatos();

        int aperturaCDP();

        int retiroCDP();

        int calculoIntereses();

        void interesAcumulados();

    private:
        //Se agregara segun la cantidad de dias, el interes que la persona obtendra.
        std::unordered_map<int, Cuotas> informacionIntereses;

};

#endif