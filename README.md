# Data Fusion DB - Base de Datos II

## Introducción

Comprender y aplicar de manera efectiva las técnicas de organización e indexación de archivos físicos con el desarrollo de un gestor de bases de datos, que optimice la gestión, almacenamiento y recuperación de datos de un `dataset`.

### Objetivos

1. **Analizar y aplicar diferentes técnicas de indexación:**

   - Implementar las técnicas AVL File, Sequential File y Extendible Hashing.
   - Comparar el tiempo y eficiencia de consultas con distintas técnicas.

2. **Implementar de las operaciones de las técnicas de indexación:**

   - Búsqueda específica.
   - Búsqueda por rango.
   - Agregación y eliminación de un registro.

3. **Desarrollar una interfaz gráfica:**
   - Manipulación de la base de datos.
   - Realización de consultas SQL.
   - Facilitar la visualización de resultados.

### Resultados Esperados

1. **Mejoramiento en la Eficiencia de las Consultas**: 
   - Las técnicas de indexación deben reducir el tiempo de respuesta.

2. **Optimización en los Tiempos de Procesamiento**
   - El uso de estructura de datos especializadas debe acelerar la recuperación, inserción y eliminación de registros, incluso en bases de datos con volumne de datos considerables.

3. **Capacidad de Procesamiento Eficiente a Gran Escala**
   - Las técnicas de indexación aplicadas deben poder manejar grandes volúmenes de datos de manera eficiente, evitando cuellos de botella y permitiendo un rendimiento constante en situaciones de alta carga o con archivos de gran tamaño.

### Técnicas de Indexación

Las técnicas implementadas son las siguientes:

   - `AVL`
   - `Sequential File`
   - `Extendible Hashing`

### Funciones Implementadas

Para cada una de las técnicas de indexación se implementaron las siguientes funciones:

   - `Search`
   - `Range Search`
   - `Add`
   - `Remove`

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

**ESTRUCTURA DE CADA REGISTRO**

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

Esta técnica de indexación utiliza un árbol AVL para mantener el índice balanceado de los registros almacenados en un archivo. Cada nodo del árbol contiene una clave y un puntero a la ubicación del registro en el archivo. Esto permite realizar búsquedas, inserciones y eliminaciones de manera eficiente, ya que el árbol AVL garantiza que estas operaciones se realicen en tiempo `O(log n)` manteniendo el equilibrio del árbol mediante rotaciones cuando sea necesario.

#### FUNCIONES EXTERNAS EMPLEADAS

#### Función `getRecord`
   - Lee un registro de una posición específica en el archivo binario y lo devuelve.

   ```cpp
   std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
   RecordType record;
   file.seekg(offset + pos * sizeof(RecordType), std::ios::beg);
   file.read((char*) &record, sizeof(RecordType));
   file.close();
   return record;
   ```

   - Abre el archivo en modo binario para lectura y escritura.
   - Calcula la posición del registro en el archivo: `offset + pos * sizeof(RecordType)`.
   - Lee el contenido de esa posición en la variable `record` de tipo `RecordType`.

#### Función `setRecord`

   - Escribe el contenido del registro en una posición específica del archivo binario. 

   ```cpp
   void setRecord(long pos, RecordType record) {
      std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);

      file.seekp(offset + pos * sizeof(RecordType), std::ios::beg);
      file.write((char*) &record, sizeof(RecordType));
      file.close();
   }
   ```

   - Abre el archivo en modo binario para lectura y escritura.
   - Calcula la posición en el archivo: `offset + pos * sizeof(RecordType)`.
   - Escribe el contenido de `record` en esa posición del archivo.

**SEARCH**

La función `find` está diseñada para la búsqueda de un registro específico mediante su clave en un archivo que implementa un árbol AVL.

#### Descripción del Proceso

#### Función `find` Principal

   ```cpp
   RecordType find(TK key) {
      return find(pos_root, key);
   }
   ```

   - Recibe un parámetro `key`, que es la clave de registro a buscar.

   - Llama a la versión sobrecargada de `find`, que busca recursivamente a partir de la posición `pos_root` (raíz del árbol) utilizand la clave proporcionada.

#### Función `find` Sobrecargada

- Realiza una búsqueda binaria recursiva en el árbol AVL utilizando la clave proporcionada.
   
   ```cpp
   RecordType find(long pos, TK key) {
      if(pos == -1) {
         return RecordType{};
      }
   ```cpp

   - Si `pos` es `-1`, significa que la posición está vacío o no se encontró el registro. Devuelve un objeto `RecordType` por defecto.

   ```cpp
   auto temp = getRecord(pos);
   ```
   - Se utiliza la función `getRecord` para obtener el registro de la posición `pos` en el archivo.

   **Comparación de Claves**

   ```cpp
   if (key < temp.id) {
      return find(temp.left, key);
   } else if (key > temp.id) {
      return find(temp.right, key);
   } else {
      return temp;
   }
   ```

   - Si `key < temp.id`, busca el subárbol izquierdo (`temp.left`).

   - Si `key > temp.id`, busca el subárbol derecho (`temp.right`).

   - Si `key == temp.id`, el registro se encontró y se devuelve (`temp`).

**ADD**

La función `insert` se encarga de insertar un nuevo registro en el árbol AVL. Esta operación involucra actualizar las referencias de los nodos, mantener el balance del árbol y, si es necesario, realizar rotaciones para asegurar que la propiedad del árbol AVL (balanceo) se mantenga.

#### Función `insert` Principal

```cpp
   void insert(RecordType record) {
        insert(pos_root, record);
   }
```

- Llama a la función `insert(long &pos, RecordType record)`, pasando como argumento `pos_root` (posición de la raíz del árbol).

#### Función `insert` Sobrecargada

   1. Verificación de Nodo Nulo

   ```cpp
   void insert(long &pos, RecordType record) {
      if (pos == -1) {
         pos = getSize();
         setRecord(pos, record);
      }
   ```

   - Comprueba si `pos` es igual a `-1`, lo que indica que se trata de un nodo vacío. `pos = getSize()` guarda la posición actual del archivo y `setRecord(pos, record)` guarda el `record` en la posición `pos` del archivo.

   2. Recuperación del Registro Actual

      ```cpp
      else {
         auto temp = getRecord(pos);
      ```

      - Lee el registro en la posición `pos` desde el archivo y lo almacena en `temp`.

   3. Inserción en el Subárbol Izquierdo

      ```cpp
      if (record.id < temp.id) {
         if (temp.left == -1) {
            temp.left = getSize();
            setRecord(temp.left, record);
         } else {
            insert(temp.left, record);
         }
      }
      ```

      - Si `record.id` es menor que `temp.id`:
         1. Si `temp.left == -1`, asigna `temp.left` a la última posición disponible (`getSize()`). Además, se inserta el registro en `temp.left` usando `setRecord()`.

         2. Caso contrario, llama a la función `insert(temp.left, record)` recursivamente para intentar insertar el registro en el subárbol izquierdo.

   4. Inserción en Subárbol Derecho

      ```cpp
      else if (record.id > temp.id) {
         if (temp.right == -1) {
            temp.right = getSize();
            setRecord(temp.right, record);
         } else {
            insert(temp.right, record);
         }
      }
      ```
      
      - De manera similar que en el anterior paso, si `record.id` es mayor que `temp.id`:
         1. Si `temp.right == -1`, asigna `temp.right` a la última posición disponible. Además, inserta el registro en `temp.right` utilizando la función `setRecord()`.
         2. Caso contrario, llama a la función `insert(temp.right, record)` recursivamente para intentar isnertar el registro en el subárbol derecho.

   5. Actualización

      ```cpp
      updateHeight(temp);
      balance(pos);
      setRecord(pos, temp);
      }
      ```

      - Se llama a la función `updateHeight()` para actualizar la altura el nodo actual (`temp`) en función de las alturas de sus hijos izquierdos y derechos.

      - Se llama a la función `balance()` para verificar y corregir el balance del subárbol en la posición `pos`. Esta función puede realizar rotaciones si el balance del árbol AVL es alterado.

      - Se guarda el nodo `temp` actualizado en la posición `pos` del archivo con la función `setRecord()`


**REMOVE**

La función `remove` se encarga de eliminar un nodo (registro) con una clave (`key`) específica del árbol. 

#### Función `remove` Principal

   ```cpp
   bool remove(TK key) {
         return remove(pos_root, key);
   }
   ```

   - La función principal llama a una versión recursiva intera del `remove` con la raíz(`pos_root`) como posición inicial.

#### Función `remove` Sobrecargada

   1. Verificar si el Árbol está Vacío

      ```cpp
      if (pos == -1) return false;
      ```
      - Si el `pos` es `-1`, significa que el nodo actual no existe, por lo que no hay nada que eliminar.

   2. Obtener el Registro Actual en la Posición `pos`

      ```cpp
      RecordType record = find(key);
      ```

      - Utiliza la función `find` para obtener el registro (nodo) con la clave `key` que deseamos eliminar. Si `find` no encuentra el registro, la ejecución no debería de ocurrir.

   3. Caso 1: Nodo con Cero o Un Hijo

      ```cpp
      if(record.left == -1 | record.right == -1) {
                  long tempPos = (record.left != -1) ? record.left : record.right;
      ```

      - Si el nodo tiene 0 o un solo hijo, se almacena la posición de ese hijo en `tempPos`

      - La condición en `tempPos` se usa para decidir cuál hijo (izquierdo o derecho) se va a usar para reemplazar al nodo eliminado.

      #### Caso 1a: Nodo Hoja (Sin Hijos):

      ```cpp
      if (tempPos == -1) {
         pos = -1;
      }
      ```
      - Si se cumple, el nodo no tiene hijos (es un nodo hoja). Simplemente se elimina asignando `pos = -1`.

      #### Caso 1b: Nodo con Hijo:

      ```cpp
      else {
         RecordType temp = getRecord(pos);
         record = temp;
         pos = tempPos;
      }
      ```
      - Caso contrario, el nodo tiene un solo hijo. Entonces se reemplaza el nodo actual por su hijo (`pos = tempPos`).

   4. Caso 2: Nodo con Dos Hijos

      ```cpp
      else {
         long nextPos = getMinValueRecord(record.right);
         RecordType nextNode = getRecord(nextPos);

         record.id = nextNode.id;
         remove(record.right, nextNode.id);
      }
      ```
      - Si el nodo tiene dos hijos, se encuentra un sucesor en el subárbol derecho.
      - La clave del sucesor (`nextNode.id`) se usa para reemplazar la clave del nodo que estamos eliminando (`record.id = nextNode.id`).
      - Luego, se llama recursivamente a `remove` para eliminar el sucesor del subárbol derecho.

   5. Actualizar y Balancear el Árbol

      ```cpp
      if(pos != -1) {
         updateHeight(record);
         balance(pos);
         setRecord(pos, record);
      }
      ```

      - Después de eliminar el nodo, si `pos != -1`, se actualiza la altura del nodo actual y se balancea el subárbol para restaurar las propiedades de balanceo del AVL.
      - Finalmente se guarda el nodo actualizado en el archivo con la función `SetRecord()`.


### Sequential File

Esta técnica de indexación se utiliza principalmente en archivos donde los registros se organizan en un orden lineal y continuo. Cada registro se almacena uno después de otro, de manera que se puede acceder a ellos en la secuencia en la que se escribieron. Este formato es comúnmente utilizado para aplicaciones donde se requiere una acceso secuencial a los datos.

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

**ADD**

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
      
      - Se abre el archivo auxiliar y se agrega el nuevo registro al final del archivo.


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

**SEARCH**

**RANGE SEARCH**

**ADD**

**REMOVE**

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

