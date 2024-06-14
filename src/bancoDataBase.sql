CREATE DATABASE IF NOT EXISTS banco;
USE banco;

CREATE TABLE cuentas (
    numero_cuenta INT PRIMARY KEY,
    balance DECIMAL(9, 2),
    tasa DECIMAL(2, 2),
    denominacion VARCHAR(10)
);

CREATE TABLE clientes (
    cedula CHAR(9) PRIMARY KEY,
    nombre VARCHAR(50),
    apellido VARCHAR(50),
    cuenta_colones INT,
    cuenta_dolares INT,
    cdp INT,
    CHECK (cedula NOT LIKE '%[^0-9]%'),
    FOREIGN KEY (cuenta_colones) REFERENCES cuentas(numero_cuenta),
    FOREIGN KEY (cuenta_dolares) REFERENCES cuentas(numero_cuenta)
);

CREATE TABLE prestamos (
    prestamo_id INT PRIMARY KEY,
    denominacion VARCHAR(8),
    tipo VARCHAR(10),
    monto_total DECIMAL(9, 2),
    plazo_meses INT,
    cuota_mensual DECIMAL(9, 2),
    cliente_id CHAR(9),
    FOREIGN KEY (cliente_id) REFERENCES clientes(cedula)
);

CREATE TABLE certificados_de_deposito (
    cdp_id INT PRIMARY KEY,
    denominacion VARCHAR(8),
    tasa DECIMAL(2, 2),
    plazo_meses INT,
    monto_deposito DECIMAL(9, 2),
    cliente_cedula CHAR(9),
    FOREIGN KEY (cliente_cedula) REFERENCES clientes(cedula)
);

CREATE TABLE registros (
    ID INT PRIMARY KEY,
    tipo_transaccion VARCHAR(50),
    fecha_transaccion DATE,
    denominacion VARCHAR(10),
    cliente_origen_cedula CHAR(9),
    cliente_destino_cedula CHAR(9),
    monto_base DECIMAL(9,2),
    FOREIGN KEY (cliente_origen_cedula) REFERENCES clientes(cedula),
    FOREIGN KEY (cliente_destino_cedula) REFERENCES clientes(cedula)
);