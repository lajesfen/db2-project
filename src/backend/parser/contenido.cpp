#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::vector<std::vector<std::string>> extraerRegistros(const std::string& texto) {
    std::vector<std::vector<std::string>> todos_los_registros;

    size_t inicio = 0;
    while ((inicio = texto.find('(', inicio)) != std::string::npos) {
        inicio += 1;
        size_t fin = texto.find(')', inicio);

        if (fin != std::string::npos) {
            std::string contenido = texto.substr(inicio, fin - inicio);
            std::vector<std::string> registro_actual;

            bool inQuotes = false;
            std::string palabra;
            for (char c : contenido) {
                if (c == '"') {
                    inQuotes = !inQuotes;
                } else if (c == ',' && !inQuotes) {
                    registro_actual.push_back(trim(palabra));
                    palabra.clear();
                } else {
                    palabra += c;
                }
            }

            if (!palabra.empty()) {
                registro_actual.push_back(trim(palabra));
            }

            todos_los_registros.push_back(registro_actual);
            inicio = fin + 1;
        }
    }

    return todos_los_registros;
}