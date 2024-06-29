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

int menu_atencion();
int menu_informacion();

#endif