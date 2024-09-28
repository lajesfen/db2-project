#ifndef PROJECT_HOSPITALRECORD_H
#define PROJECT_HOSPITALRECORD_H

#include <string>
#include "../include/json.hpp"
using json = nlohmann::json;

struct HospitalRecord {
    int id;
    char institucion[50];
    char nombre[100];
    char clasificacion[100];
    char tipo[50];
    char departamento[30];
    char provincia[30];
    char distrito[30];
    int ubigeo;
    char direccion[180];
    char telefono[40];
    char estado[8];
    int camas;
    long ruc;

    // Atributos AVL:
    long left = -1;
    long right = -1;
    int height = 0;

    // Atributos Sequential File
    bool deleted = false;

    void llenarDatos(std::vector<std::string> datos) {
        id = std::stoi(datos[0]);
        strncpy(institucion, datos[1].c_str(), sizeof(institucion) - 1);
        institucion[sizeof(institucion) - 1] = '\0';

        strncpy(nombre, datos[2].c_str(), sizeof(nombre) - 1);
        nombre[sizeof(nombre) - 1] = '\0';

        strncpy(clasificacion, datos[3].c_str(), sizeof(clasificacion) - 1);
        clasificacion[sizeof(clasificacion) - 1] = '\0';

        strncpy(tipo, datos[4].c_str(), sizeof(tipo) - 1);
        tipo[sizeof(tipo) - 1] = '\0';

        strncpy(departamento, datos[5].c_str(), sizeof(departamento) - 1);
        departamento[sizeof(departamento) - 1] = '\0';

        strncpy(provincia, datos[6].c_str(), sizeof(provincia) - 1);
        provincia[sizeof(provincia) - 1] = '\0';

        strncpy(distrito, datos[7].c_str(), sizeof(distrito) - 1);
        distrito[sizeof(distrito) - 1] = '\0';

        ubigeo = std::stoi(datos[8]);

        strncpy(direccion, datos[9].c_str(), sizeof(direccion) - 1);
        direccion[sizeof(direccion) - 1] = '\0';

        strncpy(telefono, datos[10].c_str(), sizeof(telefono) - 1);
        telefono[sizeof(telefono) - 1] = '\0';

        strncpy(estado, datos[11].c_str(), sizeof(estado) - 1);
        estado[sizeof(estado) - 1] = '\0';
        camas = std::stoi(datos[12]);

        ruc=std::stoll(datos[13]);
    }
    void mostrarDatos() {
        std::cout << "ID: " << id << ", Nombre: " << nombre << " | "<<left<<"  |"<<right <<" "<<deleted<<std::endl;
    }

    auto toJSON() {
        nlohmann::ordered_json res;
        res["id"] = id;
        res["institucion"] = institucion;
        res["nombre"] = nombre;
        res["clasificacion"] = clasificacion;
        res["tipo"] = tipo;
        res["departamento"] = departamento;
        res["provincia"] = provincia;
        res["distrito"] = distrito;
        res["ubigeo"] = ubigeo;
        res["direccion"] = direccion;
        res["telefono"] = telefono;
        res["estado"] = estado;
        res["camas"] = camas;
        res["ruc"] = ruc;

        return res;
    }
};

#endif //PROJECT_HOSPITALRECORD_H
