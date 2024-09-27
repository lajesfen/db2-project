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
        record.institucion = token;

        getline(ss, token, '\t');
        record.id = stoi(token);

        getline(ss, token, '\t');
        record.nombre = token;

        getline(ss, token, '\t');
        record.clasificacion = token;

        getline(ss, token, '\t');
        record.tipo = token;

        getline(ss, token, '\t');
        record.departamento = token;

        getline(ss, token, '\t');
        record.provincia = token;

        getline(ss, token, '\t');
        record.distrito = token;

        getline(ss, token, '\t');
        record.ubigeo = stoi(token);

        getline(ss, token, '\t');
        record.direccion = token;

        getline(ss, token, '\t');
        getline(ss, token, '\t');
        getline(ss, token, '\t');
        getline(ss, token, '\t');
        getline(ss, token, '\t');
        getline(ss, token, '\t');
        getline(ss, token, '\t');
        getline(ss, token, '\t');
        getline(ss, token, '\t');

        getline(ss, token, '\t');
        record.telefono = token;

        getline(ss, token, '\t');
        getline(ss, token, '\t');
        getline(ss, token, '\t');
        getline(ss, token, '\t');
        getline(ss, token, '\t');

        getline(ss, token, '\t');
        record.estado = token;

        getline(ss, token, '\t');
        getline(ss, token, '\t');
        getline(ss, token, '\t');

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
    std::ifstream file("directorioderes.csv");
    std::string line;

    getline(file, line);
    while(getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        SocialRecord record;

        getline(ss, token, '\t');
        record.id = stoi(token);

        getline(ss, token, '\t');
        record.nombre = token;

        getline(ss, token, '\t');
        record.abreviatura = token;

        getline(ss, token, '\t');

        getline(ss, token, '\t');
        record.url = token;

        getline(ss, token, '\t');
        record.facebook = token;

        getline(ss, token, '\t');
        record.twitter = token;

        getline(ss, token, '\t');
        record.instagram = token;

        getline(ss, token, '\t');
        record.linkedin = token;

        getline(ss, token, '\t');
        record.youtube = token;

        getline(ss, token, '\t');
        record.tiktok = token;

        res.push_back(record);
    }

    file.close();
    return res;
}