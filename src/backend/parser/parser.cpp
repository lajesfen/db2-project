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

class SQLParser {
    vector<AVLFile<int, HospitalRecord>> avlHospital;
    vector<AVLFile<int, SocialRecord>> avlSocial;
    vector<SequentialFile<int, HospitalRecord>> sequentialHospital;
    vector<SequentialFile<int, SocialRecord>> sequentialSocial;
public:
    void parse(const string& query) {
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
                    std::cout << "Tabla AVL creada con Registro tipo: " << tipo_dato << std::endl;
                } else if (tipo_dato == "directorioredes") {
                    AVLFile<int, SocialRecord> avlS(nombre_archivo + ".dat");
                    avlSocial.push_back(avlS);
                    std::cout << "Tabla AVL creada con Registro tipo: " << tipo_dato << std::endl;
                }
            } else if (tokens[7] == "sequential") {
                if (tipo_dato == "hospitalesopendata") {
                    SequentialFile<int, HospitalRecord> seqH(nombre_archivo + ".dat");
                    sequentialHospital.push_back(seqH);
                    std::cout << "Tabla Sequential creada con Registro tipo: " << tipo_dato << std::endl;
                } else if (tipo_dato == "directorioredes") {
                    SequentialFile<int, SocialRecord> seqS(nombre_archivo + ".dat");
                    sequentialSocial.push_back(seqS);
                    std::cout << "Tabla Sequential creada con Registro tipo: " << tipo_dato << std::endl;
                }
            }
        }

		// INSERT INTO command
        else if (tokens[0] == "insert" && tokens[1] == "into") {
            string nombre = tokens[2];
            int indexAVL = -1;
            int indexSequential = -1;

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

            if (indexAVL == -1 && indexSequential == -1) {
                cout << "Tabla no encontrada para insertar.\n";
            }
        }


        //select
        else if (tokens[0] == "select" && tokens[6] == "=") {
            string nombreTabla = tokens[3];
            int key = stoi(tokens[7]);
            int indexAVL = buscarAVLPorNombre(avlHospital, nombreTabla + ".dat");
            int indexSequential = buscarSequentialPorNombre(sequentialHospital, nombreTabla + ".dat");

            if (indexAVL != -1) {
                HospitalRecord registro = avlHospital[indexAVL].find(key);
                registro.mostrarDatos();
            } else if (indexSequential != -1) {
                vector<HospitalRecord> registros = sequentialHospital[indexSequential].search(key);
                for (auto &reg : registros) {
                    reg.mostrarDatos();
                }
            } else {
                indexAVL = buscarAVLPorNombre(avlSocial, nombreTabla + ".dat");
                if (indexAVL != -1) {
                    SocialRecord registro = avlSocial[indexAVL].find(key);
                    registro.mostrarDatos();
                } else {
                    indexSequential = buscarSequentialPorNombre(sequentialSocial, nombreTabla + ".dat");
                    if (indexSequential != -1) {
                        vector<SocialRecord> registros = sequentialSocial[indexSequential].search(key);
                        for (auto &reg : registros) {
                            reg.mostrarDatos();
                        }
                    }
                }
            }

            if (indexAVL == -1 && indexSequential == -1) {
                cout << "Tabla no encontrada para buscar.\n";
            }
        }

        //delete

        else if (tokens[0] == "delete" && tokens[1] == "from" && tokens[5] == "=") {
            string nombreTabla = tokens[2];
            int key = stoi(tokens[6]);
            int indexAVL = buscarAVLPorNombre(avlHospital, nombreTabla + ".dat");
            int indexSequential = buscarSequentialPorNombre(sequentialHospital, nombreTabla + ".dat");

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
            } else {
                indexAVL = buscarAVLPorNombre(avlSocial, nombreTabla + ".dat");
                if (indexAVL != -1) {
                    bool deleted = avlSocial[indexAVL].remove(key);
                    if (deleted) {
                        cout << "Registro eliminado correctamente de " << nombreTabla << ".\n";
                    } else {
                        cout << "Registro no encontrado para eliminar en " << nombreTabla << ".\n";
                    }
                } else {
                    indexSequential = buscarSequentialPorNombre(sequentialSocial, nombreTabla + ".dat");
                    if (indexSequential != -1) {
                        bool deleted = sequentialSocial[indexSequential].remove(key);
                        if (deleted) {
                            cout << "Registro eliminado correctamente de " << nombreTabla << ".\n";
                        } else {
                            cout << "Registro no encontrado para eliminar en " << nombreTabla << ".\n";
                        }
                    }
                }
            }

            if (indexAVL == -1 && indexSequential == -1) {
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