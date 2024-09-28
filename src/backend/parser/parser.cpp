#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include "contenido.cpp"
#include "../formats/AVL.cpp"
#include "../formats/SequentialFile.cpp"
#include "../data/HospitalRecord.h"
#include "../data/SocialRecord.h"
#include "../formats/ExtendibleHashing.cpp"
using namespace std;


#include "ExtendibleHashing.cpp"

vector<string> tokenize(const string& query) {
    stringstream ss(query);
    string token;
    vector<string> tokens;

    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

class MetadataManager {
    std::string metadataFile;
    struct MetadataRecord {
        std::string fileName;
        std::string fileType;  // AVL, Sequential, Hash
        std::string recordType; // hospitalesopendata, directorioredes
    };

    std::vector<MetadataRecord> metadata;

public:
    MetadataManager(const std::string& file) : metadataFile(file) {
        loadMetadata();
    }

    void loadMetadata() {
        std::ifstream file(metadataFile);
        if (!file.is_open()) {
            std::cout << "No se pudo abrir el archivo de metadatos, creando uno nuevo...\n";
            return;
        }

        std::string fileName, fileType, recordType;
        while (file >> fileName >> fileType >> recordType) {
            metadata.push_back({fileName, fileType, recordType});
        }
        file.close();
    }

    void saveMetadata(const std::string& fileName, const std::string& fileType, const std::string& recordType) {
        std::ofstream file(metadataFile, std::ios::app);
        if (!file.is_open()) {
            std::cout << "Error al abrir el archivo de metadatos\n";
            return;
        }
        file << fileName << " " << fileType << " " << recordType << "\n";
        file.close();
        metadata.push_back({fileName, fileType, recordType}); // Actualizar en memoria
    }

    const std::vector<MetadataRecord>& getMetadata() const {
        return metadata;
    }
};
class SQLParser {

    MetadataManager metadataManager;

    vector<AVLFile<int, HospitalRecord>> avlHospital;
    vector<AVLFile<int, SocialRecord>> avlSocial;
    vector<SequentialFile<int, HospitalRecord>> sequentialHospital;
    vector<SequentialFile<int, SocialRecord>> sequentialSocial;
    vector<ExtendibleHashing<int,SocialRecord>> extendibleSR;
    vector<ExtendibleHashing<int,HospitalRecord>> extendibleHR;
public:

    SQLParser() : metadataManager("metadata.txt") {
        const auto& metadata = metadataManager.getMetadata();

        for (const auto& entry : metadata) {
            if (entry.fileType == "AVL") {
                if (entry.recordType == "hospitalesopendata") {
                    AVLFile<int, HospitalRecord> avl(entry.fileName);
                    avlHospital.push_back(avl);
                } else if (entry.recordType == "directorioredes") {
                    AVLFile<int, SocialRecord> avl(entry.fileName);
                    avlSocial.push_back(avl);
                }
            } else if (entry.fileType == "Sequential") {
                if (entry.recordType == "hospitalesopendata") {
                    SequentialFile<int, HospitalRecord> seqFile(entry.fileName);
                    sequentialHospital.push_back(seqFile);
                } else if (entry.recordType == "directorioredes") {
                    SequentialFile<int, SocialRecord> seqFile(entry.fileName);
                    sequentialSocial.push_back(seqFile);
                }
            } else if (entry.fileType == "Hash") {
                if (entry.recordType == "hospitalesopendata") {
                    ExtendibleHashing<int, HospitalRecord> hashTable(entry.fileName);
                    extendibleHR.push_back(hashTable);
                } else if (entry.recordType == "directorioredes") {
                    ExtendibleHashing<int, SocialRecord> hashTable(entry.fileName);
                    extendibleSR.push_back(hashTable);
                }
            }
        }
    }



    void parse(const std::string& query) {
        auto tokens = tokenize(query);
        if (tokens.empty()) {
            cout << "Consulta vacía\n";
            return;
        }

        // CREATE TABLE command
        if (tokens[0] == "create" && tokens[1] == "table") {
            string nombre_archivo = tokens[2];
            string tipo_dato = tokens[5];
            if (tokens[7] == "AVL") {
                if (tipo_dato == "hospitalesopendata") {
                    AVLFile<int, HospitalRecord> avlH(nombre_archivo + ".dat");
                    avlHospital.push_back(avlH);

                    metadataManager.saveMetadata(nombre_archivo+".dat","AVL",tipo_dato);
                    std::cout << "Tabla AVL creada con Registro tipo: " << tipo_dato << std::endl;
                } else if (tipo_dato == "directorioredes") {
                    AVLFile<int, SocialRecord> avlS(nombre_archivo + ".dat");
                    avlSocial.push_back(avlS);

                    metadataManager.saveMetadata(nombre_archivo+".dat","AVL",tipo_dato);

                    std::cout << "Tabla AVL creada con Registro tipo: " << tipo_dato << std::endl;
                }
            } else if (tokens[7] == "sequential") {
                if (tipo_dato == "hospitalesopendata") {
                    SequentialFile<int, HospitalRecord> seqH(nombre_archivo + ".dat");
                    sequentialHospital.push_back(seqH);
                    metadataManager.saveMetadata(nombre_archivo+".dat","Sequential",tipo_dato);
                    std::cout << "Tabla Sequential creada con Registro tipo: " << tipo_dato << std::endl;
                } else if (tipo_dato == "directorioredes") {
                    SequentialFile<int, SocialRecord> seqS(nombre_archivo + ".dat");
                    sequentialSocial.push_back(seqS);
                    metadataManager.saveMetadata(nombre_archivo+".dat","Sequential",tipo_dato);

                    std::cout << "Tabla Sequential creada con Registro tipo: " << tipo_dato << std::endl;
                }
            }
            else if (tokens[7] == "hash") {
                if (tipo_dato == "hospitalesopendata") {
                    ExtendibleHashing<int,HospitalRecord> hashTable(nombre_archivo+".dat");
                    extendibleHR.push_back(hashTable);
                    metadataManager.saveMetadata(nombre_archivo+".dat","Hash",tipo_dato);

                    std::cout << "Tabla Hash creada con Registro tipo: " << tipo_dato << std::endl;
                } else if (tipo_dato == "directorioredes") {
                    ExtendibleHashing<int,SocialRecord> hashTable(nombre_archivo+".dat");
                    extendibleSR.push_back(hashTable);
                    metadataManager.saveMetadata(nombre_archivo+".dat","Hash",tipo_dato);

                    std::cout << "Tabla Hash creada con Registro tipo: " << tipo_dato << std::endl;
                }
            }
        }

        // INSERT INTO command
        else if (tokens[0] == "insert" && tokens[1] == "into") {
            string nombre = tokens[2];
            int indexAVL = -1;
            int indexSequential = -1;
            int indexHash = -1;

            // AVLFile handling
            indexAVL = buscarAVLPorNombre(avlHospital, nombre + ".dat");
            if (indexAVL != -1) {
                handleInsertAVL<HospitalRecord>(tokens, avlHospital[indexAVL]);
            } else {
                indexAVL = buscarAVLPorNombre(avlSocial, nombre + ".dat");
                if (indexAVL != -1) {
                    handleInsertAVL<SocialRecord>(tokens, avlSocial[indexAVL]);
                }
            }

            // SequentialFile handling
            indexSequential = buscarSequentialPorNombre(sequentialHospital, nombre + ".dat");
            if (indexSequential != -1) {
                handleInsertSequential<HospitalRecord>(tokens, sequentialHospital[indexSequential]);
            } else {
                indexSequential = buscarSequentialPorNombre(sequentialSocial, nombre + ".dat");
                if (indexSequential != -1) {
                    handleInsertSequential<SocialRecord>(tokens, sequentialSocial[indexSequential]);
                }
            }
           // ExtendibleHashing handling
            indexHash = buscarExtendibleHashing(extendibleHR, nombre+".dat");
            if (indexHash != -1) {
              handleInsertHash<HospitalRecord>(tokens, extendibleHR[indexHash]);
            } else {
                indexHash = buscarExtendibleHashing(extendibleSR, nombre+".dat");
                if (indexHash != -1) {handleInsertHash<SocialRecord>(tokens, extendibleSR[indexHash]);
                }
            }

            if (indexAVL == -1 && indexSequential == -1 &&indexHash== -1 ){
                cout << "Tabla no encontrada para insertar.\n";
            }
        }

        // SELECT command
        else if (tokens[0] == "select" && tokens[6] == "=") {
            string nombreTabla = tokens[3];
            int key = stoi(tokens[7]);
            int indexAVL = buscarAVLPorNombre(avlHospital, nombreTabla + ".dat");
            int indexSequential = buscarSequentialPorNombre(sequentialHospital, nombreTabla + ".dat");
            int HashIndex=buscarExtendibleHashing(extendibleHR,nombreTabla+".dat");
            if (indexAVL != -1) {
                HospitalRecord registro = avlHospital[indexAVL].find(key);
                registro.mostrarDatos();
            } else if (indexSequential != -1) {
                HospitalRecord registros = sequentialHospital[indexSequential].search(key);
                    registros.mostrarDatos();
            } else if(HashIndex!=-1) {
                HospitalRecord registroH=extendibleHR[HashIndex].find(key);
                registroH.mostrarDatos();
            }

            else {
                indexAVL = buscarAVLPorNombre(avlSocial, nombreTabla + ".dat");
                indexSequential=buscarSequentialPorNombre(sequentialSocial,nombreTabla+".dat");
                HashIndex=buscarExtendibleHashing(extendibleSR,nombreTabla+".dat");
                if (indexAVL != -1) {
                    SocialRecord registro = avlSocial[indexAVL].find(key);
                    registro.mostrarDatos();
                } else if(indexSequential!=-1){
                    indexSequential = buscarSequentialPorNombre(sequentialSocial, nombreTabla + ".dat");
                    if (indexSequential != -1) {
                        SocialRecord registros = sequentialSocial[indexSequential].search(key);
                           registros.mostrarDatos();

                    }
                }
                 else if(HashIndex!=-1) {
                    SocialRecord registroS=extendibleSR[HashIndex].find(key);
                    registroS.mostrarDatos();
                }
            }

            if (indexAVL == -1 && indexSequential == -1 and HashIndex) {
                cout << "Tabla no encontrada para buscar.\n";
            }
        }

        // Range search
        else if (tokens[0] == "select" && tokens[6] == "between") {
            string nombreTabla = tokens[3];
            int keya = stoi(tokens[7]);
            int keyb = stoi(tokens[9]);
            int HashIndex = buscarExtendibleHashing(extendibleHR, nombreTabla + ".dat");
            int indexSequential = buscarSequentialPorNombre(sequentialHospital, nombreTabla + ".dat");

            if (HashIndex != -1) {
                vector<HospitalRecord> results=extendibleHR[HashIndex].findRange(keya,keyb);
                for(auto a:results)
                    a.mostrarDatos();
            } else if (indexSequential != -1) {
                vector<HospitalRecord> registros = sequentialHospital[indexSequential].rangeSearch(keya, keyb);
                for (auto &reg : registros) {
                    reg.mostrarDatos();
                }
            } else {
                HashIndex = buscarExtendibleHashing(extendibleSR, nombreTabla + ".dat");
                if (HashIndex != -1) {
                    vector<SocialRecord> results=extendibleSR[HashIndex].findRange(keya,keyb);
                    for(auto a:results)
                        a.mostrarDatos();
                } else {
                    indexSequential = buscarSequentialPorNombre(sequentialSocial, nombreTabla + ".dat");
                    if (indexSequential != -1) {
                        vector<SocialRecord> registros = sequentialSocial[indexSequential].rangeSearch(keya, keyb);
                        for (auto &reg : registros) {
                            reg.mostrarDatos();
                        }
                    }
                }
            }

            if (HashIndex == -1 && indexSequential == -1) {
                cout << "Tabla no encontrada para buscar.\n";
            }
        }

        // DELETE command
        else if (tokens[0] == "delete" && tokens[1] == "from" && tokens[5] == "=") {
            string nombreTabla = tokens[2];
            int key = stoi(tokens[6]);
            int indexAVL = buscarAVLPorNombre(avlHospital, nombreTabla + ".dat");
            int indexSequential = buscarSequentialPorNombre(sequentialHospital, nombreTabla + ".dat");
            int HashIndex=buscarExtendibleHashing(extendibleHR,nombreTabla+".dat");
            if (indexAVL != -1) {
                bool deleted = avlHospital[indexAVL].remove(key);
                if (deleted) {
                    cout << "Registro eliminado correctamente de " << nombreTabla << ".\n";
                } else {
                    cout << "Registro no encontrado para eliminar en " << nombreTabla << ".\n";
                }
            } else if (indexSequential != -1) {
                bool deleted = sequentialHospital[indexSequential].remove(key);
                if (deleted) {
                    cout << "Registro eliminado correctamente de " << nombreTabla << ".\n";
                } else {
                    cout << "Registro no encontrado para eliminar en " << nombreTabla << ".\n";
                }
            } else if (HashIndex != -1) {
                extendibleHR[HashIndex].remove(key);
            }
            else {
                indexAVL = buscarAVLPorNombre(avlSocial, nombreTabla + ".dat");
                indexSequential = buscarSequentialPorNombre(sequentialSocial, nombreTabla + ".dat");
                HashIndex=buscarExtendibleHashing(extendibleSR,nombreTabla+".dat");

                if (indexAVL != -1) {
                    bool deleted = avlSocial[indexAVL].remove(key);
                    if (deleted) {
                        cout << "Registro eliminado correctamente de " << nombreTabla << ".\n";
                    } else {
                        cout << "Registro no encontrado para eliminar en " << nombreTabla << ".\n";
                    }
                } else if(indexSequential!=-1){
                        bool deleted = sequentialSocial[indexSequential].remove(key);
                        cout<<key;
                        if (deleted) {
                            cout << "Registro eliminado correctamente de " << nombreTabla << ".\n";
                        } else {
                            cout << "Registro no encontrado para eliminar en " << nombreTabla << ".\n";
                        }

                }
                else if (HashIndex !=-1){
                    extendibleSR[HashIndex].remove(key);
                }
            }

            if (indexAVL == -1 && indexSequential == -1 and HashIndex== -1) {
                cout << "Tabla no encontrada para eliminar.\n";
            }
        }
    }

private:
    template <typename RecordType>
    void handleInsertAVL(vector<string>& tokens, AVLFile<int, RecordType>& avlFile) {
        if (tokens.size() >= 5) {
            vector<vector<string>> contenedor = extraerRegistros(tokens[4]);
            for (const auto& vect : contenedor) {
                RecordType registro;
                registro.llenarDatos(vect);
                cout << "Registro insertado correctamente en AVL.\n";
                avlFile.insert(registro);
            }

        }
    }

    template <typename RecordType>
    void handleInsertSequential(vector<string>& tokens, SequentialFile<int, RecordType>& sequentialFile) {
        if (tokens.size() >= 5) {
            vector<vector<string>> contenedor = extraerRegistros(tokens[4]);
            for (const auto& vect : contenedor) {
                RecordType registro;
                registro.llenarDatos(vect);
                cout << "Registro insertado correctamente en SequentialFile.\n";
                sequentialFile.add(registro);
            }

        }
    }
    template<typename RecordType>
    void handleInsertHash(vector<string>& tokens, ExtendibleHashing<int,RecordType> hash) {
        if (tokens.size() >= 5) {
            vector<vector<string>> contenedor = extraerRegistros(tokens[4]);
            for ( auto vect : contenedor) {
                RecordType registro;
                registro.llenarDatos(vect);
                hash.insert(registro);
                cout << "Registro insertado correctamente en Hash.\n";

            }

        }
    }
};

void prueba() {
//insert into Datitos values (767,Abigail,Salcedo,eee,eer,ttyy,uuuu,ttt,5,www,rrr,ttt,6,785646544),(868,Abigail888888888888888,Salcedo,eee,eer,ttyy,uuuu,ttt,5,www,rrr,ttt,6,78564544)
//create table Datitos from file hospitalesopendata using AVL
    SQLParser parse;
    /*string frase;
    getline(std::cin, frase);
    parse.parse(frase);
    cout<<endl;
    string frase2;
    getline(std::cin, frase2);
    parse.parse(frase2);
    cout<<endl;
    string frase3;
    getline(std::cin, frase3);
    parse.parse(frase3);
    cout<<endl;
    string frase4;
    getline(std::cin, frase4);
    parse.parse(frase4);*/

}