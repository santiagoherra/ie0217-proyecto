#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <iostream>

int main() {
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;

    try {
        // Create a connection
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "Patoku800");

        // Connect to the MySQL database
        con->setSchema("banco");

        // Create a new statement
        stmt = con->createStatement();

        // Execute an SQL query
        stmt->execute("INSERT INTO clientes (cedula, nombre, apellido, cuenta_colones, cuenta_dolares, cdp) VALUES ('123456789', 'John', 'Doe', 1, 2, 3)");

        std::cout << "Record inserted successfully" << std::endl;

        // Clean up
        delete stmt;
        delete con;
    } catch (sql::SQLException &e) {
        std::cerr << "SQLException: " << e.what() << std::endl;
        std::cerr << "MySQL error code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQLState: " << e.getSQLState() << std::endl;
    }

    return 0;
}