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

    auto toJSON() {
        json res;
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
