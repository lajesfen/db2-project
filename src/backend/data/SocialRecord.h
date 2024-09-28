#ifndef PROJECT_SOCIALRECORD_H
#define PROJECT_SOCIALRECORD_H

#include <string>
#include "../include/json.hpp"
using json = nlohmann::json;

struct SocialRecord {
    int id;
    char nombre[130];
    char abreviatura[40];
    char url[50];
    char facebook[60];
    char twitter[60];
    char instagram[60];
    char linkedin[80];
    char youtube[90];
    char tiktok[60];

    // Atributos AVL:
    long left = -1;
    long right = -1;
    int height = 0;

    // Atributos Sequential File
    bool deleted = false;

    void llenarDatos(std::vector<std::string> datos) {
        id = std::stoi(datos[0]);

        strncpy(nombre, datos[1].c_str(), sizeof(nombre) - 1);
        nombre[sizeof(nombre) - 1] = '\0';

        strncpy(abreviatura, datos[2].c_str(), sizeof(abreviatura) - 1);
        abreviatura[sizeof(abreviatura) - 1] = '\0';

        strncpy(url, datos[3].c_str(), sizeof(url) - 1);
        url[sizeof(url) - 1] = '\0';

        strncpy(facebook, datos[4].c_str(), sizeof(facebook) - 1);
        facebook[sizeof(facebook) - 1] = '\0';

        strncpy(twitter, datos[5].c_str(), sizeof(twitter) - 1);
        twitter[sizeof(twitter) - 1] = '\0';

        strncpy(instagram, datos[6].c_str(), sizeof(instagram) - 1);
        instagram[sizeof(instagram) - 1] = '\0';

        strncpy(linkedin, datos[7].c_str(), sizeof(linkedin) - 1);
        linkedin[sizeof(linkedin) - 1] = '\0';

        strncpy(youtube, datos[8].c_str(), sizeof(youtube) - 1);
        youtube[sizeof(youtube) - 1] = '\0';

        strncpy(tiktok, datos[9].c_str(), sizeof(tiktok) - 1);
        tiktok[sizeof(tiktok) - 1] = '\0';
    }


    void mostrarDatos() {
        std::cout << "ID: " << id << ", Nombre: " << nombre <<  std::endl;
    }

    auto toJSON() {
        json res;
        res["id"] = id;
        res["nombre"] = nombre;
        res["abreviatura"] = abreviatura;
        res["url"] = url;
        res["facebook"] = facebook;
        res["twitter"] = twitter;
        res["instagram"] = instagram;
        res["linkedin"] = linkedin;
        res["youtube"] = youtube;
        res["tiktok"] = tiktok;

        return res;
    }
};

#endif //PROJECT_SOCIALRECORD_H
