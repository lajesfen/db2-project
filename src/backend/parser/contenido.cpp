#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::vector<std::string> extraerPalabras(const std::string& texto) {
    std::vector<std::string> palabras;

    size_t inicio = texto.find('(');
    if (inicio != std::string::npos) {
        inicio += 1;
        size_t fin = texto.find(')', inicio);
        if (fin != std::string::npos) {
            // Extraer el contenido dentro de los paréntesis
            std::string contenido = texto.substr(inicio, fin - inicio);

            size_t pos = 0;
            while ((pos = contenido.find(',')) != std::string::npos) {
                std::string palabra = contenido.substr(0, pos);
                palabras.push_back(trim(palabra));
                contenido.erase(0, pos + 1);
            }

            palabras.push_back(trim(contenido));
        }
    }

    return palabras;
}