#ifndef PROJECT_HOSPITALRECORD_H
#define PROJECT_HOSPITALRECORD_H

#include <string>
#include "../include/json.hpp"
using json = nlohmann::json;

struct HospitalRecord {
    int id;
    std::string institucion;
    std::string nombre;
    std::string clasificacion;
    std::string tipo;
    std::string departamento;
    std::string provincia;
    std::string distrito;
    int ubigeo;
    std::string direccion;
    std::string telefono;
    std::string estado;
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
