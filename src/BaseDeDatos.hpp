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


static int callback(void *data, int argc, char**argv, char **azColName);
int crearDB();
int llenarDB();

#endif