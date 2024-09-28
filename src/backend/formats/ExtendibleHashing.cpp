#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <cmath>

#define MAX_DEPTH 3   // Profundidad máxima del directorio
#define MAX_FILL 4    // Capacidad máxima de cada bucket

using namespace std;

template <typename TK, typename RecordType>
class ExtendibleHashing {
private:
    string indexFilename;  // Nombre del archivo de índice
    string dataFilename;   // Nombre del archivo de datos

public:
    // Constructor que recibe el nombre base para los archivos
    ExtendibleHashing(const string &filename)
    {
        // Asignar nombres de archivos
        indexFilename = "indx_" + filename;
        dataFilename = filename;

        // Crear el archivo de índice
        ofstream outFile(indexFilename, ios::binary | ios::trunc);
        if (outFile)
        {
            for (int i = 0; i < (1 << MAX_DEPTH); ++i)
            {
                outFile.write(reinterpret_cast<const char *>(&i), sizeof(i));
                int bucketIndex = i % 2;
                outFile.write(reinterpret_cast<const char *>(&bucketIndex), sizeof(bucketIndex));
            }
            outFile.close();
            cout << "Index guardado en " << indexFilename << "." << endl;
        }
        else
        {
            cout << "Error al abrir " << indexFilename << "." << endl;
        }
    }

    // Función de hash
    int hashFunction(TK key) const
    {
        return static_cast<int>(key) % (1 << MAX_DEPTH);
    }

    // Función para insertar registros
    void insert(const RecordType &record)
    {
        int hashValue = hashFunction(record.id);
        int bucketIndex = getBucketIndex(hashValue);

        // Si no existe el registro, lo insertamos
        if (!find(record.id).id)
        {
            ofstream dataFile(dataFilename, ios::binary | ios::app);
            if (dataFile)
            {
                dataFile.write(reinterpret_cast<const char *>(&record), sizeof(RecordType));
                dataFile.close();
                cout << "Registro insertado: " << record.id << endl;
            }
            else
            {
                cout << "Error al abrir " << dataFilename << " para escribir." << endl;
            }
        }
        else
        {
            cout << "El registro con código " << record.id << " ya existe." << endl;
        }
    }

    // Función para buscar registros por código
    RecordType find(TK codi)
    {
        ifstream dataFile(dataFilename, ios::binary | ios::in);  // Asegurarse de abrir en modo lectura
        RecordType record;
        if (dataFile)
        {
            while (dataFile.read(reinterpret_cast<char *>(&record), sizeof(RecordType)))
            {
                if (record.id == codi)
                {
                    dataFile.close();
                    return record;
                }
            }
            dataFile.close();
        }
        else
        {
            cout << "Error al abrir " << dataFilename << " para leer." << endl;
        }
        return RecordType();
    }

    // Función para eliminar un registro
    void remove(TK codigo)
    {
        fstream dataFile(dataFilename, ios::binary | ios::in | ios::out);
        if (dataFile)
        {
            RecordType record;
            streampos pos;
            while (dataFile.read(reinterpret_cast<char *>(&record), sizeof(RecordType)))
            {
                if (record.id == codigo)
                {
                    pos = dataFile.tellg() - static_cast<streampos>(sizeof(RecordType));
                    dataFile.seekp(pos);
                    RecordType emptyRecord;
                    dataFile.write(reinterpret_cast<const char *>(&emptyRecord), sizeof(RecordType));
                    cout << "Registro con código " << codigo << " eliminado." << endl;
                    dataFile.close();
                    return;
                }
            }
            cout << "Registro con código " << codigo << " no encontrado." << endl;
            dataFile.close();
        }
        else
        {
            cout << "Error al abrir " << dataFilename << " para eliminar." << endl;
        }
    }

    // Función para encontrar registros en un rango
    vector<RecordType> findRange(TK lower, TK upper)
    {
        vector<RecordType> results;
        ifstream dataFile(dataFilename, ios::binary | ios::in);
        RecordType record;
        if (dataFile)
        {
            while (dataFile.read(reinterpret_cast<char *>(&record), sizeof(RecordType)))
            {
                if (record.id >= lower && record.id <= upper)
                {
                    results.push_back(record);
                }
            }
            dataFile.close();
        }
        else
        {
            cout << "Error al abrir " << dataFilename << " para leer." << endl;
        }
        return results;
    }

private:
    // Función para obtener el índice del bucket desde el archivo de índice
    int getBucketIndex(TK key)
    {
        ifstream inFile(indexFilename, ios::binary | ios::in);  // Asegurarse de abrir en modo lectura
        if (inFile)
        {
            int index = hashFunction(key);
            int bucketIndex;
            inFile.seekg(index * (sizeof(int) + sizeof(int)) + sizeof(int));
            inFile.read(reinterpret_cast<char *>(&bucketIndex), sizeof(bucketIndex));
            inFile.close();
            return bucketIndex;
        }
        return -1;
    }

    // Función para buscar por nombre de archivo (útil si tienes múltiples tablas)
    friend int buscarExtendibleHashing(const std::vector<ExtendibleHashing<TK, RecordType>> &sqfS, const std::string &nombreBuscado)
    {
        for (int i = 0; i < sqfS.size(); ++i)
        {
            if (sqfS[i].dataFilename == nombreBuscado)
            {
                return i;
            }
        }
        return -1;
    }
};

