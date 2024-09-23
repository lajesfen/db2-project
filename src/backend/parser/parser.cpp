#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include "contenido.cpp"
#include "../formats/AVL.cpp"
#include "../formats/SequentialFile.cpp"
using namespace std;
//modifique el nombre de record de el sequential file para que no se confunda con el otro record y agregue para que se cree el archivo al ponerle el nombre en el constructor

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
    vector<AVLFile<int>> avls;
    // vector<ExtendibleHashing> hashes;
    vector<SequentialFile<int>> sequentialFiles;
public:
    void parse(const string& query) {
        auto tokens = tokenize(query);
        if (tokens.empty()) return;
        // create
        if (tokens[0] == "create" and tokens[1] == "table") {
            string nombre_archivo=tokens[2];

            if(tokens[3]=="from") {
                string archivo_csv=tokens[5];

                if(tokens[7]=="AVL") {
                    nombre_archivo += ".dat";
                    AVLFile<int> avl(nombre_archivo);
                    cout << "Creando tabla AVL del archivo csv: " << archivo_csv << endl;
                    avls.push_back(avl);
                }
                // Soporte para Extendible Hashing
                /*else if (tokens[7] == "hash") {
                    ExtendibleHashing hashTable;
                    cout << "Creando tabla Hash: " << archivo_csv << endl;
                    hashes.push_back(hashTable);
                }*/
                // Soporte para Sequential File
                else if (tokens[7] == "sequential") {
                    nombre_archivo += ".csv";
                    SequentialFile<int> seqFile(nombre_archivo);
                    cout << "Creando tabla Sequential: " << nombre_archivo << endl;
                    sequentialFiles.push_back(seqFile);
                }
            }
        }
        //insert
        else if (tokens[0] == "insert" && tokens[1] == "into") {
            string nombre=tokens[2];

            int indexAVL = buscarAVLPorNombre(avls, nombre+".dat" );
            //  int indexHash = buscarHashPorNombre(hashes, nombre);
            int indexSequential = buscarSequentialPorNombre(sequentialFiles, nombre+".csv");

            if (indexAVL != -1) {
                handleInsertAVL(tokens, avls[indexAVL]);
            }
            /*else if (indexHash != -1) {
                handleInsertHash(tokens, hashes[indexHash]);
            }*/
            else if (indexSequential != -1) {
                handleInsertSequential(tokens, sequentialFiles[indexSequential]);
            } else {
                cout << "Tabla no encontrada para insertar.\n";
            }
        }
        //select
        else if (tokens[0] == "select") {
            string nombrearchivo=tokens[3];
            if(tokens[6]=="between") {
                int indexSequential = buscarSequentialPorNombre(sequentialFiles, nombrearchivo+ ".csv");
                vector<RecordSF<int>> res=handleSelectRange(tokens,sequentialFiles[indexSequential]);
                for(auto a:res) {
                    a.showData();
                    cout<<endl;
                }
            }
            else if(tokens[6]=="=")
            {
                int indexAVL = buscarAVLPorNombre(avls, nombrearchivo+ ".dat");
                // int indexHash = buscarHashPorNombre(hashes, nombrearchivo);
                int indexSequential = buscarSequentialPorNombre(sequentialFiles, nombrearchivo+ ".csv");

                if (indexAVL != -1) {
                    handleSelectAVL(tokens, indexAVL);
                }
                /*else if (indexHash != -1) {
                    handleSelectHash(tokens, indexHash);
                }*/
                else if (indexSequential != -1) {
                    handleSelectSequential(tokens, indexSequential);
                } else {
                    cout << "Tabla no encontrada para seleccionar.\n";
                }
            }
        }

        //delete

        else if (tokens[0] == "delete" && tokens[2] == "from") {
            string nombrearchivo = tokens[3] ;
            int indexAVL = buscarAVLPorNombre(avls, nombrearchivo+ ".dat");
            //int indexHash = buscarHashPorNombre(hashes, nombrearchivo);
            int indexSequential = buscarSequentialPorNombre(sequentialFiles, nombrearchivo+ ".csv");

            if (indexAVL != -1) {
                handleDeleteAVL(tokens, avls[indexAVL]);
            }
            /*else if (indexHash != -1) {
                handleDeleteHash(tokens, hashes[indexHash]);
            }*/
            else if (indexSequential != -1) {
                handleDeleteSequential(tokens, sequentialFiles[indexSequential]);
            } else {
                cout << "Tabla no encontrada para eliminar.\n";
            }
        }
    }

private:

    // insertar sequential
    void handleInsertSequential(vector<string>& tokens, SequentialFile<int>& seqFile) {
        if (tokens.size() >= 5) {
            vector<string> contenedor = extraerPalabras(tokens[4]);
            RecordSF<int> registro;
            registro.codigo = stoi(contenedor[0]);
            strncpy(registro.nombre, contenedor[1].c_str(), sizeof(registro.nombre) - 1);
            registro.nombre[sizeof(registro.nombre) - 1] = '\0';
            strncpy(registro.apellido, contenedor[2].c_str(), sizeof(registro.apellido) - 1);
            registro.apellido[sizeof(registro.apellido) - 1] = '\0';
            registro.ciclo = stoi(contenedor[3]);
            registro.eliminado = false;
            seqFile.add(registro);
        }
    }
    // select rangos sequential
    vector<RecordSF<int>> handleSelectRange(vector<string>& tokens, SequentialFile<int>& seqFile) {
        vector<RecordSF<int>> resultado=seqFile.rangeSearch(stoi(tokens[7]),stoi(tokens[9]));
        return resultado;
    }
    // select sequential
    void handleSelectSequential(vector<string>& tokens, int pos) {
        if (tokens.size() >= 5 && tokens[4] == "where" && tokens[5] == "codigo") {
            int key = stoi(tokens[7]);
            try {
                RecordSF<int> registro_b = sequentialFiles[pos].search(key);
                registro_b.showData();
                cout<<endl;
            } catch (const std::runtime_error& e) {
                cout << e.what() << endl;
            }
        }
    }

    // Métodos de eliminación para Sequential File
    void handleDeleteSequential(vector<string>& tokens, SequentialFile<int>& seqFile) {
        if (tokens.size() >= 5 && tokens[4] == "where" && tokens[5] == "codigo") {
            int key = stoi(tokens[7]);
            if (seqFile.remove(key)) {
                cout << "Registro eliminado con exito.\n";
            } else {
                cout << "Error al eliminar el registro.\n";
            }
        }
    }


    void handleInsertAVL(vector<string>& tokens, AVLFile<int> avl) {
        if(tokens.size() >= 5) {

            vector<string> contenedor = extraerPalabras(tokens[4]);

            Record registro;
            registro.codigo = stoi(contenedor[0]);

            // Copiar el nombre y apellido usando strncpy
            strncpy(registro.nombre, contenedor[1].c_str(), sizeof(registro.nombre) - 1);
            registro.nombre[sizeof(registro.nombre) - 1] = '\0';

            strncpy(registro.apellido, contenedor[2].c_str(), sizeof(registro.apellido) - 1);
            registro.apellido[sizeof(registro.apellido) - 1] = '\0';

            registro.ciclo = stoi(contenedor[3]);
                cout<<registro.apellido;
            avl.insert(registro);
            cout << "Registro insertado correctamente en AVL.\n";

        }

    }

    //select avl
    void handleSelectAVL( vector<string>& tokens, int pos) {
        if (tokens.size() >= 5 && tokens[4] == "where" && tokens[5] == "codigo") {
            int key = stoi(tokens[7]);
            Record registro_b = avls[pos].find(key);
            registro_b.showData();
            //  diferenciar registros encontrados y no encontrados

        }
    }

    void handleDeleteAVL(vector<string>& tokens, AVLFile<int>& avl) {
        if (tokens.size() >= 5 && tokens[4] == "where" && tokens[5] == "codigo") {
            int key = stoi(tokens[7]);
            bool eliminado = avl.remove(key);

            if (eliminado) {
                cout << "Registro eliminado correctamente del AVL.\n";
            } else {
                cout << "Error: Registro no encontrado en AVL.\n";
            }
        }
    }
};
void prueba() {
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