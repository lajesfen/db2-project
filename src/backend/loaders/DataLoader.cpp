#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "../data/HospitalRecord.h"
#include "../data/SocialRecord.h"

std::vector<HospitalRecord> readHospitalFromCSV() {
    std::vector<HospitalRecord> res;
    std::ifstream file("hospitalesopendata.csv");
    std::string line;

    getline(file, line);
    while(getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        HospitalRecord record;

        getline(ss, token, '\t');
        strncpy(record.institucion, token.c_str(), sizeof(record.institucion) - 1);
        record.institucion[sizeof(record.institucion) - 1] = '\0';  // Null-terminate

        getline(ss, token, '\t');
        record.id = stoi(token);

        getline(ss, token, '\t');
        strncpy(record.nombre, token.c_str(), sizeof(record.nombre) - 1);
        record.nombre[sizeof(record.nombre) - 1] = '\0';  // Null-terminate

        getline(ss, token, '\t');
        strncpy(record.clasificacion, token.c_str(), sizeof(record.clasificacion) - 1);
        record.clasificacion[sizeof(record.clasificacion) - 1] = '\0';  // Null-terminate

        getline(ss, token, '\t');
        strncpy(record.tipo, token.c_str(), sizeof(record.tipo) - 1);
        record.tipo[sizeof(record.tipo) - 1] = '\0';  // Null-terminate

        getline(ss, token, '\t');
        strncpy(record.departamento, token.c_str(), sizeof(record.departamento) - 1);
        record.departamento[sizeof(record.departamento) - 1] = '\0';  // Null-terminate

        getline(ss, token, '\t');
        strncpy(record.provincia, token.c_str(), sizeof(record.provincia) - 1);
        record.provincia[sizeof(record.provincia) - 1] = '\0';  // Null-terminate

        getline(ss, token, '\t');
        strncpy(record.distrito, token.c_str(), sizeof(record.distrito) - 1);
        record.distrito[sizeof(record.distrito) - 1] = '\0';  // Null-terminate

        getline(ss, token, '\t');
        record.ubigeo = stoi(token);

        getline(ss, token, '\t');
        strncpy(record.direccion, token.c_str(), sizeof(record.direccion) - 1);
        record.direccion[sizeof(record.direccion) - 1] = '\0';  // Null-terminate

        for (int i = 0; i < 9; ++i) {
            getline(ss, token, '\t');
        }

        getline(ss, token, '\t');
        strncpy(record.telefono, token.c_str(), sizeof(record.telefono) - 1);
        record.telefono[sizeof(record.telefono) - 1] = '\0';  // Null-terminate

        for (int i = 0; i < 5; ++i) {
            getline(ss, token, '\t');
        }

        getline(ss, token, '\t');
        strncpy(record.estado, token.c_str(), sizeof(record.estado) - 1);
        record.estado[sizeof(record.estado) - 1] = '\0';  // Null-terminate

        for (int i = 0; i < 3; ++i) {
            getline(ss, token, '\t');
        }

        getline(ss, token, '\t');
        if (token.empty()) {
            record.camas = 0;
        } else {
            record.camas = stoi(token);
        }

        getline(ss, token, '\t');
        record.ruc = stol(token);

        res.push_back(record);
    }

    file.close();
    return res;
}

std::vector<SocialRecord> readSocialFromCSV() {
    std::vector<SocialRecord> res;
    std::ifstream file("directorioredes.csv");
    std::string line;

    getline(file, line);
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        SocialRecord record;

        getline(ss, token, '\t');
        record.id = stoi(token);

        getline(ss, token, '\t');
        strncpy(record.nombre, token.c_str(), sizeof(record.nombre) - 1);
        record.nombre[sizeof(record.nombre) - 1] = '\0';  

        getline(ss, token, '\t');
        strncpy(record.abreviatura, token.c_str(), sizeof(record.abreviatura) - 1);
        record.abreviatura[sizeof(record.abreviatura) - 1] = '\0';  

        getline(ss, token, '\t');

        getline(ss, token, '\t');
        strncpy(record.url, token.c_str(), sizeof(record.url) - 1);
        record.url[sizeof(record.url) - 1] = '\0';  

        getline(ss, token, '\t');
        strncpy(record.facebook, token.c_str(), sizeof(record.facebook) - 1);
        record.facebook[sizeof(record.facebook) - 1] = '\0';  

        getline(ss, token, '\t');
        strncpy(record.twitter, token.c_str(), sizeof(record.twitter) - 1);
        record.twitter[sizeof(record.twitter) - 1] = '\0';  

        getline(ss, token, '\t');
        strncpy(record.instagram, token.c_str(), sizeof(record.instagram) - 1);
        record.instagram[sizeof(record.instagram) - 1] = '\0';  

        getline(ss, token, '\t');
        strncpy(record.linkedin, token.c_str(), sizeof(record.linkedin) - 1);
        record.linkedin[sizeof(record.linkedin) - 1] = '\0';  

        getline(ss, token, '\t');
        strncpy(record.youtube, token.c_str(), sizeof(record.youtube) - 1);
        record.youtube[sizeof(record.youtube) - 1] = '\0';  

        getline(ss, token, '\t');
        strncpy(record.tiktok, token.c_str(), sizeof(record.tiktok) - 1);
        record.tiktok[sizeof(record.tiktok) - 1] = '\0';  

        res.push_back(record);
    }

    file.close();
    return res;
}