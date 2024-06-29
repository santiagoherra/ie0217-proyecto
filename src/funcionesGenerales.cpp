#include "funcionesGenerales.hpp"


bool existeCliente(const std::string& clienteID){

    sqlite3 *db;
    int rc;

    rc = sqlite3_open("banco.db", &db);
    if (rc) {
        std::cerr << "No se puede abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    const char* consultaSql = "SELECT 1 FROM clientes WHERE cedula = ? LIMIT 1;";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, consultaSql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "No se puede preparar la declaración: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_bind_text(stmt, 1, clienteID.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        std::cerr << "Error al enlazar el valor: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_step(stmt);
    bool existe = (rc == SQLITE_ROW);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return existe;
}

int agregarCliente(const std::string clienteID, std::string nombre, std::string apellido,
                    long int cuentacolones, long int cuentadolares){

    sqlite3 *db;
    int rc;

    rc = sqlite3_open("banco.db", &db);
    if (rc) {
        std::cerr << "No se puede abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    const char* insertarSql = "INSERT INTO clientes (cedula, nombre, apellido, cuenta_colones,"
                              "cuenta_dolares) VALUES (?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, insertarSql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "No se puede preparar la declaración: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    //vincular datos

    sqlite3_bind_text(stmt, 1, clienteID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, nombre.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, apellido.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, cuentacolones);
    sqlite3_bind_int(stmt, 5, cuentadolares);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Error al ejecutar la declaración: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    std::cout << "El cliente ha sido agregado de manera exitosa!" << std::endl;

    return 0;
    
}

std::tm string_a_fecha(const std::string& fecha_string) {
    std::tm tm = {};
    std::istringstream ss(fecha_string);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    return tm;
}

std::string obtenerFechaString() {
    auto fechaActual = std::chrono::system_clock::now();
    std::time_t fechaActualTime = std::chrono::system_clock::to_time_t(fechaActual);
    std::tm fechaActualLocal = *std::localtime(&fechaActualTime);

    // Crear una cadena compatible con las consultas para la base de datos
    std::ostringstream oss;
    oss << std::put_time(&fechaActualLocal, "%Y-%m-%d");

    // Devolver cadena
    return oss.str();
}

void leerInt(int num, int op1 = 0, int op2 = 0, int op3 = 0) {
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Entrada no valida. Se esperaba un numero entero." << std::endl;
        std::exit(EXIT_FAILURE);  // Termina el programa con un estado de fallo
    }

    else if(op1 != 0 || op2 != 0 || op3 != 0){
        if(num != op1 || num != op2 || num != op3){
            std::cerr << "Entrada no valida. No existe la opcion seleccionada" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
}


void leerCedula(std::string cedula) {

    std::regex patron("^\\d{10}$");
    if (!std::regex_match(cedula, patron)) {
        std::cerr << "Entrada no valida. Se esperaba una forma de cedula." << std::endl;
        std::exit(EXIT_FAILURE);  // Termina el programa con un estado de fallo
    }
}