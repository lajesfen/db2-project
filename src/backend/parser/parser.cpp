#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include "contenido.cpp"
#include "../formats/AVL.cpp"
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
vector<AVLFile<int>> avls;
public:
    void parse(const string& query) {
        auto tokens = tokenize(query);
        if (tokens.empty()) return;
        if (tokens[0] == "create" and tokens[1] == "table") {
            string nombre_archivo=tokens[2];
            nombre_archivo+=".dat";
            /*if(tokens[3]=="using" ) {
                if(tokens[4]=="AVL") {
                    /*nombre_archivo=nombre_archivo+=".dat";
                    AVLFile<string> avl(nombre_archivo);
                    vector<string> owo=extraerPalabras(tokens[6]);
                    string key=owo[0];
                    cout<<"hola";
                    cout<<owo[0];
                    avls.push_back(avl);#1#
                }

            }*/

             if(tokens[3]=="from") {
                string achirvo_csv=tokens[5];

                if(tokens[7]=="AVL") {
                        AVLFile<int> avl(nombre_archivo);
                        cout<<achirvo_csv;
                        avls.push_back(avl);
                }
            }
        }

        else if (tokens[0] == "insert" && tokens[1] == "into") {
            string nombre=tokens[2];
            nombre+=".dat";
            int enct=buscarAVLPorNombre(avls,nombre);
            cout<<avls.size();
            handleInsert(tokens, avls[enct]);
        }
        else if (tokens[0] == "select") {
            string nombrearchivo=tokens[3]+".dat";
            int enVect=buscarAVLPorNombre(avls,nombrearchivo);
            handleSelect(tokens,enVect);
        }
        /*else if (tokens[0] == "delete" && tokens[2] == "from") {
             string archivo=tokens[2]+".dat";
             int pos=buscarAVLPorNombre(avls,archivo);
             handleDelete(tokens,pos);

    }*/
    }

private:
    /*void handleCreateTable(const vector<string>& tokens) {

        cout << "Creando tabla " << tokens[2] << endl;
    }*/

    void handleInsert(vector<string>& tokens, AVLFile<int> avl) {
        while(tokens.size() >= 5) {
            cout << tokens[4];
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

            tokens.pop_back();
        }

    }


    void handleSelect( vector<string>& tokens, int pos) {
        if (tokens.size() >= 5 && tokens[4] == "where" && tokens[5] == "codigo") {
            int key = stoi(tokens[7]);
            cout<<key<<"   ksksks  ";
            Record registro_b=avls[pos].find(key);
            cout<<registro_b.nombre;
        }
    }

    /*void handleDelete(const vector<string>& tokens, int pos) {
        if (tokens.size() >= 5 && tokens[4] == "where" && tokens[5] == "codigo") {
            cout<<stoi(tokens[7])<<" o  ";
            bool eliminado=avls[pos].remove(stoi(tokens[7]));
            if(eliminado) {
                cout << "Cliente eliminado con codigo: " << tokens[7] << endl;
            }
            else if(!eliminado) {
                cout<<" no se elimino ";
            }
        }
    }*/
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