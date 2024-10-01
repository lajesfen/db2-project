#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdexcept>

#define MAX_AUX_RECORD 50 // Máxima cantidad de registros

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

        std::fstream file(this->filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open())
        {
            file.open(this->filename, std::ios::out | std::ios::binary);
            file.close();
        }

        std::fstream auxFile(this->aux_filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!auxFile.is_open())
        {
            auxFile.open(this->aux_filename, std::ios::out | std::ios::binary);
            auxFile.close();
        }

        file.close();
        auxFile.close();
    }

    Registro search(TK key)
    {
        Registro reg;
        std::ifstream file(filename, std::ios::binary);

        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            if (reg.id == key)
            {
                file.close();
                return reg;
            }
        }
        file.close();

        file.open(aux_filename, std::ios::binary);
        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            if (reg.id == key)
            {
                file.close();
                return reg;
            }
        }
        file.close();

        Registro failReg;
        failReg.id = -1;
        return failReg;
    }

    std::vector<Registro> rangeSearch(TK begin_key, TK end_key)
    {
        std::vector<Registro> results;
        Registro reg;
        std::ifstream file(filename, std::ios::binary);

        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            if (reg.id >= begin_key && reg.id <= end_key)
            {
                results.push_back(reg);
            }
        }
        file.close();

        file.open(aux_filename, std::ios::binary);
        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            if (reg.id >= begin_key && reg.id <= end_key)
            {
                results.push_back(reg);
            }
        }
        file.close();

        return results;
    }

    void add(Registro registro)
    {
        Registro existingRecord = search(registro.id);
        if (existingRecord.id == registro.id)
        {
            return;
        }

        std::ofstream auxFile(aux_filename, std::ios::binary | std::ios::app);
        auxFile.write(reinterpret_cast<const char *>(&registro), sizeof(Registro));
        auxFile.close();

        if (getAuxRecordCount() >= MAX_AUX_RECORD)
        {
            mergeFiles();
        }
    }

    std::vector<Registro> getAllRecords()
    {
        std::vector<Registro> allRecords;
        Registro reg;

        std::ifstream file(filename, std::ios::binary);
        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            allRecords.push_back(reg);
        }
        file.close();

        file.open(aux_filename, std::ios::binary);
        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            allRecords.push_back(reg);
        }
        file.close();

        std::sort(allRecords.begin(), allRecords.end(), [](const Registro &a, const Registro &b)
                  { return a.id < b.id; });

        return allRecords;
    }

    bool remove(TK key)
    {
        bool found = false;
        std::vector<Registro> mainRecords;
        std::vector<Registro> auxRecords;
        Registro reg;

        std::ifstream file(filename, std::ios::binary);
        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            if (reg.id != key)
            {
                mainRecords.push_back(reg);
            }
            else
            {
                found = true;
            }
        }
        file.close();

        if (found)
        {
            std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
            for (const auto &r : mainRecords)
            {
                outFile.write(reinterpret_cast<const char *>(&r), sizeof(Registro));
            }
            outFile.close();
            return true; 
        }

        std::ifstream auxFile(aux_filename, std::ios::binary);
        while (auxFile.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            if (reg.id != key)
            {
                auxRecords.push_back(reg);
            }
            else
            {
                found = true; 
            }
        }
        auxFile.close();

        if (found)
        {
            std::ofstream outAuxFile(aux_filename, std::ios::binary | std::ios::trunc);
            for (const auto &r : auxRecords)
            {
                outAuxFile.write(reinterpret_cast<const char *>(&r), sizeof(Registro));
            }
            outAuxFile.close();
        }

        return found;
    }

    int getAuxRecordCount()
    {
        std::ifstream file(aux_filename, std::ios::binary);
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

        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            allRecords.push_back(reg);
        }
        file.close();

        file.open(aux_filename, std::ios::binary);

        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            allRecords.push_back(reg);
        }
        file.close();

        std::sort(allRecords.begin(), allRecords.end(), [](const Registro &a, const Registro &b)
                  { return a.id < b.id; });

        std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
        for (const auto &r : allRecords)
        {
            outFile.write(reinterpret_cast<const char *>(&r), sizeof(Registro));
        }
        outFile.close();

        std::ofstream auxFile(aux_filename, std::ios::binary | std::ios::trunc);
        auxFile.close();
    }

    // Para el parser
    friend int buscarSequentialPorNombre(const std::vector<SequentialFile<TK, Registro>> &sqfS, const std::string &nombreBuscado)
    {
        for (int i = 0; i < sqfS.size(); ++i)
        {
            if (sqfS[i].filename == nombreBuscado)
            {
                return i;
            }
        }
        return -1;
    }
};