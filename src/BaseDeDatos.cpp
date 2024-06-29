#include <iostream>
#include <sqlite3.h>
#include "BaseDeDatos.hpp"

using namespace std;

static int callback(void *data, int argc, char**argv, char **azColName){
    for(int i = 0; i < argc; i++){
        cout << azColName[i] << (argv[i] ? argv[i] : "NULL") << endl;
    }
    return 0;
}

int crearDB(){
    sqlite3 *db;
    char *errMsg = 0;
    int rc;
    const char* data = "Callback function called";
    const char *sql = "CREATE TABLE cuentas ("
        "numero_cuenta INT PRIMARY KEY,"
        "balance DECIMAL(11, 2),"
        "tasa DECIMAL(2, 2),"
        "denominacion VARCHAR(10));"
        
        "CREATE TABLE clientes ("
        "cedula CHAR(9) PRIMARY KEY,"
        "nombre VARCHAR(50),"
        "apellido VARCHAR(50),"
        "cuenta_colones INT,"
        "cuenta_dolares INT,"
        "CHECK (cedula NOT LIKE '%[^0-9]%'),"
        "FOREIGN KEY (cuenta_colones) REFERENCES cuentas(numero_cuenta),"
        "FOREIGN KEY (cuenta_dolares) REFERENCES cuentas(numero_cuenta));"
        
        "CREATE TABLE prestamos ("
        "prestamo_id INT PRIMARY KEY,"
        "denominacion VARCHAR(8),"
        "tipo VARCHAR(10),"
        "monto_total DECIMAL(11, 2),"
        "plazo_meses INT,"
        "plazo_restante INT,"
        "cuota_mensual INT,"
        "tasa DECIMAL(2, 2),"
        "cliente_id CHAR(9),"
        "FOREIGN KEY (cliente_id) REFERENCES clientes(cedula));"
        
        "CREATE TABLE certificados_de_deposito ("
        "cdp_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "denominacion VARCHAR(8),"
        "tasa DECIMAL(2, 2),"
        "plazo_meses INT,"
        "monto_deposito DECIMAL(11, 2),"
        "fecha_deposito DATE," 
        "cliente_cedula CHAR(9),"
        "FOREIGN KEY (cliente_cedula) REFERENCES clientes(cedula));"
        
        "CREATE TABLE registros ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "tipo_transaccion VARCHAR(50),"
        "fecha_transaccion DATE,"
        "denominacion VARCHAR(10),"
        "cliente_origen_cedula CHAR(9),"
        "cliente_destino_cedula CHAR(9),"
        "monto_base DECIMAL(11, 2),"
        "FOREIGN KEY (cliente_origen_cedula) REFERENCES clientes(cedula),"
        "FOREIGN KEY (cliente_destino_cedula) REFERENCES clientes(cedula));";
    rc = sqlite3_open("banco.db", &db);
    if(rc){
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return (0);
    } else {
        cout << "Ingreso correcto a la base de datos" << endl;
    }

    

    rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
    if (rc != SQLITE_OK){
        cerr << "Error de SQL: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Tablas creadas de forma correcta" << endl;
    }
    
    sqlite3_close(db);
    return 0;
}

int llenarDB(){
    sqlite3 *db;
    char *errMsg = 0;
    int rc;
    const char* data = "Callback function called";

    rc = sqlite3_open("banco.db", &db);
    if(rc){
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return (0);
    } else {
        cout << "Ingreso correcto a la base de datos" << endl;
    }

    const char *inserts_sql = "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (1891435200, 10000.99, 0.01, 'colones');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (2891435200, 85426636.52, 0.01, 'dolares');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (1949435235, 92739879.58, 0.01, 'colones');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (2949435235, 21569505.56, 0.01, 'dolares');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (1732194468, 74430870.91, 0.01, 'colones');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (2732194468, 6279783.57, 0.01, 'dolares');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (1228849575, 70876106.31, 0.01, 'colones');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (2228849575, 24007097.88, 0.01, 'dolares');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (1813166556, 45342690.52, 0.01, 'colones');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (2813166556, 3397091.51, 0.01, 'dolares');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (1326823600, 60747940.15, 0.01, 'colones');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (2326823600, 57409544.21, 0.01, 'dolares');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (1114819158, 39301111.41, 0.01, 'colones');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (2114819158, 44094938.6, 0.01, 'dolares');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (1456251480, 85927372.65, 0.01, 'colones');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (2456251480, 48028733.88, 0.01, 'dolares');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (1409975644, 19080612.1, 0.01, 'colones');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (2409975644, 66519468.19, 0.01, 'dolares');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (1544018749, 97889957.9, 0.01, 'colones');"
        "INSERT INTO cuentas (numero_cuenta, balance, tasa, denominacion)"
        "VALUES (2544018749, 47827464.59, 0.01, 'dolares');"
        "INSERT INTO clientes (cedula, nombre, apellido, cuenta_colones, cuenta_dolares)" 
        "VALUES (891435200, 'Richard', 'Hatcher', 1891435200, 2891435200);"
        "INSERT INTO clientes (cedula, nombre, apellido, cuenta_colones, cuenta_dolares)" 
        "VALUES (949435235, 'Doris', 'Tucker', 1949435235, 2949435235);"
        "INSERT INTO clientes (cedula, nombre, apellido, cuenta_colones, cuenta_dolares)" 
        "VALUES (732194468, 'Mildred', 'Apple', 1732194468, 2732194468);"
        "INSERT INTO clientes (cedula, nombre, apellido, cuenta_colones, cuenta_dolares)" 
        "VALUES (228849575, 'Michael', 'Pena', 1228849575, 2228849575);"
        "INSERT INTO clientes (cedula, nombre, apellido, cuenta_colones, cuenta_dolares)" 
        "VALUES (813166556, 'Christopher', 'Lambert', 1813166556, 2813166556);"
        "INSERT INTO clientes (cedula, nombre, apellido, cuenta_colones, cuenta_dolares)" 
        "VALUES (326823600, 'Effie', 'Gibney', 1326823600, 2326823600);"
        "INSERT INTO clientes (cedula, nombre, apellido, cuenta_colones, cuenta_dolares)" 
        "VALUES (114819158, 'Jennifer', 'Vargas', 1114819158, 2114819158);"
        "INSERT INTO clientes (cedula, nombre, apellido, cuenta_colones, cuenta_dolares)" 
        "VALUES (456251480, 'Nathan', 'Parks', 1456251480, 2456251480);"
        "INSERT INTO clientes (cedula, nombre, apellido, cuenta_colones, cuenta_dolares)" 
        "VALUES (409975644, 'Edward', 'Kruger', 1409975644, 2409975644);"
        "INSERT INTO clientes (cedula, nombre, apellido, cuenta_colones, cuenta_dolares)" 
        "VALUES (544018749, 'Lorraine', 'Ryan', 1544018749, 2544018749);"
        "INSERT INTO prestamos (prestamo_id, denominacion, tipo, monto_total, plazo_meses, plazo_restante, cuota_mensual, tasa, cliente_id)"  
        "VALUES (1, 'colones', 'prendario', 2000000, 12, 5, 400000.0, 0.15, 326823600);"
        "INSERT INTO prestamos (prestamo_id, denominacion, tipo, monto_total, plazo_meses, plazo_restante, cuota_mensual, tasa, cliente_id)" 
        "VALUES (2, 'colones', 'personal', 5000000, 12 ,10, 416666.67, 0.1, 456251480);"
        "INSERT INTO prestamos (prestamo_id, denominacion, tipo, monto_total, plazo_meses, plazo_restante, cuota_mensual, tasa, cliente_id)" 
        "VALUES (3, 'dolares', 'hipotecario', 117187.5, 15, 13, 1000000.0, 0.08, 409975644);"
        "INSERT INTO certificados_de_deposito (denominacion, tasa, plazo_meses, monto_deposito, fecha_deposito, cliente_cedula)" 
        "VALUES ('colones', 0.12, 5, 1000000.00, '2016-05-15', 732194468);"
        "INSERT INTO certificados_de_deposito (denominacion, tasa, plazo_meses, monto_deposito, fecha_deposito, cliente_cedula)" 
        "VALUES ('colones', 0.12, 10, 1000000.00, '2017-09-22', 114819158);"
        "INSERT INTO certificados_de_deposito (denominacion, tasa, plazo_meses, monto_deposito, fecha_deposito, cliente_cedula)" 
        "VALUES ('dolares', 0.12, 13, 1953.12, '2019-01-13', 544018749);";



    rc = sqlite3_exec(db, inserts_sql, callback, 0, &errMsg);
    if (rc != SQLITE_OK){
        cerr << "Error de SQL: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Informacion agregada de forma correcta" << endl;
    }
    
    sqlite3_close(db);
    return 0;
}