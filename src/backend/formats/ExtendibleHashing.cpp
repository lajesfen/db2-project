#include <iostream>
#include <fstream>
#include <bitset>
#include <cstring>
#include <vector>
#include <cmath>

#define MAX_DEPTH 3 
#define MAX_FILL 4  

using namespace std;

struct RecordType {
    int codigo;
    char nombre[50];
    char descripcion[100];
    int cantidad;

    RecordType() : codigo(0), cantidad(0) {
        strcpy(nombre, "");
        strcpy(descripcion, "");
    }

    RecordType(int cod, const char* nom, const char* desc, int cant) : codigo(cod), cantidad(cant) {
        strncpy(nombre, nom, sizeof(nombre));
        strncpy(descripcion, desc, sizeof(descripcion));
    }
};

class ExtendibleHashing {
public:
    ExtendibleHashing() {
        ofstream outFile("index.dat", ios::binary | ios::trunc);
        if (outFile) {
            for (int i = 0; i < (1 << MAX_DEPTH); ++i) {
                outFile.write(reinterpret_cast<const char *>(&i), sizeof(i));
                int bucketIndex = i % 2; 
                outFile.write(reinterpret_cast<const char *>(&bucketIndex), sizeof(bucketIndex));
            }
            outFile.close();
            cout << "Index guardado en index.dat." << endl;
        } else {
            cout << "Error al abrir index.dat." << endl;
        }
    }

    int hashFunction(int key) const {
        return key % (1 << MAX_DEPTH);
    }

    void insert(const RecordType &record) {
        int hashValue = hashFunction(record.codigo);
        int bucketIndex = getBucketIndex(hashValue);

        if (!find(record.codigo).codigo) {  
            ofstream dataFile("datafile.dat", ios::binary | ios::app); 
            if (dataFile) {
                dataFile.write(reinterpret_cast<const char *>(&record), sizeof(RecordType));
                dataFile.close();
                cout << "Registro insertado: " << record.codigo << endl;
            } else {
                cout << "Error al abrir datafile.dat para escribir." << endl;
            }
        } else {
            cout << "El registro con código " << record.codigo << " ya existe." << endl;
        }
    }

    RecordType find(int codi) {
        ifstream dataFile("datafile.dat", ios::binary);
        RecordType record;
        if (dataFile) {
            while (dataFile.read(reinterpret_cast<char *>(&record), sizeof(RecordType))) {
                if (record.codigo == codi) {
                    dataFile.close();
                    return record;
                }
            }
            dataFile.close();
        } else {
            cout << "Error al abrir datafile.dat para leer." << endl;
        }
        return RecordType();  
    }

    void remove(int codigo) {
        fstream dataFile("datafile.dat", ios::binary | ios::in | ios::out);
        if (dataFile) {
            RecordType record;
            streampos pos;
            while (dataFile.read(reinterpret_cast<char *>(&record), sizeof(RecordType))) {
                if (record.codigo == codigo) {
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
        } else {
            cout << "Error al abrir datafile.dat para eliminar." << endl;
        }
    }

    void displayDatafile() const {
        ifstream dataFile("datafile.dat", ios::binary);
        if (dataFile) {
            cout << "Contenido de datafile.dat:\n";
            RecordType record;
            while (dataFile.read(reinterpret_cast<char *>(&record), sizeof(RecordType))) {
                if (record.codigo != 0) { 
                    cout << "Código: " << record.codigo << ", Nombre: " << record.nombre
                         << ", Descripción: " << record.descripcion << ", Cantidad: " << record.cantidad << endl;
                }
            }
            dataFile.close();
        } else {
            cout << "Error al abrir datafile.dat para mostrar." << endl;
        }
    }

    void displayIndexDat() const {
        ifstream inFile("index.dat", ios::binary);
        if (inFile) {
            cout << "\nContenido de index.dat:\n";
            cout << "Binario   #Bucket\n";
            for (size_t i = 0; i < (1 << MAX_DEPTH); ++i) {
                int indexValue, bucketIndex;

                inFile.read(reinterpret_cast<char *>(&indexValue), sizeof(indexValue));
                inFile.read(reinterpret_cast<char *>(&bucketIndex), sizeof(bucketIndex));

                cout << bitset<MAX_DEPTH>(indexValue) << "      " << bucketIndex << "\n";
            }
            inFile.close();
        } else {
            cout << "Error al abrir index.dat." << endl;
        }
    }

private:
    int getBucketIndex(int i) {
        ifstream inFile("index.dat", ios::binary);
        if (inFile) {
            int bucketIndex;
            inFile.seekg(i * (sizeof(int) + sizeof(int)) + sizeof(int));
            inFile.read(reinterpret_cast<char *>(&bucketIndex), sizeof(bucketIndex));
            inFile.close();
            return bucketIndex;
        }
        return -1;
    }
};
