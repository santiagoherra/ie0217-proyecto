#ifndef FUNCIONESGENERALES_HPP
#define FUNCIONESGENERALES_HPP

#include <iostream>
#include <random>
#include <string>
#include <sqlite3.h>
#include <ctime>
#include <sstream>
#include <iomanip>


//Esta funcion es para poder generar un numero de cuenta aleatorio en colones
long long int generarNumeroCuentaColon();

//Esta funcion es para generar un numero de cuenta aleatorio en dolares
long long int generarNumeroCuentaDolar();

//ESta funcion es para agregar el cliente en el sistema
int agregarCliente(const std::string clienteID, std::string nombre, std::string apellido);

//Funcion que revisa si el cliente existe
bool existeCliente(const std::string& clienteID);

std::tm string_a_fecha(const std::string& dateStr);


#endif
