#ifndef FUNCIONESGENERALES_HPP
#define FUNCIONESGENERALES_HPP

#include <iostream>
#include <string>
#include <sqlite3.h>
#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <regex>
#include <cstdlib>

using namespace std;

//Esta funcion es para poder generar un numero de cuenta aleatorio en colones
long long int generarNumeroCuentaColon();

//Esta funcion es para generar un numero de cuenta aleatorio en dolares
long long int generarNumeroCuentaDolar();

//ESta funcion es para agregar el cliente en el sistema
int agregarCliente(const std::string clienteID, std::string nombre, std::string apellido,
                    long int cuentacolones, long int cuentadolares);

//Funcion que revisa si el cliente existe
bool existeCliente(const std::string& clienteID);

std::tm string_a_fecha(const std::string& dateStr);

// Esta funcion se utiliza para obtener la fecha en forma de string
std::string obtenerFechaString();

void leerInt(int num, int op1 = 0, int op2 = 0, int op3 = 0, int op4 = 0, int op5 = 0);

void leerCedula(const std::string cedula);


#endif
