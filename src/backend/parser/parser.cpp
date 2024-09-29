#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "contenido.cpp"
#include "../formats/AVL.cpp"
#include "../formats/SequentialFile.cpp"
#include "../formats/ExtendibleHashing.cpp"
#include "../data/HospitalRecord.h"
#include "../data/SocialRecord.h"
#include "../loaders/DataLoader.cpp"
using namespace std;

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

    nlohmann::ordered_json parse(const std::string& query) {
        auto tokens = tokenize(query);
        nlohmann::ordered_json response;

        if (tokens.empty()) {
            cout << "Consulta vacía\n";
            response["message"] = "Consulta vacía.";
            return response;
        }

        // CREATE TABLE command
        if (tokens[0] == "create" && tokens[1] == "table") {
            string nombre_archivo = tokens[2];
            string tipo_dato = tokens[5];
            auto hospitalData = readHospitalFromCSV();
            auto redesData = readSocialFromCSV();

            if (tokens[7] == "AVL") {
                if (tipo_dato == "hospitalesopendata") {
                    AVLFile<int, HospitalRecord> avlH(nombre_archivo + ".dat");
                    metadataManager.saveMetadata(nombre_archivo+".dat","AVL",tipo_dato);

                    for(auto data : hospitalData) {
                        avlH.insert(data);
                    }
                    avlHospital.push_back(std::move(avlH));

                    std::cout << "Tabla AVL creada con Registro tipo: " << tipo_dato << std::endl;
                    response["message"] = "Tabla AVL creada con Registro tipo: " + tipo_dato;
                    return response;

                } else if (tipo_dato == "directorioredes") {
                    AVLFile<int, SocialRecord> avlS(nombre_archivo + ".dat");
                    metadataManager.saveMetadata(nombre_archivo+".dat","AVL",tipo_dato);

                    for(auto data : redesData) {
                        avlS.insert(data);
                    }
                    avlSocial.push_back(std::move(avlS));

                    std::cout << "Tabla AVL creada con Registro tipo: " << tipo_dato << std::endl;
                    response["message"] = "Tabla AVL creada con Registro tipo: " + tipo_dato;
                    return response;
                }
            } else if (tokens[7] == "sequential") {
                if (tipo_dato == "hospitalesopendata") {
                    SequentialFile<int, HospitalRecord> seqH(nombre_archivo + ".dat");
                    metadataManager.saveMetadata(nombre_archivo+".dat","Sequential",tipo_dato);

                    for(auto data : hospitalData) {
                        seqH.add(data);
                    }
                    sequentialHospital.push_back(std::move(seqH));

                    std::cout << "Tabla Sequential creada con Registro tipo: " << tipo_dato << std::endl;
                    response["message"] = "Tabla Sequential creada con Registro tipo: " + tipo_dato;
                    return response;

                } else if (tipo_dato == "directorioredes") {
                    SequentialFile<int, SocialRecord> seqS(nombre_archivo + ".dat");
                    metadataManager.saveMetadata(nombre_archivo+".dat","Sequential",tipo_dato);

                    for(auto data : redesData) {
                        seqS.add(data);
                    }
                    sequentialSocial.push_back(std::move(seqS));

                    std::cout << "Tabla Sequential creada con Registro tipo: " << tipo_dato << std::endl;
                    response["message"] = "Tabla Sequential creada con Registro tipo: " + tipo_dato;
                    return response;
                }
            }
            else if (tokens[7] == "hash") {
                if (tipo_dato == "hospitalesopendata") {
                    ExtendibleHashing<int,HospitalRecord> hashTable(nombre_archivo+".dat");
                    metadataManager.saveMetadata(nombre_archivo+".dat","Hash",tipo_dato);

                    for(auto data : hospitalData) {
                        hashTable.insert(data);
                    }
                    extendibleHR.push_back(std::move(hashTable));

                    std::cout << "Tabla Hash creada con Registro tipo: " << tipo_dato << std::endl;
                    response["message"] = "Tabla Hash creada con Registro tipo: " + tipo_dato;
                    return response;

                } else if (tipo_dato == "directorioredes") {
                    ExtendibleHashing<int,SocialRecord> hashTable(nombre_archivo+".dat");
                    metadataManager.saveMetadata(nombre_archivo+".dat","Hash",tipo_dato);

                    for(auto data : redesData) {
                        hashTable.insert(data);
                    }
                    extendibleSR.push_back(std::move(hashTable));

                    std::cout << "Tabla Hash creada con Registro tipo: " << tipo_dato << std::endl;
                    response["message"] = "Tabla Hash creada con Registro tipo: " + tipo_dato;
                    return response;
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
                return handleInsertAVL<HospitalRecord>(query, avlHospital[indexAVL]);
            } else {
                indexAVL = buscarAVLPorNombre(avlSocial, nombre + ".dat");
                if (indexAVL != -1) {
                    return handleInsertAVL<SocialRecord>(query, avlSocial[indexAVL]);
                }
            }

            // SequentialFile handling
            indexSequential = buscarSequentialPorNombre(sequentialHospital, nombre + ".dat");
            if (indexSequential != -1) {
                return handleInsertSequential<HospitalRecord>(query, sequentialHospital[indexSequential]);
            } else {
                indexSequential = buscarSequentialPorNombre(sequentialSocial, nombre + ".dat");
                if (indexSequential != -1) {
                    return handleInsertSequential<SocialRecord>(query, sequentialSocial[indexSequential]);
                }
            }
           // ExtendibleHashing handling
            indexHash = buscarExtendibleHashing(extendibleHR, nombre+".dat");
            if (indexHash != -1) {
                return handleInsertHash<HospitalRecord>(query, extendibleHR[indexHash]);
            } else {
                indexHash = buscarExtendibleHashing(extendibleSR, nombre+".dat");
                if (indexHash != -1) {
                    return handleInsertHash<SocialRecord>(query, extendibleSR[indexHash]);
                }
            }

            if (indexAVL == -1 && indexSequential == -1 &&indexHash== -1 ){
                cout << "Tabla no encontrada para insertar.\n";
                response["message"] = "Tabla no encontrada para insertar.";
                return response;
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
                if(registro.id != -1) {
                    response.push_back(registro.toJSON());
                    return response;
                }
                response["message"] = "No se encontraron datos para esa consulta.";
                return response;
            } else if (indexSequential != -1) {
                HospitalRecord registros = sequentialHospital[indexSequential].search(key);
                if(registros.id != -1) {
                    response.push_back(registros.toJSON());
                    return response;
                }
                response["message"] = "No se encontraron datos para esa consulta.";
                return response;
            } else if(HashIndex!=-1) {
                HospitalRecord registroH=extendibleHR[HashIndex].find(key);
                if(registroH.id != -1) {
                    response.push_back(registroH.toJSON());
                    return response;
                }
                response["message"] = "No se encontraron datos para esa consulta.";
                return response;
            }

            else {
                indexAVL = buscarAVLPorNombre(avlSocial, nombreTabla + ".dat");
                indexSequential=buscarSequentialPorNombre(sequentialSocial,nombreTabla+".dat");
                HashIndex=buscarExtendibleHashing(extendibleSR,nombreTabla+".dat");
                if (indexAVL != -1) {
                    SocialRecord registro = avlSocial[indexAVL].find(key);
                    if(registro.id != -1) {
                        response.push_back(registro.toJSON());
                        return response;
                    }
                    response["message"] = "No se encontraron datos para esa consulta.";
                    return response;
                } else if(indexSequential!=-1) {
                    indexSequential = buscarSequentialPorNombre(sequentialSocial, nombreTabla + ".dat");
                    if (indexSequential != -1) {
                        SocialRecord registros = sequentialSocial[indexSequential].search(key);
                        if(registros.id != -1) {
                            response.push_back(registros.toJSON());
                            return response;
                        }
                        response["message"] = "No se encontraron datos para esa consulta.";
                        return response;
                    }
                }
                 else if(HashIndex!=-1) {
                    SocialRecord registroS=extendibleSR[HashIndex].find(key);
                    if(registroS.id != -1) {
                        response.push_back(registroS.toJSON());
                        return response;
                    }
                    response["message"] = "No se encontraron datos para esa consulta.";
                    return response;
                }
            }

            if (indexAVL == -1 && indexSequential == -1 and HashIndex) {
                cout << "Tabla no encontrada para buscar.\n";
                response["message"] = "Tabla no encontrada para buscar.";
                return response;
            }
        }

        // Range search
        else if (tokens[0] == "select" && tokens[6] == "between") {
            string nombreTabla = tokens[3];
            int keya = stoi(tokens[7]);
            int keyb = stoi(tokens[9]);
            int indexAVL=buscarAVLPorNombre(avlHospital,nombreTabla+".dat");
            int HashIndex = buscarExtendibleHashing(extendibleHR, nombreTabla + ".dat");
            int indexSequential = buscarSequentialPorNombre(sequentialHospital, nombreTabla + ".dat");

            if (indexAVL != -1) {
                vector<HospitalRecord> results = avlHospital[indexAVL].searchByRange(keya,keyb);
                for(auto &a:results) {
                    if(a.id != -1)
                        response.push_back(a.toJSON());
                }

                if(!response.empty()) {
                    return response;
                } else {
                    response["message"] = "No se encontraron datos para esa consulta.";
                    return response;
                }
            }
             else if (HashIndex != -1) {
                vector<HospitalRecord> results = extendibleHR[HashIndex].findRange(keya,keyb);
                for(auto &a:results) {
                    if(a.id != -1)
                        response.push_back(a.toJSON());
                }

                if(!response.empty()) {
                    return response;
                } else {
                    response["message"] = "No se encontraron datos para esa consulta.";
                    return response;
                }
            } else if (indexSequential != -1) {
                vector<HospitalRecord> registros = sequentialHospital[indexSequential].rangeSearch(keya, keyb);
                for (auto &reg : registros) {
                    if(reg.id != -1)
                        response.push_back(reg.toJSON());
                }

                if(!response.empty()) {
                    return response;
                } else {
                    response["message"] = "No se encontraron datos para esa consulta.";
                    return response;
                }
            } else {
                HashIndex = buscarExtendibleHashing(extendibleSR, nombreTabla + ".dat");
                indexAVL=buscarAVLPorNombre(avlSocial,nombreTabla+".dat");

                if (HashIndex != -1) {
                    vector<SocialRecord> results = avlSocial[indexAVL].searchByRange(keya,keyb);
                    for(auto &a:results) {
                        if(a.id != -1)
                            response.push_back(a.toJSON());
                    }

                    if(!response.empty()) {
                        return response;
                    } else {
                        response["message"] = "No se encontraron datos para esa consulta.";
                        return response;
                    }
                } else if (HashIndex != -1) {
                    vector<SocialRecord> results=extendibleSR[HashIndex].findRange(keya,keyb);
                    for(auto &a:results) {
                        if(a.id != -1)
                            response.push_back(a.toJSON());
                    }

                    if(!response.empty()) {
                        return response;
                    } else {
                        response["message"] = "No se encontraron datos para esa consulta.";
                        return response;
                    }
                } else {
                    indexSequential = buscarSequentialPorNombre(sequentialSocial, nombreTabla + ".dat");
                    if (indexSequential != -1) {
                        vector<SocialRecord> registros = sequentialSocial[indexSequential].rangeSearch(keya, keyb);
                        for (auto &reg : registros) {
                            if(reg.id != -1)
                                response.push_back(reg.toJSON());
                        }

                        if(!response.empty()) {
                            return response;
                        } else {
                            response["message"] = "No se encontraron datos para esa consulta.";
                            return response;
                        }
                    }
                }

                response["message"] = "No se encontraron datos para esa consulta.";
                return response;
            }

            if (HashIndex == -1 && indexSequential == -1) {
                cout << "Tabla no encontrada para buscar.\n";
                response["message"] = "Tabla no encontrada para buscar.";
                return response;
            }
        }

        // SELECT ALL
        else if (tokens[0] == "select") {
            string nombreTabla = tokens[3];
            int indexAVL = buscarAVLPorNombre(avlHospital, nombreTabla + ".dat");
            int indexSequential = buscarSequentialPorNombre(sequentialHospital, nombreTabla + ".dat");
            int HashIndex=buscarExtendibleHashing(extendibleHR,nombreTabla+".dat");

            if (indexAVL != -1) {
                auto registros = avlHospital[indexAVL].inorder();
                for(auto registro : registros) {
                    response.push_back(registro.toJSON());
                }
                return response;
            } else if (indexSequential != -1) {
                auto registros = sequentialHospital[indexSequential].getAllRecords();
                for(auto registro : registros) {
                    response.push_back(registro.toJSON());
                }
                return response;
            } else if(HashIndex!=-1) {
//                HospitalRecord registroH=extendibleHR[HashIndex];
//                response.push_back(registroH.toJSON());
                return response;
            }

            else {
                indexAVL = buscarAVLPorNombre(avlSocial, nombreTabla + ".dat");
                indexSequential=buscarSequentialPorNombre(sequentialSocial,nombreTabla+".dat");
                HashIndex=buscarExtendibleHashing(extendibleSR,nombreTabla+".dat");
                if (indexAVL != -1) {
                    auto registros = avlSocial[indexAVL].inorder();
                    for(auto registro : registros) {
                        response.push_back(registro.toJSON());
                    }
                    return response;
                } else if(indexSequential!=-1) {
                    indexSequential = buscarSequentialPorNombre(sequentialSocial, nombreTabla + ".dat");
                    if (indexSequential != -1) {
                        auto registros = sequentialSocial[indexSequential].getAllRecords();
                        for(auto registro : registros) {
                            response.push_back(registro.toJSON());
                        }
                        return response;
                    }
                }
                else if(HashIndex!=-1) {
//                    SocialRecord registroS=extendibleSR[HashIndex].find(key);
//                    response.push_back(registroS.toJSON());
                    return response;
                }
            }

            if (indexAVL == -1 && indexSequential == -1 and HashIndex) {
                cout << "Tabla no encontrada para buscar.\n";
                response["message"] = "Tabla no encontrada para buscar.";
                return response;
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
                    response["message"] = "Registro eliminado correctamente de " + nombreTabla;
                    return response;
                } else {
                    cout << "Registro no encontrado para eliminar en " << nombreTabla << ".\n";
                    response["message"] = "Registro no encontrado para eliminar en " + nombreTabla;
                    return response;
                }
            } else if (indexSequential != -1) {
                bool deleted = sequentialHospital[indexSequential].remove(key);
                if (deleted) {
                    cout << "Registro eliminado correctamente de " << nombreTabla << ".\n";
                    response["message"] = "Registro eliminado correctamente de " + nombreTabla;
                    return response;
                } else {
                    cout << "Registro no encontrado para eliminar en " << nombreTabla << ".\n";
                    response["message"] = "Registro no encontrado para eliminar en " + nombreTabla;
                    return response;
                }
            } else if (HashIndex != -1) {
                bool deleted = extendibleHR[HashIndex].remove(key);
                if (deleted) {
                    cout << "Registro eliminado correctamente de " << nombreTabla << ".\n";
                    response["message"] = "Registro eliminado correctamente de " + nombreTabla;
                    return response;
                } else {
                    cout << "Registro no encontrado para eliminar en " << nombreTabla << ".\n";
                    response["message"] = "Registro no encontrado para eliminar en " + nombreTabla;
                    return response;
                }
            }
            else {
                indexAVL = buscarAVLPorNombre(avlSocial, nombreTabla + ".dat");
                indexSequential = buscarSequentialPorNombre(sequentialSocial, nombreTabla + ".dat");
                HashIndex=buscarExtendibleHashing(extendibleSR,nombreTabla+".dat");

                if (indexAVL != -1) {
                    bool deleted = avlSocial[indexAVL].remove(key);
                    if (deleted) {
                        cout << "Registro eliminado correctamente de " << nombreTabla << ".\n";
                        response["message"] = "Registro eliminado correctamente de " + nombreTabla;
                        return response;
                    } else {
                        cout << "Registro no encontrado para eliminar en " << nombreTabla << ".\n";
                        response["message"] = "Registro no encontrado para eliminar en " + nombreTabla;
                        return response;
                    }
                } else if(indexSequential!=-1){
                        bool deleted = sequentialSocial[indexSequential].remove(key);
                        cout<<key;
                        if (deleted) {
                            cout << "Registro eliminado correctamente de " << nombreTabla << ".\n";
                            response["message"] = "Registro eliminado correctamente de " + nombreTabla;
                            return response;
                        } else {
                            cout << "Registro no encontrado para eliminar en " << nombreTabla << ".\n";
                            response["message"] = "Registro no encontrado para eliminar en " + nombreTabla;
                            return response;
                        }
                }
                else if (HashIndex !=-1){
                    bool deleted = extendibleSR[HashIndex].remove(key);
                    if (deleted) {
                        cout << "Registro eliminado correctamente de " << nombreTabla << ".\n";
                        response["message"] = "Registro eliminado correctamente de " + nombreTabla;
                        return response;
                    } else {
                        cout << "Registro no encontrado para eliminar en " << nombreTabla << ".\n";
                        response["message"] = "Registro no encontrado para eliminar en " + nombreTabla;
                        return response;
                    }
                }
            }

            if (indexAVL == -1 && indexSequential == -1 and HashIndex== -1) {
                cout << "Tabla no encontrada para eliminar.\n";
                response["message"] = "Tabla no encontrada para eliminar.";
                return response;
            }
        }

        response["message"] = "Query indefinida.";
        return response;
    }

private:
    template <typename RecordType>
    json handleInsertAVL(const string &query, AVLFile<int, RecordType>& avlFile) {
        vector<vector<string>> contenedor = extraerRegistros(query);
        json response;

        if (!contenedor.empty()) {
            for (const auto& vect : contenedor) {
                RecordType registro;
                registro.llenarDatos(vect);
                avlFile.insert(registro);

                cout << "Registro insertado correctamente en AVL.\n";
                response["message"] = "Registro insertado correctamente en AVL.";
                return response;
            }
        }

        response["message"] = "Error insertando.";
        return response;
    }

    template <typename RecordType>
    json handleInsertSequential(const string &query, SequentialFile<int, RecordType>& sequentialFile) {
        vector<vector<string>> contenedor = extraerRegistros(query);
        json response;

        if (!contenedor.empty()) {
            for (const auto& vect : contenedor) {
                RecordType registro;
                registro.llenarDatos(vect);
                sequentialFile.add(registro);

                cout << "Registro insertado correctamente en SequentialFile.\n";
                response["message"] = "Registro insertado correctamente en SequentialFile.";
                return response;
            }
        }

        response["message"] = "Error insertando.";
        return response;
    }

    template<typename RecordType>
    json handleInsertHash(const string &query, ExtendibleHashing<int,RecordType> hash) {
        vector<vector<string>> contenedor = extraerRegistros(query);
        json response;

        if (!contenedor.empty()) {
            for ( auto vect : contenedor) {
                RecordType registro;
                registro.llenarDatos(vect);
                hash.insert(registro);

                cout << "Registro insertado correctamente en HashTable.\n";
                response["message"] = "Registro insertado correctamente en HashTable.";
                return response;
            }
        }

        response["message"] = "Error insertando.";
        return response;
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