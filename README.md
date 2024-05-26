# Proyecto Final
## Curso: Estructuras abstractas de datos y algoritmos para ingenieria 0217.
## Prof. Esteban Badilla
### Integrantes:
* Santiago Herra Castro C1721
* José Ignacio Sáenz Díaz C27032
* Kevin Jiménez Acuña C13876

## Descripción de proyecto.
Este proyecto se centra en la investigación, diseño e implementación de un sistema de gestión bancaria. Esta siendo para poder ser utilizado por agentes en ventanillas bancaria, facilitando tanto la atención a clientes como la gestion de información de prestamos bancarios.

### Funcionalidad:

* El sistema inicia preguntando si se va a utilizar para la atención al cliente o para gestionar información de préstamos, asi teniendo una interacción específica según el objetivo del usuario.
* Si la persona desea gestionar información sobre préstamos se generara una tabla de pagos personalizados en base al monto que el usuario ingrese, con esto se generará la tabla con la cantidad de cuotas, tasas de interés y los diferentes plazo a la disposición del banco.
* Los clientes pueden gestionar hasta 2 cuentas bancarias. una en colones y otra en dólares, además de obtener certificados de depósito a plazo.
* El sistema cuenta con diversas operaciones disponibles, como lo son depósitos, retiros, transferencias entre cuentas dirigidas a otros clientes, entre otras.
* El sistema cuenta con un registro de tipo historial de todas las transacciones realizadas en el sistema. Este registro muestra el número identificador del cliente, el tipo de transacción que realizó, el monto implicado en dicha transacción y el número identificador de un segundo cliente en caso de que el tipo de transacción involucre a un segundo.
* Se identifica a los clientes en el sistema, con un número único de identificación, el cual estará limitado a un número entero entre 0 y 999999999.
* Los clientes pueden contar con préstamos asociados en colones o en dólares. Cada uno de estos con características específicas como la tasa de interés, el período en meses y la cuota mensual.
* Los clientes pueden solicitar un reporte de sus préstamos, a lo que el sistema responderá generando un archivo de texto que detalle cada una de las cuotas pagadas, desglose el aporte de capital y los intereses abonados hasta el momento.

## Prestamos:
Los prestamos se realizarán en base al monto que la persona desee obtener, el salario que la persona posea y moneda que la persona desee. En cuanto se tenga esto se desplegará una tabla con las opciones posibles donde la persona podrá elegir la cuota mensual que deberá pagar en base al lapso de tiempo por el que escoja el préstamos, esto dependiendo del salario que esta persona tenga. Las tasas de interés las dara el banco las cuales varían dependiendo del tipo de préstamo que la persona escoja. Además, se debe de considerar que los préstamos pueden ser en colones o en dólares.

### Pestamos personales:
Los prestamos personales es un tipo de credito **no asegurado** que ofrecen los bacos y ptras instituciones financieras a los individuos. Este tipo de prestamo no requiere que la persona ofrazca un objeto colateral para asegurar el prestamo. La tasa de interes que dara el banco para un prestamos personal sera de 4%, entonces la formula para calcular las coutas es:



Si la persona desea tener un prestamos por 1000000 de colones:

## Préstamos prendarios:
Los préstamos prendarios son aquellos en los que la entidad bancaria solicita al cliente que requiere el préstamo, entregarle uno o varios activos físicos o financieros como garantía de que cumplirá con lo acordado según el préstamo [1]. Una forma más sencilla de comprender este tipo de préstamo es visualizarlo como si fuera lo que popularmente se conoce como "empeño", en donde un individuo cede un activo físico de forma temporal a una casa de empeño a cambio de dinero; en el caso del préstamo prendario sucede de forma similar, cediendo bienes como vehículos y acciones que permitirán eventualmente a la entidad bancaria, embargar o vender en caso de que el cliente incumpla con sus acuerdos y de esta forma, recuperar la inversión [1].

En cuanto a las características generales de este tipo de préstamos, se encuentra una baja considerable en los intereses a comparación de los préstamos personales (estos son más comunes en las entidades bancarias), encontrándose estas entre el 1% y 5% y además, generalmente este tipo de préstamos cuenta con plazos de rembolso menores, siendo estos de entre 1 y 8 años (dependiendo de la cantidad de dinero solicitada por el cliente) [1]. Es necesario destacar que la cantidad de dinero recibida por el cliente puede depender del valor de los bienes que haya entregado, y estos no serán devueltos hasta que se finalice la devolución del dinero prestado [1].

Por último, el cliente que presenta el préstamo prendario no se ve afectado por comisiones por amortización o cancelación anticipada; esto quiere decir que el cliente que entregó uno o más bienes puede recuperarlo sin tener que realizar ningún tipo de gasto extra como consecuencia [1]. 

En este sistema, para solicitar un crédito prendario, es necesario tomar en cuenta que:

* Se financia el 80% del valor del avualuo del colateral ofrecido como garantía.
* Financiación mínima es de cinco millones de colones.
* El cliente solcitante requiere 6 meses de continuidad laboral.

A continuación, se muestran la tabla de valores con las opciones que puede seleccionar el cliente:

| Tasa de interés anual | 6 años  | 7 años  | 8 años  | Tipo de Moneda |
|-----------------------|---------|---------|---------|----------------|
| 12%                   |97 750,96|88 263,66|81 264,21| Colones        |
| 7,25%                 |167,64   |148,56   |134,33   | Dólares        |

**Nota, esto sería para el caso en que se soliciten cinco millones de colones o su equivalente en dólares.**

### Cuentas bancarias y gestión de ahorro:

La personas pueden tener cuentas bancarias las cuales pueden ser en dólares o colones, dichas pueden poseer un máximo de dos cuentas bancarias. Estas están destinadas a la gestión de sus ahorros, donde que se utilizarán para obtener certificados de depósito a plazo. Los certificados de depósito a plazo son certificados que dan el banco gracias al ingreso de ahorros de parte de la persona, estos certificados se basan en un pago de interés que se le dará a la persona por el ingreso de sus ahorros por un plazo de tiempo definido. El banco da una tasa de interés para ahorros en colones y en dólares. 

### Base de datos:

Para el correcto funcionamiento del programa, es necesario establecer una base de datos corrupta y eficiente, que tenga la cantidad justa de tablas relacionadas, para que la posterior inserción, modificación y deleción de información sea de forma sencilla e intuitiva. Por ello, es importante plantear desde etapas tempranas la estructura de dicha base de datos, con el fin de optimizar los algoritmos a implementar. Por ello, se definirán las tablas pertenecientes a la base de datos del sistema bancario.

#### Banco:

Esta tabla contiene la información del banco, donde existen las siguientes filas:
- Nombre del banco
- Tipo de cambio
- Préstamos Personales
- Préstamos Prendarios
- Cuenta de ahorros
- CDP
En las filas de Préstamos, cuenta de ahorros y CDP se tendrán subtablas, donde se almacenará la información asociada a las tasas de interés y los plazos.
#### Clientes:
La tabla clientes contendrá la información de cada cliente por separado. Esta almacenará el número de cédula que será la llave primaria y el nombre completo del cliente.

| **Columna**       | **Tipo de Dato** | **Descripción**                              |
|-------------------|------------------|----------------------------------------------|
| cedula            | INT              | Número de cédula (llave primaria)            |
| nombre            | VARCHAR(50)      | Nombre del cliente                           |
| apellido          | VARCHAR(50)      | Apellido del cliente                         |
| cuenta_colones    | INT              | Cuenta en colones                            |
| cuenta_dolares    | INT              | Cuenta en dólares                            |
| cdp               | INT              | Certificado de Depósito a Plazo (CDP)        |

```sql
CREATE TABLE clientes (
    cedula INT PRIMARY KEY,
    nombre VARCHAR(50),
    apellido VARCHAR(50),
    cuenta_colones INT,
    cuenta_dolares INT,
    cdp INT
);
```

Los enteros de cuenta_colones, cuenta_dolares y cdp hacen referencia al número de cuenta o número de certificado, que es un número único. 

#### Cuentas:
Las cuentas incluirán el número de cuenta que será la llave primaria, el balance de la cuenta, que será un DECIMAL(9,2) para almacenar un máximo de centésimas de colón o de dolar. Por otra parte, tendrá un Tasa decimal(2,2) que contendrá la tasa de interés de ahorro de la cuenta. La tabla se observa a continuación:

| **Columna**   | **Tipo de Dato** | **Descripción**                           |
|---------------|------------------|-------------------------------------------|
| numero_cuenta | INT              | Número de cuenta (llave primaria)         |
| balance       | DECIMAL(9, 2)    | Balance de la cuenta                      |
| tasa          | DECIMAL(2, 2)    | Tasa de interés de ahorro de la cuenta    |

```sql
CREATE TABLE cuentas (
    numero_cuenta INT PRIMARY KEY,
    balance DECIMAL(9, 2),
    tasa DECIMAL(2, 2)
);
```
## Referencias

[1] “¿Qué es un préstamo prendario?: Características y ejemplos”. Santander Consumer Finance. Accedido el 24 de mayo de 2024. [En línea]. Disponible: https://www.santanderconsumer.es/blog/post/que-es-un-prestamo-prendario-caracteristicas-y-ejemplos
