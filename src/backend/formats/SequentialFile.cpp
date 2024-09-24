#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <stdexcept>

template <typename KT, typename RecordType>
class SequentialFile
{
private:
    std::string filename;
    std::string tempFilename = "temp_data.csv";

    RecordType parseLine(const std::string &line)
    {
        std::stringstream ss(line);
        std::string field;
        RecordType record;

        std::getline(ss, field, ',');
        record.codigo = static_cast<KT>(std::stoi(field));
        std::getline(ss, field, ',');
        std::strncpy(record.nombre, field.c_str(), sizeof(record.nombre));
        std::getline(ss, field, ',');
        std::strncpy(record.apellido, field.c_str(), sizeof(record.apellido));
        std::getline(ss, field, ',');
        record.ciclo = std::stoi(field);
        std::getline(ss, field, ',');
        record.eliminado = (field == "1");

        return record;
    }

    std::string recordToLine(const RecordType &record)
    {
        std::stringstream ss;
        ss << record.codigo << ','
           << record.nombre << ','
           << record.apellido << ','
           << record.ciclo << ','
           << (record.eliminado ? "1" : "0");
        return ss.str();
    }

    std::vector<RecordType> readAllRecords()
    {
        std::vector<RecordType> records;
        std::ifstream file(filename);
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                if (!line.empty())
                {
                    records.push_back(parseLine(line));
                }
            }
            file.close();
        }
        return records;
    }

    void writeAllRecords(const std::vector<RecordType> &records, const std::string &file)
    {
        std::ofstream fileStream(file);
        if (fileStream.is_open())
        {
            for (const auto &record : records)
            {
                fileStream << recordToLine(record) << '\n';
            }
            fileStream.close();
        }
    }

    void compact()
    {
        std::vector<RecordType> records = readAllRecords();
        std::vector<RecordType> activeRecords;

        for (const auto &record : records)
        {
            if (!record.eliminado)
            {
                activeRecords.push_back(record);
            }
        }

        writeAllRecords(activeRecords, tempFilename);

        std::remove(filename.c_str());
        std::rename(tempFilename.c_str(), filename.c_str());
    }

public:
    SequentialFile(const std::string &file) : filename(file) {
        std::ifstream archivo_existente(filename);
        if(!archivo_existente.is_open())
            std::ofstream archivo(filename);
        archivo_existente.close();
    }

    RecordType search(KT key)
    {
        std::vector<RecordType> records = readAllRecords();
        for (const auto &record : records)
        {
            if (record.codigo == key && !record.eliminado)
            {
                return record;
            }
        }
        throw std::runtime_error("Record not found");
    }

    std::vector<RecordType> rangeSearch(KT begin_key, KT end_key)
    {
        std::vector<RecordType> records = readAllRecords();
        std::vector<RecordType> result;
        for (const auto &record : records)
        {
            if (record.codigo >= begin_key && record.codigo <= end_key && !record.eliminado)
            {
                result.push_back(record);
            }
        }
        return result;
    }

    bool add(const RecordType &record)
    {
        std::vector<RecordType> records = readAllRecords();
        records.push_back(record);
        writeAllRecords(records, filename);
        return true;
    }

    bool remove(KT key)
    {
        std::vector<RecordType> records = readAllRecords();
        bool removed = false;

        for (auto &record : records)
        {
            if (record.codigo == key)
            {
                record.eliminado = true;
                removed = true;
            }
        }

        if (removed)
        {
            writeAllRecords(records, filename);
            compact();
            return true;
        }

        return false;
    }
    friend int buscarSequentialPorNombre( std::vector<SequentialFile> sequentialFiles, const std::string& nombre) {
        for (int i = 0; i < sequentialFiles.size(); ++i) {
            if (sequentialFiles[i].filename==nombre) {
                return i;
            }
        }
        return -1;
    }
};