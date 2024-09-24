#ifndef PROJECT_SOCIALRECORD_H
#define PROJECT_SOCIALRECORD_H

#include <string>
#include "../include/json.hpp"
using json = nlohmann::json;

struct SocialRecord {
    int id;
    std::string nombre;
    std::string abreviatura;
    std::string url;
    std::string facebook;
    std::string twitter;
    std::string instagram;
    std::string linkedin;
    std::string youtube;
    std::string tiktok;

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
