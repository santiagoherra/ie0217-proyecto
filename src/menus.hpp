/**
 * @file menus.hpp
 * @version 1.0
 * @date 6/2024
 * @authors JoséIgnacioSáenzDíaz, SantiagoHerraCastro, KevinJiménezAcuña
 * @brief Implementación por menus del programa
*/
#include <iostream>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <chrono>
#include <ctime>
#include <fstream>
#include <cstdlib>


#include "BaseDeDatos.hpp"
#include "RegistrosGenerales.hpp"
#include "Operaciones.hpp"
#include "prestamos.hpp"
#include "funcionesGenerales.hpp"
#include "cdp.hpp"
#include "CuentaAhorros.hpp"

#ifndef MENUS_HPP
#define MENUS_HPP

// Despliegue de menus que define la implementacion del resto de archivos
// Se busca orden y simpleza

/**
* @brief Menu de atencion al cliente
* @return Int que determina el funcionamiento del menú
*/
int menu_atencion();
/**
* @brief Menu de información
* @return Int que determina el funcionamiento del menú
*/
int menu_informacion();
/**
* @brief Menu de préstamos
* @return Int que determina el funcionamiento del menú
*/
int menu_prestamos();
/**
* @brief Menu de operaciones
* @return Int que determina el funcionamiento del menú
*/
int menu_operaciones();
/**
* @brief Menu de CDPs
* @return Int que determina el funcionamiento del menú
*/
int menu_CDP();

#endif