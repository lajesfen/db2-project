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

- `AVL`
- `Sequential File`
- `Extendible Hashing`

### Funciones Implementadas

Para cada una de las técnicas de indexación se implementaron las siguientes funciones:

- Search
- Range Search
- Add
- Remove

### Dataset

Se utilizaron los datasets de la [Plataforma Nacional de Datos Abiertos](https://www.datosabiertos.gob.pe/).

- [Directorio de Establecimientos de Salud](https://www.datosabiertos.gob.pe/dataset/directorio-de-establecimientos-de-salud/resource/e396181b-457d-470d-a8a6-98d6c8650f47#{})
- [Directorio Nacional de Redes Sociales](https://www.datosabiertos.gob.pe/dataset/directorio-nacional-de-redes-sociales/resource/19cd3462-0d84-4db2-ad73-88e6cb08db27)

La selección del dataset se baso en los siguientes factores:

1. **Tamaño del Dataset**

   - La cantidad de datos que contiene cada dataset es de gran tamaño lo que facilita la realización de pruebas exhaustivas de consultas y la evaluación de la eficiencia de las técnicas de indexación. Un tamaño adecuado del dataset permite analizar el comportamiento de las estructuras de datos en situaciones de carga realista, proporcionando información valiosa sobre su rendimiento.

2. **Variedad de Tipos de Datos**

   - La diversidad en los tipos de datos presentes en el dataset es fundamental para evaluar la capacidad de las estructuras de datos en la gestión de diferentes formatos e información. Esto incluye no solo datos numéricos y texto, sino también fechas y valores booleanos, entre otros.

3. **Complejidad/Variedad de Consultas**
   - La abundancia y variedad de datos nos brinda la oportunidad de realizar diferentes tipos de consultas (simples y más complejas), lo que resulta esencial para evaluar el rendimiento y la eficiencia de las técnicas de indexación.

#### **Estructura de cada registro**

#### **Social Record**

| Campo         | Tipo de Dato | Descripción                               |
| ------------- | ------------ | ----------------------------------------- |
| `id`          | `int`        | Id único del registro.                    |
| `nombre`      | `char[130]`  | Nombre completo de la entidad.            |
| `abreviatura` | `char[40]`   | Abreviatura del nombre de la entidad.     |
| `url`         | `char[50]`   | URL del sitio web oficial de la entidad.  |
| `facebook`    | `char[60]`   | URL de la página oficial de Facebook.     |
| `twitter`     | `char[60]`   | URL de la cuenta oficial de Twitter.      |
| `instagram`   | `char[60]`   | URL del perfil oficial de Instagram.      |
| `linkedin`    | `char[80]`   | URL del perfil de LinkedIn de la entidad. |
| `youtube`     | `char[90]`   | URL del canal oficial de YouTube.         |
| `tiktok`      | `char[60]`   | URL del perfil oficial de TikTok.         |

#### **Hospital Record**

| Campo           | Tipo de Dato | Descripción                                                             |
| --------------- | ------------ | ----------------------------------------------------------------------- |
| `id`            | `int`        | Id único del registro.                                                  |
| `institucion`   | `char[50]`   | Nombre de la institución o entidad del hospital.                        |
| `nombre`        | `char[100]`  | Nombre completo del hospital.                                           |
| `clasificacion` | `char[100]`  | Clasificación del hospital (por ejemplo, general, especializado, etc.). |
| `tipo`          | `char[50]`   | Tipo de hospital (público, privado, etc.).                              |
| `departamento`  | `char[30]`   | Nombre del departamento donde se ubica el hospital.                     |
| `provincia`     | `char[30]`   | Nombre de la provincia donde se encuentra el hospital.                  |
| `distrito`      | `char[30]`   | Nombre del distrito donde se localiza el hospital.                      |
| `ubigeo`        | `int`        | Código de ubicación geográfica del hospital.                            |
| `direccion`     | `char[180]`  | Dirección física completa del hospital.                                 |
| `telefono`      | `char[40]`   | Número de teléfono del hospital.                                        |
| `estado`        | `char[8]`    | Estado del hospital (por ejemplo, "activo", "cerrado").                 |
| `camas`         | `int`        | Número de camas disponibles en el hospital.                             |
| `ruc`           | `long`       | Registro Único de Contribuyentes del hospital.                          |

## Técnicas Utilizadas

### AVL File

#### Search

#### Range Search

#### Add

#### Remove

### Sequential File

**SEARCH**

La función `search` está diseñada para realizar la búsqueda exacta de un registro en un archivo principal y un archivo auxiliar a través de su clave. Devuelve el registro encontrado o nada, en caso no lo encuentre.

#### Descripción del Proceso

   1. Inicialización de Variables

      ```cpp
      Registro reg;
      std::ifstream file(filename, std::ios::binary);
      ```

      - `std::ifstream file(filename, std::ios::binary)`: Se abre el archivo principal en modo de lectura binaria. El nombre del archivo es proporcionado en el constructor de la clase.

   2. Lectura de Archivo Principal

   - Este bucle se ejecuta mientras se pueda leer un registro completo del archivo.

   ```cpp
   while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
   ```

   3. Comparación de claves en el Archivo Principal

   - Dentro del bucle, se compara la clave del registro leído (`reg.key`) con la clave buscada (`key`). Si se encuentra una coincidencia, se cierra el archivo y devuelve el registro encontrado.

   ```cpp
   if (reg.key == key)
   {
      file.close();
      return reg;
   }
   ```

   4. Búsqueda y Lectura en Archivo Auxiliar

   - Si el registro no se encontró en el archivo principal, se abre el archivo auxiliar.

   ```cpp
   file.open(aux_filename, std::ios::binary);
   ```

   - Al igual que con el archivo principal, se intenta leer registros del archivo auxiliar en un bucle.

   ```cpp
   while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
   ```

   6. Comparación de Claves en el Archivo Auxiliar

   - Se compara nuevamente la clave del registro leído con la clave buscada. Si se encuentra una coincidencia se cierra el archivo auxiliar y retorna el registro encontrado.

   ```cpp
   if (reg.key == key)
   {
      file.close();
      return reg;
   }
   ```

   7. Manejo de excepciones

   - Si el registro no se encuentra en ninguno de los archivos, se lanza una excepción std::runtime_error con el mensaje "Not found". Esto informa al usuario de que no se encontró el registro con la clave especificada.

   ```cpp
   throw std::runtime_error("Not found");
   ```

**RANGE SEARCH**

La función `rangeSearch` está diseñada para la búsqueda de registros por rango. La función recibirá los límites inferior (`begin_key`) y superior (`begin_key`) del rango de búsqueda.

#### Descripción del Proceso

   1. Inicialización de Variables

   ```cpp
   std::vector<Registro> results;
   Registro reg;
   std::ifstream file(filename, std::ios::binary);
   ```

   - `vector<Registro>`: Se declara un vector llamado results que almacenará los registros que cumplan con la condición de búsqueda dentro del rango.

   2. Lectura de Archivo Principal

   - Este bucle se ejecuta mientras se pueda leer un registro completo del archivo.

   ```cpp
   while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
   ```

   3. Comparación de Claves

   - Dentro del bucle, se verifica si la clave del registro leído (`reg.key`) está dentro del rango definido por `begin_key` y `end_key`.

   - Si la clave está dentro del rango, se agrega el registro al vector `results`.

   ```cpp
   if (reg.key >= begin_key && reg.key <= end_key)
   {
      results.push_back(reg);
   }
   ```

   4. Búsqueda y Lectura en el Archivo Auxiliar

   - Si la búsqueda no se limita a los registros en el archivo principal, se abre el archivo auxiliar. Al igual que con el archivo principal, se intenta leer registros del archivo auxiliar en un bucle.

   ```cpp
   file.open(aux_filename, std::ios::binary);

   while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
   ```
   5. Comparación de Claves en el Archivo Auxiliar

   - Se compara nuevamente la clave del registro leído con el rango definido por `begin_key` y `end_key`. Si la clave está dentro del rango, se agrega el registro al vector `results`.

   ```cpp
   if (reg.key >= begin_key && reg.key <= end_key)
   {
      results.push_back(reg);
   }
   ```

4.  **ADD**

#### Función `Merge`
Esta función se encarga de fusionar los registros almacenados en el archivo auxiliar y el archivo principal en un único archivo principal ordenado. Se llama cuando el número de registros en el archivo auxiliar alcanza un límite (`MAX_AUX_RECORD`).

   1. Lectura del Archivo Principal
   - Se abre el archivo principal y mientras se pueda leer un registro completo del archivo principal, se agrega cada registro leído al vector `allRecords`.

      ```cpp
      std::ifstream file(filename, std::ios::binary);

      while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
      {
         allRecords.push_back(reg);
      }
      file.close();
      ```
   2. Lectura del Archivo Auxiliar
   - De igual manera que el archivo principal, se lee cada registro del archivo auxiliar y se agrega al vector `allRecords`.

   ```cpp
   file.open(aux_filename, std::ios::binary);

      while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
      {
         allRecords.push_back(reg);
      }
      file.close();
      ```

   3. Ordenamiento de Registros
   - Se ordenan todos los registros almacenados en el vector `allRecords` utilizando la clave de cada registro. La función lambda compara las claves para determinar el orden.

   ```cpp
   std::sort(allRecords.begin(), allRecords.end(), [](const Registro &a, const Registro &b){ return a.key < b.key; });
   ```

   4. Escritura de Registros en el Archivo Principal

   - Se abre el archivo principal y se trunca (vacía) para que se escriban solo los registros fusionados.

   ```cpp
      std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
      for (const auto &r : allRecords)
      {
         outFile.write(reinterpret_cast<const char *>(&r), sizeof(Registro));
      }
      outFile.close();
      ```
   5. Vaciar el Archivo Auxiliar
   - Se trunca el archivo auxiliar, dejandolo vacío después de que se han fusionado sus registros en el archivo principal.

   ```cpp
   std::ofstream auxFile(aux_filename, std::ios::binary |std::ios::trunc);
   auxFile.close();
   ```
#### Función `getAuxRecordCount`
Este método se encarga de contar el número de registros que hay en el archivo auxiliar (`aux_filenae`) y devuelve este conteo.

   - Se abre el archivo auxiliar en modo lectura para leer los datos del archivo sin alterar su contenido.
   - Se utiliza la función `seekg` para mover el puntero de lectura al final del archivo.
   - Se obtiene la posición actual del puntero usando `tellg`, se divide por `sizeof(Registro)` (representa el tamaño de cada registro en bytes) para obtener el número total de registros en el archivo. 

   ```cpp
   int getAuxRecordCount()
   {
      std::ifstream file(aux_filename, std::ios::binary);
      file.seekg(0, std::ios::end);
      int count = file.tellg() / sizeof(Registro);
      file.close();
      return count;
   }
   ```

#### Descripción del Proceso

   1. Registro existente
   - Antes de realizar la inserción, se intenta buscar el registro utilizando su clave. Si el registro ya existe, se lanza una excepción `std::runtime_error`. La función simplemente retorna sin hacer nada, evitando duplicados.

   ```cpp
   try
   {
      search(registro.key);
      return;
   }
   catch (const std::runtime_error &){}
   ```

   2. Escritura del Archivo Auxiliar
   Se abre el archivo auxiliar y se agrega el nuevo registro al final del archivo.

   ```cpp
      std::ofstream auxFile(aux_filename, std::ios::binary | std::ios::app);
      auxFile.write(reinterpret_cast<const char *>(&registro), sizeof(Registro));
      auxFile.close();
   ```
   3. Verificación y Fusión de Registros
   - Se verifica si el nÚmero de registros en el archivo auxiliar ha alcanzado el límite máximo (`MAX_AUX_RECORD`). Si alcanza el límite, se llama a la función `mergeFiles` para fusionar los registros en el archivo principal.

   ```cpp
   if (getAuxRecordCount() >= MAX_AUX_RECORD)
   {
      mergeFiles();
   }
   ```

**REMOVE**

La función `remove` está diseñada para la eliminación de un registro en específico utilizando la llave para la búsqueda.

#### Descripción del Proceso

   1. Inicialización de Variables

   - `vector<Registro> registros`: Almacenará todos los registros que no coincidan con la clave de eliminación.
   - `found = false`: Se inicializará una variable booleana `found` para rastrear si se encontró el registro que se desea eliminar.

   ```cpp
   bool remove(TK key)
   {
      bool found = false;
      std::vector<Registro> registros;
      Registro reg;
   ```
   2. Lectura de Registros en el Archivo Principal

   - Si la clave del registro actual no coincide con la clave proporcionada, se agrega el registro al vector `registros`.

   - Caso contrario, se establece `found = true`, indicando que se ha encontrado el registro a eliminar.

   ```cpp
   while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
   {
      if (reg.key != key)
      {
         registros.push_back(reg);
      }
      else
      {
         found = true;
      }
      }
      file.close();
   ```
   3. Verificación de la Existencia del Registro
   - Si la variable `found` sigue siendo `false`, significa que no se encontró el registro a eliminar. En este caso, la función devuelve `false`.

   ```cpp
   if (!found)
   {
      return false;
   }
   ```
   4. Escritura de Registros restants
   - Se abre el archivo principal y se trunca (vacía) para que se puede escribir el contenido actualizado sin el registro eliminado.

   - Se escribe cada registro que no coincide con la clase proporcionada en el archivo principal. Esto garantiza que el registro a eliminar no se incluya en el archivo.

   ```cpp
   std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
   for (const auto &r : registros)
   {
      outFile.write(reinterpret_cast<const char *>(&r), sizeof(Registro));
   }
   outFile.close();
   ```

### Extendible Hashing

#### Search

#### Range Search

#### Add

#### Remove

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

SELECT \* FROM ... WHERE: Permite realizar búsquedas basadas en elcampo clave (`id`)

```sql
select * from Datitos where id = 3
```

SELECT \* FROM ... WHERE ... BETWEEN: Realiza búsquedas en un rango de claves.

```sql
select * from Datitos where id between 3 and 17
```

**DELETE FROM**: Elimina un registro basado en su `id`.

```sql
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
