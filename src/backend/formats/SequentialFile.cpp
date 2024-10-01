#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdexcept>

#define MAX_AUX_RECORD 40 // Máxima cantidad de registros

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
        if (searchInFile(filename, key, reg))
        {
            return reg;
        }
        if (searchInFile(aux_filename, key, reg))
        {
            return reg;
        }

        Registro failReg{};
        failReg.id = -1;
        return failReg;
    }

    bool searchInFile(const std::string &filename, TK key, Registro &result)
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open())
            return false;

        file.seekg(0, std::ios::end);
        int fileSize = file.tellg();
        int recordCount = fileSize / sizeof(Registro);

        int left = 0;
        int right = recordCount - 1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2;

            file.seekg(mid * sizeof(Registro), std::ios::beg);
            file.read(reinterpret_cast<char *>(&result), sizeof(Registro));

            if (result.deleted)
            {
                left = mid + 1;
                continue;
            }

            if (result.id == key)
            {
                file.close();
                return true;
            }
            else if (result.id < key)
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }

        file.close();
        return false;
    }

    std::vector<Registro> rangeSearch(TK begin_key, TK end_key)
    {
        std::vector<Registro> results;
        readRecordsInRange(filename, begin_key, end_key, results);
        readRecordsInRange(aux_filename, begin_key, end_key, results);
        return results;
    }

    void readRecordsInRange(const std::string &filename, TK begin_key, TK end_key, std::vector<Registro> &results)
    {
        Registro reg;
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open())
            return;

        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            if (!reg.deleted && reg.id >= begin_key && reg.id <= end_key)
            {
                results.push_back(reg);
            }
        }
        file.close();
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

    bool remove(TK key)
    {
        if (removeFromFile(filename, key))
        {
            return true;
        }
        return removeFromFile(aux_filename, key);
    }

    bool removeFromFile(const std::string &filename, TK key)
    {
        bool found = false;
        Registro reg;

        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open())
            return false;

        std::vector<Registro> records;
        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            records.push_back(reg);
        }
        file.close();

        auto it = std::lower_bound(records.begin(), records.end(), key, [](const Registro &r, TK key)
                                   { return r.id < key; });

        if (it != records.end() && it->id == key && !it->deleted)
        {
            found = true;
            it->deleted = true;
        }

        if (found)
        {
            std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
            for (const auto &r : records)
            {
                outFile.write(reinterpret_cast<const char *>(&r), sizeof(Registro));
            }
            outFile.close();
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

        readAllRecords(filename, allRecords);
        readAllRecords(aux_filename, allRecords);

        std::sort(allRecords.begin(), allRecords.end(), [](const Registro &a, const Registro &b)
                  { return a.id < b.id; });

        std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
        for (const auto &r : allRecords)
        {
            if (!r.deleted)
            {
                outFile.write(reinterpret_cast<const char *>(&r), sizeof(Registro));
            }
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

    void readAllRecords(const std::string &filename, std::vector<Registro> &allRecords)
    {
        Registro reg;
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open())
            return;

        while (file.read(reinterpret_cast<char *>(&reg), sizeof(Registro)))
        {
            allRecords.push_back(reg);
        }
        file.close();
    }
};
