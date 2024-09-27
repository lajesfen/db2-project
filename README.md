# Data Fusion DB - Base de Datos II

## Introducción

Comprender y aplicar de manera efectiva las técnicas de organización e indexación de archivos físicos con el desarrollo de un gestor de bases de datos, que optimice la gestión, almacenamiento y recuperación de datos de un `dataset`.

### Objetivos

1. **Analizar diferentes técnicas de indexación:**
   - Implementar las técnicas AVL File, Sequential File y Extendible Hashing.
   - Comparar el tiempo y eficiencia de consultas con distintas técnicas.

2. **Implementar las operaciones:**
   - Búsqueda específica.
   - Búsqueda por rango.
   - Agregación y eliminación de un registro.

3. **Crear una interfaz gráfica:**
   - Manipulación de la base de datos.
   - Realización de consultas SQL.

### Resultados Esperados

### Técnicas de Indexación

Las técnicas implementadas son las siguientes:

* AVL
* Sequential File
* Extendible Hashing

### Funciones Implementadas

Para cada una de las técnicas de indexación se implementaron las siguientes funciones:

* Search
* Range Search
* Add
* Remove

### Dataset

Se utilizaron los datasets de la [Plataforma Nacional de Datos Abiertos](https://www.datosabiertos.gob.pe/).

- [Directorio de Establecimientos de Salud](https://www.datosabiertos.gob.pe/dataset/directorio-de-establecimientos-de-salud/resource/e396181b-457d-470d-a8a6-98d6c8650f47#{})
- [Directorio Nacional de Redes Sociales](https://www.datosabiertos.gob.pe/dataset/directorio-nacional-de-redes-sociales/resource/19cd3462-0d84-4db2-ad73-88e6cb08db27)

La selección del dataset se baso en los siguientes factores:

- **Tamaño del Dataset**: La cantidad de datos que contiene cada dataset es de gran tamaño  lo que facilita la realización de pruebas exhaustivas de consultas y la evaluación de la eficiencia de las técnicas de indexación. Un tamaño adecuado del dataset permite analizar el comportamiento de las estructuras de datos en situaciones de carga realista, proporcionando información valiosa sobre su rendimiento.

- **Variedad de Tipos de Datos**: La diversidad en los tipos de datos presentes en el dataset es fundamental para evaluar la capacidad de las estructuras de datos en la gestión de diferentes formatos e información. Esto incluye no solo datos numéricos y texto, sino también fechas y valores booleanos, entre otros. 

- **Complejidad/Variedad de Consultas**: La abundancia y variedad de datos nos brinda la oportunidad de realizar diferentes tipos de consultas (simples y más complejas), lo que resulta esencial para evaluar el rendimiento y la eficiencia de las técnicas de indexación.

#### Estructura de cada registro

#### **Social Record**

| Campo         | Tipo de Dato         | Descripción                                                                                  |
|---------------|----------------------|----------------------------------------------------------------------------------------------|
| `id`          | `int`                | Id único del registro.                                                                       |
| `nombre`      | `char[130]`          | Nombre completo de la entidad.                                                               |
| `abreviatura` | `char[40]`           | Abreviatura del nombre de la entidad.                                                        |
| `url`         | `char[50]`           | URL del sitio web oficial de la entidad.                                                     |
| `facebook`    | `char[60]`           | URL de la página oficial de Facebook.                                                        |
| `twitter`     | `char[60]`           | URL de la cuenta oficial de Twitter.                                                         |
| `instagram`   | `char[60]`           | URL del perfil oficial de Instagram.                                                         |
| `linkedin`    | `char[80]`           | URL del perfil de LinkedIn de la entidad.                                                    |
| `youtube`     | `char[90]`           | URL del canal oficial de YouTube.                                                            |
| `tiktok`      | `char[60]`           | URL del perfil oficial de TikTok.                                                            |

#### **Hospital Record**

| Campo          | Tipo de Dato        | Descripción                                                                                |
|----------------|---------------------|--------------------------------------------------------------------------------------------|
| `id`           | `int`               | Id único del registro.                                                                     |
| `institucion`  | `char[50]`          | Nombre de la institución o entidad del hospital.                                           |
| `nombre`       | `char[100]`         | Nombre completo del hospital.                                                              |
| `clasificacion`| `char[100]`         | Clasificación del hospital (por ejemplo, general, especializado, etc.).                    |
| `tipo`         | `char[50]`          | Tipo de hospital (público, privado, etc.).                                                 |
| `departamento` | `char[30]`          | Nombre del departamento donde se ubica el hospital.                                        |
| `provincia`    | `char[30]`          | Nombre de la provincia donde se encuentra el hospital.                                     |
| `distrito`     | `char[30]`          | Nombre del distrito donde se localiza el hospital.                                         |
| `ubigeo`       | `int`               | Código de ubicación geográfica del hospital.                                               |
| `direccion`    | `char[180]`         | Dirección física completa del hospital.                                                    |
| `telefono`     | `char[40]`          | Número de teléfono del hospital.                                                           |
| `estado`       | `char[8]`           | Estado del hospital (por ejemplo, "activo", "cerrado").                                    |
| `camas`        | `int`               | Número de camas disponibles en el hospital.                                                |
| `ruc`          | `long`              | Registro Único de Contribuyentes del hospital.                                             |

## Técnicas Utilizadas

### AVL File

#### Search

#### Range Search

#### Add

#### Remove

### Sequential File

#### Search

La función `search` está diseñada para realizar la búsqueda exacta de un registro en un archivo principal y un archivo auxiliar a través de su clave. Devuelve el registro encontrado o nada, en caso no lo encuentre.

#### Descripción del Proceso

#### Range Search

La función `rangeSearch` está diseñada para la búsqueda de registros por rango. La función recibirá los límites inferior (`begin_key`) y superior (`begin_key`) del rango.

#### Descripción del Proceso

#### Add

#### Descripción del Proceso

#### Remove

La función `remove` está diseñada para la eliminación de un registro en específico utilizando la llave para la búsqueda. 

#### Descripción del Proceso

### Extendible Hashing

#### Search

#### Descripción del Proceso

#### Range Search

#### Descripción del Proceso

#### Add

#### Descripción del Proceso

#### Remove

#### Descripción del Proceso

## Parser SQL
### Comandos SQL Implementados:
**CREATE TABLE**

Crea una tabla en base a un archivo específico y elige la estructura de datos a usar (`AVL`, `Sequential`, o `Hash`).
   
``` 
create table Datitos from file hospitalesopendata using AVL
```

**INSERT INTO**

Inserta uno o más registros en la tabla especificada. Los registros se proporcionan como parte de la consulta SQL.
```sql
insert into Datitos values (1,Presidencia de la República del Perú,Presidencia,/presidencia,www.facebook.com/PresidenciaPeru,www.twitter.com/presidenciaperu,www.instagram.com/presidenciaperu,www.linkedin.com/company/74524807/admin/,www.youtube.com/presidenciaperu,www.tiktok.com/@presidenciaperu)
```

**SELECT**
      
SELECT * FROM ... WHERE: Permite realizar búsquedas basadas en elcampo clave (`id`)

```sql
select * from Datitos where id = 3
```

SELECT * FROM ... WHERE ... BETWEEN: Realiza búsquedas en un rango de claves.

```sql
select * from Datitos where id between 3 and 17
```

**DELETE FROM**: Elimina un registro basado en su `id`.
```	sql
delete from Datitos where id = 3
```

### Componentes Clave

#### 1. Tokenización
- La función `tokenize` toma una consulta SQL como una cadena de texto y la divide en un vector de **tokens** (palabras clave). Estos tokens son interpretados por el parser para identificar las acciones a realizar, como:
  - Crear una tabla
  - Insertar registros
  - Seleccionar datos
  - Eliminar registros

#### 2. Manejo de valores
- La función `extraerRegistros` toma la porción de la consulta SQL que contiene los valores a insertar, los **tokeniza** y organiza en un vector de cadenas. Esto permite que los valores se conviertan en registros completos como `HospitalRecord` o `SocialRecord`.
  
- Estos registros son insertados adecuadamente en las estructuras de datos como **AVL**, **SequentialFile**, o **Extendible Hashing**, permitiendo una manipulación eficiente de los datos en el sistema.

**Ejemplo de Consulta SQL**

```sql
insert into Datitos values 
(5577,GOBIERNO REGIONAL,Hospital Amazónico-Yarinacocha,
HOSPITALES O ClínicaS de ATENCION GENERAL,ESTABLECIMIENTO de
SALUD CON INTERNAMIENTO,Ucayali,CoronelPortillo,Yarinacocha,250105,Jirón Aguaytía
605,(061)596408,ACTIVO,165,20175940015),(1234, GOBIERNO REGIONAL,
Hospital Regional de Cusco,HOSPITALES O ClínicaS de ATENCION GENERAL,
ESTABLECIMIENTO de SALUDCON INTERNAMIENTO,Cusco,Cusco,Cusco,080101,
Avenida Cultura 1234, (084)567891,ACTIVO,200,20112345678)
```
## Experimentación

### Cuadro Comparativo de Técnicas de Indexación

Video: [pruebas de experimentación](link)
