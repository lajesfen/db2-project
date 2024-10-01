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

#### Función `updateHeight`

   ```cpp
   void updateHeight(RecordType &record) {
      int leftHeight = getHeight(record.left);
      int rightHeight = getHeight(record.right);
      record.height = std::max(leftHeight, rightHeight);
   }
   ```

   - `leftHeight`: Almacena la altura del subárbol izquierdo del nodo actual.

   - `rightHeight`: Almacena la altura del subárbol derecho del nodo actual.

   - La función auxiliar `getHeight` se utiliza para obtener la altura de un subárbol dado. Retorna -1 si el subparbol no existe (es decir, si el nodo hijo es `-1`).

   - La nueva altura del nodo actual se calcula como el máximo entre la altura del subárbol izquierdo y derecho, más 1. Esto se debe a que la altura de un nodo es la mayor altura de sus hijos, más 1 para icnluir al propio nodo.

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

**RANGE SEARCH**
La función `searchByRange` encuentra todos los registros que se encuentran dentro de un rango específico de claves.

#### Función `searchByRange` Principal

   ```cpp
   std::vector<RecordType> searchByRange(TK lower, TK upper)
   {
      std::vector<RecordType> results;
      searchByRange(pos_root, lower, upper, results);
      return results;
   }
   ```


   - El vector results almacenará los registros que caen dentro del rango.

   - Se llama a la función privada `searchByRange` con los parámetros `pos_root`(posición del nodo raíz), `lower`(límite inferior) y `upper`(límite superior), y el vector `results` para almacenar los resultados.

#### Función `searchByRange` Recursiva

   1. Verificación de la Posición

      ```cpp
      if (pos == -1)
         return;
      ```

      - Comprueba si `pos` es igual a `-1`. Este valor indica que hemos llegado a un nodo vacío o no existente.

      - Si es `-1`, la función retorna inmediatamente sin realizar ninguna acción. Esto evita intentos de acceder a registros que no existen.

   2. Obtener el Registro Actual

      ```cpp
      RecordType current = getRecord(pos);
      ```

      - Llama a la función `getRecord(pos)` para obtener el registro correspondiente a la posición actual en el árbol AVL. Se utilizará  para comparar su `id` con los límites del rango.

   3. Búsqueda en el Subárbol Izquierdo

      ```cpp
      if (current.id >= lower)
         searchByRange(current.left, lower, upper, results);
      ```

      - Verifica si el `id` del registro actual (`current.id`) es mayor o igual que el límite inferior.

      - Si se cumple, se llama recursivamente a `searchByRange`, pasando el hijo izquierdo, ya que este subárbol puede contener nodos que también caen dentro del rango.

   4. Agregar Registro a Resultados

      ```cpp
      if (current.id >= lower && current.id <= upper)
         results.push_back(current);
      ```

      - Comprueba si el `id` del registro actual está dentro del rango especificado.
      - Si el `id` está en el rango, se agrega `current` al vector `results`.

   5. Búsqueda en el Subárbol Derecho

      ```cpp
      if (current.id <= upper)
         searchByRange(current.right, lower, upper, results);
      ```

      - Verifica si el `id` del registro actual es menor o igual que el límite superior.
      - Si es cierto, se llama recursivamene a `searchByRange` en el hijo derecho son mayores que el nodo actual, lo que significa que podrían estar dentro del rango.

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
      if (pos == -1) 
         return false;
      ```

      
      - Si el `pos` es `-1`, significa que el nodo actual no existe, por lo que no hay nada que eliminar.

   2. Obtener el Registro Actual en la Posición `pos`

      ```cpp
      RecordType record = getRecord(pos);
      ```

      - Utiliza la función `getRecord` para obtener el registro que se encuentra en la posición `pos`.

   3. Decicidir el Camino de Búsqueda
   
      ```
      if (key < record.id)
      {
         if (!remove(record.left, key))
            return false;
      }
      else if (key > record.id)
      {
         if (!remove(record.right, key))
            return false;
      }
      else
      {
      ```

      - Si `key < record.id`: Se realiza unan llamada recursiva para buscar y eliminar el registro en el subárbol izquierdo.

      - Si `key > record.id`: Se realiza una llamada recursiva para buscar y eliminar el registro en el subárbol derecho.

      - Caso contrario, se procede con la lógica de eliminación 

   3. Caso 1: Nodo con Cero o Un Hijo

      ```cpp
      if (record.left == -1 || record.right == -1)
      {
        long tempPos = (record.left != -1) ? record.left : record.right;

        if (tempPos == -1)
        {
          pos = -1;
        }
        else
        {
          pos = tempPos;
        }
      }

      ```

      - Se verifica si el nodo tiene un hijo izquierdo o un hijo derecho igual a `-1`. Esto indica que el nodo tiene uno o ningún hijo.

      - Caso sin hijos: Si `tempPos` es igual a `-1`, el nodo no tiene hijos, por lo que se asigna `pos` a `-1`, eliminando así el nodo.

      - Si hay un hijo: Se asigna `pos` a `tempPos`, enlazando el padre al único hijo y eliminando el nodo actual.

   4. Caso 2: Nodo con Dos Hijos

      ```cpp
      else
      {
        long nextPos = getMinValueRecord(record.right);
        RecordType nextNode = getRecord(nextPos);

        record.id = nextNode.id;

        if (!remove(record.right, nextNode.id))
           return false;
      }
      ```

      - Encontrar el nodo mínimo: Se llama a la  función `getMinValueRecord(record.right)` para encontrar la posició´n del nodo más pequeño en el subárbol derecho al nodo actual.

      - Reemplazar el valor del nodo: se obtiene el nodo más pequeño y se copia el `id` del nodo más pequeño que se desea eliminar.

      - Eliminar el nodo mínimo: Se llama a la función `remove` para eliminar el nodo más pequeño en el subárbol derecho, asegurando que el árbol permanezca equilibrado y que no haya dplicados.

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

**Función Auxiliar**

1. Apertura del archivo

   ```cpp
   std::ifstream file(filename, std::ios::binary);
   if (!file.is_open())
      return false;
   ```

   - Si no se puede abrir, retorna `false` (búsqueda fallida).

2. Determinación del tamaño del archivo y número de registros

   ```cpp
   file.seekg(0, std::ios::end);
   int fileSize = file.tellg();
   int recordCount = fileSize / sizeof(Registro);
   ```
   - Calcula el temaño total del archivo y el número de registros en el archivo.

3. Inicialización de punteros para la búsqueda binaria

   ```cpp
   int left = 0;
   int right = recordCount - 1;
   ```
   - Inicializamos los punteros `left` y `right` que representan los límites de la búsqueda binaria.

4. Realización de la búsqueda binaria

   ```cpp
   while (left <= right)
   {
      int mid = left + (right - left) / 2;

      file.seekg(mid * sizeof(Registro), std::ios::beg);
      file.read(reinterpret_cast<char *>(&result), sizeof(Registro));

      if (result.id == key)
      {
         file.close();
         return true;
      }
      else if (result.id < key)
      {
         left = mid + 1;
      }
      else
      {
         right = mid - 1;
      }
   }
   ```

   - Mientras `left` sea menor o iual a `right`, secalcula el punto medio `mid`.

   - Se posiciona el puntero del archivo(`seekg`) e el registro del medio y se lee el registro

   - Si el `id` delr egistro leído es igual a la `key`, se cierra el archivo y se retorna `true`.

   - Caso contrario, si es menor que la `key`, se actualiza `left` para buscar en la mitad superior.

   - Caso Contrario, se actualiza el `right` para buscar la mitad inferior.

**Función Principal**

   1. Búsqueda en el archivo principal y auxiliar

      ```cpp
      Registro search(TK key)
      {
         Registro reg;
         if (searchInFile(filename, key, reg))
         {
               return reg;
         }
         if (searchInFile(aux_filename, key, reg))
         {
               return reg;
         }
      ```

      - Se llama a la función auxiliar `searchInFile` para buscar el registro en el archivo principal. Si se encuentra retorna el registro inmediatamente.

      - De igual manera se llama a la función para buscar en el archivo auxiliar, si se encuentra retorna el registro.

   2. Manejo de registro no encontrado

      ```cpp
      Registro failReg;
      failReg.id = -1;
      return failReg;
      }
      ```

      - Si no se encuentra el registro en ninguno de los dos archivos, se crea un registro `failReg`, el valor -1 indica que no se encontró el registro.

**RANGE SEARCH**

La función `rangeSearch` está diseñada para la búsqueda de registros por rango. La función recibirá los límites inferior (`begin_key`) y superior (`begin_key`) del rango de búsqueda.

#### Descripción del Proceso

**Función Axiliar**

   1. Apertura de Archivo

      ```cpp
      std::ifstream file(filename, std::ios::binary);
      if (!file.is_open())
         return false;
      ```

      - Se abre el archivo en modo lectura binaria.

      - Si no se puede abrir el archivo, la función retorna `false` inmediatamente.

   2. Lectura de Todos los Registros

      - Este bucle se ejecuta mientras se pueda leer un registro completo del archivo. Se lee cada registro en la variable `reg`.
 
   
      ```cpp
      while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
      ```

   3. Comparación de Claves

      - Dentro del bucle, se verifica si la clave del registro leído (`reg.key`) está dentro del rango definido por `begin_key` y `end_key`.

      - Si la clave está dentro del rango, se agrega el registro al vector `results`.


      ```cpp
      if (reg.id >= begin_key && reg.id <= end_key)
      {
         results.push_back(reg);
      }
      ```

**Función Principal**

   ```cpp
   std::vector<Registro> rangeSearch(TK begin_key, TK end_key){
      std::vector<Registro> results;
      readRecordsInRange(filename, begin_key, end_key, results);
      readRecordsInRange(aux_filename, begin_key, end_key, results);
      return results;
   }
   ```

   - Se crea un vector `results` para almacenar los registros que se encuentren dentro del rango especificado.

   - Se le llama a la función auxiliar `readRecordsInRange` para buscar registros en el archivo principal y auxiliar.

   - Todos los registros que se encuuentren en el rango se añadirán al vector. 

**ADD**

**Función `readAllRecords`**

   ```cpp
   void readAllRecords(const std::string &filename, std::vector<Registro> &allRecords)
   {
      Registro reg;
      std::ifstream file(filename, std::ios::binary);
      if (!file.is_open())
         return;

      while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
      {
         allRecords.push_back(reg);
      }
      file.close();
   }
   ```

   - Se abre el archivo, y verifica si es que se abrió correctamente.

   - Se utiliza un bucle para leer todos los registros del archivo. Cada vez que se le un registro, se añade al vector `allRecors`.

**Función `Merge`**
Esta función se encarga de fusionar los registros almacenados en el archivo auxiliar y el archivo principal en un único archivo principal ordenado. Se llama cuando el número de registros en el archivo auxiliar alcanza un límite (`MAX_AUX_RECORD`).

   1. Lectura del Archivo Principal y Auxiliar

      - Se llama a la función `readAllRecord` para leer todos los registros del archivo principal y el archivo auxiliar, y se almacenan en el vector `allRecords`.

      ```cpp
      std::vector<Registro> allRecords;
      readAllRecords(filename, allRecords);
      readAllRecords(aux_filename, allRecords);
      ```

   2. Ordenamiento de los Registros

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

   5. Limpieza del Archivo Auxiliar

      - Se trunca el archivo auxiliar, dejandolo vacío después de que se han fusionado sus registros en el archivo principal.


      ```cpp
      std::ofstream auxFile(aux_filename, std::ios::binary |std::ios::trunc);
      auxFile.close();
      ```

**Función `getAuxRecordCount**
Este método se encarga de contar el número de registros que hay en el archivo auxiliar (`aux_filenae`) y devuelve este conteo.

   - Se abre el archivo auxiliar en modo lectura para leer los datos del archivo sin alterar su contenido.

   - Se utiliza la función `seekg` para mover el puntero de lectura  al final del archivo.

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

      - Se llama a la función `search` para verificar si ya existe un registro con el mismo `id` en el archivo principal o auxiliar.

      - Si se encuentra, la función retorna nada.


      ```cpp
      void add(Registro registro)
      {
        Registro existingRecord = search(registro.id);
        if (existingRecord.id == registro.id)
        {
            return;
        }
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

**Función Auxiliar**

1. Apertura del Archivo

   ```cpp
   bool removeFromFile(const std::string &filename, TK key)
   {
      bool found = false;
      Registro reg;

      std::ifstream file(filename, std::ios::binary);
      if (!file.is_open())
         return false;
   ```

   - Se declara una variable booleana `found` que se inicializa en `false`. Esta variable se utilizará para indicar si el registro fue encontrado y eliminado.

   - Se abre el archivo, en caso no pueda retornará `false`.

2. Lectura de Registros

   ```cpp
   std::vector<Registro> records;
   while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
   {
      records.push_back(reg);
   }
   ```

   - Se declara un vector `records` para almacenar todos los registros leídos del archivo.

   - Se utiliza el bucle `while` para leer los registros del archivo, almacenando cada uno en el vector `records`.

3. Búsqueda y Eliminación del Registro

   ```cpp
   auto it = std::lower_bound(records.begin(), records.end(), key, [](const Registro &r, TK key)
                              { return r.id < key; });

   if (it != records.end() && it->id == key)
   {
      found = true;
      records.erase(it);
   }
   ```

   - La función `lower_bound` se utiliza para encontrar la posición del registro en el vector que podría contener la clave buscada. Este método requiere que los registros estén ordenados. Devuelve un iterador quee apunta al primer elemento que no es menor que `key`.

   - Se verifica si el iterador no ha alcanzado el final del vector y lo elimina.

4. Escritura de registros actualizados en el archivo

   ```cpp
   if (found)
   {
      std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
      for (const auto &r : records)
      {
         outFile.write(reinterpret_cast<const char *>(&r), sizeof(Registro));
      }
      outFile.close();
   }
   ```

   - Si el registro fue encontrado y eliminado, se trunca (vacía) el archivo.

   - Se recorren todos los registros restantes en el vector `records` y se escriben de nuevo en el archivo.

**Función Principal**

   ```cpp
   bool remove(TK key)
   {
      if (removeFromFile(filename, key))
      {
         return true;
      }
      return removeFromFile(aux_filename, key);
   }
   ```

   - La función llama a `removeFromFile`, si retorna `true`, significa que el registro fue encontrado y eliminado.

   - Si la eliminación no se realizó, la función intentará eliminar el registro del archivo auxiliar.

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

La función `search` en Extendible Hashing se encarga de buscar un registro específico utilizando la clave de dicho registro. El proceso se basa en calcular el valor de hash de la clave y luego usar este valor para identificar el bucket correspondiente donde puede estar almacenado el registro. Una vez identificado el bucket, la función lee los registros en el archivo de datos, recupera y verifica si alguno de ellos coincide con la clave dada.

```cpp
RecordType find(TK codi) {
    std::ifstream dataFile(dataFilename, std::ios::binary | std::ios::in);
    RecordType record;
    if (dataFile) {
        while (dataFile.read(reinterpret_cast<char *>(&record), sizeof(RecordType))) {
            if (record.key == codi) {
                dataFile.close();
                return record;
            }
        }
        dataFile.close();
    } else {
        std::cout << "Error al abrir " << dataFilename << " para leer." << std::endl;
    }
    return RecordType();
}

```
- Se aplica la función de hash a la clave para obtener el índice del bucket.
- Se accede al archivo de datos para leer los registros dentro del bucket correspondiente.
-  Si el registro con la clave proporcionada es encontrado, se devuelve dicho registro.
-  i no se encuentra, la función devuelve un registro vacío y genera una excepción de "Registro no encontrado".

#### Range Search
La función `rangeSearch` está diseñada para buscar registros dentro de un rango de claves específicas. Similar al proceso de búsqueda específica, se utiliza la función de hash para determinar los buckets donde podrían estar los registros. Sin embargo, esta vez se buscan múltiples claves en un rango, y se devuelven todos los registros que caigan dentro del intervalo especificado.


```cpp
std::vector<RecordType> findRange(TK lower, TK upper) {
    std::vector<RecordType> results;
    std::ifstream dataFile(dataFilename, std::ios::binary | std::ios::in);
    RecordType record;
    if (dataFile) {
        while (dataFile.read(reinterpret_cast<char *>(&record), sizeof(RecordType))) {
            if (record.key >= lower && record.key <= upper) {
                results.push_back(record);
            }
        }
        dataFile.close();
    } else {
        std::cout << "Error al abrir " << dataFilename << " para leer." << std::endl;
    }
    return results;
}
```


-  Se calcula la función de hash para los valores límite del rango.
- Se accede al archivo de datos y se leen los registros.
- Para cada registro, se verifica si su clave cae dentro del rango.
- Se devuelven todos los registros que cumplen con los criterios de rango.


#### Add


La función `add` se encarga de insertar un nuevo registro en el archivo de datos utilizando el método de hashing. Si el bucket donde debería ir el registro está lleno, la función divide el bucket (`split`), creando un nuevo bucket y redistribuyendo los registros.

```cpp
void insert(const RecordType &record) {
    int hashValue = hashFunction(record.key);
    int bucketIndex = getBucketIndex(hashValue);

    if (!find(record.key).key) {
        std::ofstream dataFile(dataFilename, std::ios::binary | std::ios::app);
        if (dataFile) {
            dataFile.write(reinterpret_cast<const char *>(&record), sizeof(RecordType));
            dataFile.close();
            std::cout << "Registro insertado: " << record.key << std::endl;
        } else {
            std::cout << "Error al abrir " << dataFilename << " para escribir." << std::endl;
        }
    } else {
        std::cout << "El registro con código " << record.key << " ya existe." << std::endl;
    }
}

```

- Se aplica la función de hash para calcular el índice del bucket donde se insertará el nuevo registro.
- Se verifica si el registro ya existe en el bucket:
-- Si ya existe, se cancela la operación de inserción.
-- Si no existe, el registro se añade al archivo de datos.
- Si el bucket está lleno, se procede con la operación de split, lo que implica:
-- Duplicar el directorio de buckets y asignar nuevos índices a los buckets.
-- Redistribuir los registros entre los buckets existentes y el nuevo bucket creado.
-- Actualizar el archivo de índice para reflejar los cambios.
- Finalmente, el registro es insertado en el bucket correspondiente.

#### Remove

La función `remove` se utiliza para eliminar un registro específico del archivo de datos. Similar al proceso de búsqueda, se aplica la función de hash para localizar el bucket donde está almacenado el registro. Luego, se sobrescribe el registro con un registro vacío para marcarlo como eliminado.

```cpp
void remove(TK key) {
    std::fstream dataFile(dataFilename, std::ios::binary | std::ios::in | std::ios::out);
    if (dataFile) {
        RecordType record;
        std::streampos pos;
        while (dataFile.read(reinterpret_cast<char *>(&record), sizeof(RecordType))) {
            if (record.key == key) {
                pos = dataFile.tellg() - static_cast<std::streampos>(sizeof(RecordType));
                dataFile.seekp(pos);
                RecordType emptyRecord;
                dataFile.write(reinterpret_cast<const char *>(&emptyRecord), sizeof(RecordType));
                std::cout << "Registro con código " << key << " eliminado." << std::endl;
                dataFile.close();
                return;
            }
        }
        std::cout << "Registro con código " << key << " no encontrado." << std::endl;
        dataFile.close();
    } else {
        std::cout << "Error al abrir " << dataFilename << " para eliminar." << std::endl;
    }
}
```

- Se aplica la función de hash a la clave para obtener el índice del bucket.
- Se accede al archivo de datos y se busca el registro correspondiente en el bucket.
- Si se encuentra el registro, este es sobrescrito por un registro vacío.
- Se actualiza el archivo de datos para reflejar los cambios.


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
### 1. Tokenización

La función `tokenize` toma una consulta SQL como una cadena de texto y la divide en un vector de tokens (palabras clave). Estos tokens son luego interpretados por el parser para identificar las acciones a realizar, como:

- Crear una tabla
  
```cpp
if (tokens[0] == "create" && tokens[1] == "table") {
   string nombre_archivo = tokens[2];
   string tipo_dato = tokens[5];
…
}
```

- Insertar registros
  
```cpp
else if (tokens[0] == "insert" && tokens[1] == "into") {
   string nombre = tokens[2];
   int indexAVL = -1;
   int indexSequential = -1;
   int indexHash = -1;
…}
```
- Seleccionar datos
  
```cpp
else if (tokens[0] == "select" && tokens[6] == "=") {
   string nombreTabla = tokens[3];
   int key = stoi(tokens[7]);
…}

else if (tokens[0] == "select" && tokens[6] == "between") {
   string nombreTabla = tokens[3];
   int keya = stoi(tokens[7]);
   int keyb = stoi(tokens[9])

```
- Eliminar registros

```cpp
else if (tokens[0] == "delete" && tokens[1] == "from" && tokens[5] == "=") {
   string nombreTabla = tokens[2];
   int key = stoi(tokens[6]);
…}
```
### Ejemplo de Tokenización:

Consulta SQL de ejemplo:

```sql
create TABLE hospitales (id, nombre, direccion);
```

La función `tokenize` convertirá esta consulta en un vector de tokens como:

```cpp
vector<string> tokens = tokenize("create table hospitales from file hospitalesopendata using AVL");
// Resultado: {"create", "table", "hospitales", "from",”file”,”hospitalesopendata”,””using”,”AVL”}
```

### Código:
```cpp
vector<string> tokenize(const string& query) {
   stringstream ss(query);
   string token;
   vector<string> tokens;

   while (ss >> token) {
       tokens.push_back(token);
   }
   return tokens;
}
```

## 2. Extracción de Registros

La función `extraerRegistros` toma la porción de la consulta SQL que contiene los valores a insertar, los tokeniza y organiza en un vector de cadenas. Esto permite que los valores se conviertan en registros completos, como `HospitalRecord` o `SocialRecord`.

### Ejemplo de Extracción de Registros:

Consulta SQL de ejemplo:
```sql
insert into hospitales values (1,Hospital Central,Av. Siempre Viva);
```

La función extraerRegistros toma una cadena con valores entre paréntesis y los convierte en un vector de vectores de cadenas. Para cada conjunto de valores entre paréntesis, los separa por comas y organiza cada valor en un vector. Al final, devuelve un vector de registros completos. Además si hay varios parentesis devolvera los valores de cada uno de estos en un vector dentro de una matriz de vectores:
```cpp
std::vector<std::vector<std::string>> registros = extraerRegistros("(1,Hospital Central,Av. Siempre Viva)");
// Resultado: {{"1", "Hospital Central", "Av. Siempre Viva"}}
```

### Código:
```cpp
std::vector<std::vector<std::string>> extraerRegistros(const std::string& texto) {
   std::vector<std::vector<std::string>> todos_los_registros;

   size_t inicio = 0;
   while ((inicio = texto.find('(', inicio)) != std::string::npos) {
       inicio += 1;
       size_t fin = texto.find(')', inicio);

       if (fin != std::string::npos) {
           std::string contenido = texto.substr(inicio, fin - inicio);

           size_t pos = 0;
           std::vector<std::string> registro_actual;
           while ((pos = contenido.find(',')) != std::string::npos) {
               std::string palabra = contenido.substr(0, pos);
               registro_actual.push_back(trim(palabra));
               contenido.erase(0, pos + 1);
           }
           registro_actual.push_back(trim(contenido));

           todos_los_registros.push_back(registro_actual);
           inicio = fin + 1;
       }
   }

   return todos_los_registros;
}
```

### 3. Metadata

La clase `MetadataManager` se encarga de gestionar los metadatos que describen los tipos de archivos (AVL, Secuenciales, Hash) y los tipos de registros que almacenan (como `hospitalesopendata` y `directorioredes`). Estos metadatos se almacenan en un archivo y se cargan en memoria para ser fácilmente accesibles por la clase `SQLParser`.

**Gestión de Metadatos:**
Cada vez que se crea una nueva tabla con estructuras de datos específicas, los metadatos correspondientes se guardan para asegurar su recuperación en el futuro sin pérdida de información.

### Ejemplo de MetadataManager:
```cpp
SQLParser() : metadataManager("metadata.txt") {
   const auto& metadata = metadataManager.getMetadata();
   // Los metadatos ahora están accesibles para gestionar las estructuras de datos
}
```

Cuando se crea una tabla usando el comando SQL `CREATE TABLE`, el sistema guarda los metadatos correspondientes a traves de `saveMetadata`, lo que permite al parser SQL recuperar y manejar las estructuras de datos adecuadas de manera eficiente. 

```cpp
if (tokens[0] == "create" && tokens[1] == "table") {
   string nombre_archivo = tokens[2];
   string tipo_dato = tokens[5];
   if (tokens[7] == "AVL") {
       if (tipo_dato == "hospitalesopendata") {
           AVLFile<int, HospitalRecord> avlH(nombre_archivo + ".dat");
           avlHospital.push_back(avlH);

           metadataManager.saveMetadata(nombre_archivo + ".dat", "AVL", tipo_dato);
           std::cout << "Tabla AVL creada con Registro tipo: " << tipo_dato << std::endl;
       }
   }
}
```


## Experimentación

### Cuadro Comparativo de Técnicas de Indexación
![image](https://github.com/user-attachments/assets/ba2bcf5c-39c4-434b-a843-23980fe0c843)
![image](https://github.com/user-attachments/assets/0b859e9b-7cd5-47e1-802b-db0034727d5b)
![image](https://github.com/user-attachments/assets/928998a7-52a3-48f6-80f6-5d68c94e4201)
![image](https://github.com/user-attachments/assets/28871046-dbc6-49d8-b6cd-932f4246d65a)
![image](https://github.com/user-attachments/assets/30944026-79fc-4c59-bca5-5be4c762039e)