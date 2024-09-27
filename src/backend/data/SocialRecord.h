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
