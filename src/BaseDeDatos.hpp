#ifndef BASEDEDATOS_HPP
#define BASEDEDATOS_HPP

using namespace std;


static int callback(void *data, int argc, char**argv, char **azColName);
int crearDB();
int llenarDB();

#endif