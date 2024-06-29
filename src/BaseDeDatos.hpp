/**
 * @file BaseDeDatos.hpp
 * @version 1.0
 * @date 6/2024
 * @authors JoséIgnacioSáenzDíaz, SantiagoHerraCastro, KevinJiménezAcuña
 * @brief Crea e inserta información a la base de datos
*/
#ifndef BASEDEDATOS_HPP
#define BASEDEDATOS_HPP

using namespace std;

/**
* @brief Obtiene indicadores de error de sqlite
* @return int 0 si no hay error, 1 si lo hubo.
*/
static int callback(void *data, int argc, char**argv, char **azColName);
/**
* @brief Crea la base de datos con las tablas respectivas
* @return Int que indica la existencia de errores
*/
int crearDB();
/**
* @brief Llena la base con datos iniciales
* @return Int que indica la existencia de errores
*/
int llenarDB();

#endif