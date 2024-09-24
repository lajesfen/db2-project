#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdexcept>

#define MAX_AUX_RECORD 5 // Máxima cantidad de registros

template <typename TK, typename Registro>
class SequentialFile
{
private:
    std::string filename;
    std::string aux_filename;

public:
    SequentialFile(std::string filename, std::string aux_filename = "")
    {
        this->filename = filename;

        if (aux_filename.empty())
        {
            this->aux_filename = "aux_" + filename;
        }
        else
        {
            this->aux_filename = aux_filename;
        }
    }

    std::vector<Registro> search(TK key)
    {
        std::vector<Registro> results;
        Registro reg;
        std::ifstream file(filename, std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("No se pudo abrir el archivo principal");
        }

        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            if (reg.key == key)
            {
                results.push_back(reg);
            }
        }
        file.close();

        file.open(aux_filename, std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("No se pudo abrir el archivo auxiliar");
        }
        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            if (reg.key == key)
            {
                results.push_back(reg);
            }
        }
        file.close();

        return results;
    }

    std::vector<Registro> rangeSearch(TK begin_key, TK end_key)
    {
        std::vector<Registro> results;
        Registro reg;
        std::ifstream file(filename, std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("No se pudo abrir el archivo principal");
        }

        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            if (reg.key >= begin_key && reg.key <= end_key)
            {
                results.push_back(reg);
            }
        }
        file.close();

        file.open(aux_filename, std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("No se pudo abrir el archivo auxiliar");
        }
        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            if (reg.key >= begin_key && reg.key <= end_key)
            {
                results.push_back(reg);
            }
        }
        file.close();

        return results;
    }

    bool add(Registro registro)
    {
        std::ofstream auxFile(aux_filename, std::ios::binary | std::ios::app);

        if (!auxFile.is_open())
        {
            throw std::runtime_error("No se pudo abrir el archivo auxiliar");
        }

        auxFile.write(reinterpret_cast<const char *>(&registro), sizeof(Registro));
        auxFile.close();

        if (getAuxRecordCount() >= MAX_AUX_RECORD)
        {
            mergeFiles();
        }

        return true;
    }

    bool remove(TK key)
    {
        bool found = false;
        std::vector<Registro> registros;
        Registro reg;

        std::ifstream file(filename, std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("No se pudo abrir el archivo principal");
        }

        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            if (reg.key != key)
            {
                registros.push_back(reg);
            }
            else
            {
                found = true;
            }
        }
        file.close();

        if (!found)
        {
            return false;
        }

        std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);

        if (!outFile.is_open())
        {
            throw std::runtime_error(" No se pudo abrir el archivo principal");
        }

        for (const auto &r : registros)
        {
            outFile.write(reinterpret_cast<const char *>(&r), sizeof(Registro));
        }
        outFile.close();

        return true;
    }

    int getAuxRecordCount()
    {
        std::ifstream file(aux_filename, std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("No se pudo abrir el archivo auxiliar");
        }

        file.seekg(0, std::ios::end);
        int count = file.tellg() / sizeof(Registro);
        file.close();
        return count;
    }

    void mergeFiles()
    {
        std::vector<Registro> allRecords;
        Registro reg;

        std::ifstream file(filename, std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("No se pudo abrir el archivo principal");
        }

        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            allRecords.push_back(reg);
        }
        file.close();

        file.open(aux_filename, std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("No se pudo abrir el archivo auxiliar");
        }

        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            allRecords.push_back(reg);
        }
        file.close();

        std::sort(allRecords.begin(), allRecords.end(), [](const Registro &a, const Registro &b)
                  { return a.key < b.key; });

        std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
        if (!outFile.is_open())
        {
            throw std::runtime_error("No se pudo abrir el archivo principal");
        }

        for (const auto &r : allRecords)
        {
            outFile.write(reinterpret_cast<const char *>(&r), sizeof(Registro));
        }
        outFile.close();

        std::ofstream auxFile(aux_filename, std::ios::binary | std::ios::trunc);
        if (!auxFile.is_open())
        {
            throw std::runtime_error("No se pudo abrir el archivo auxiliar");
        }
        auxFile.close();
    }
};