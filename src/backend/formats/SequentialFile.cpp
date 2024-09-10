#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <stdexcept>

template <typename KT>
struct Record
{
    KT codigo;
    char nombre[12];
    char apellido[12];
    int ciclo;
    bool eliminado;

    void showData() const
    {
        std::cout << "\nCodigo: " << codigo;
        std::cout << "\nNombre: " << nombre;
        std::cout << "\nApellido: " << apellido;
        std::cout << "\nCiclo : " << ciclo;
        std::cout << "\nEliminado: " << (eliminado ? "Sí" : "No");
    }

    bool operator==(const Record &other) const
    {
        return codigo == other.codigo &&
               std::strcmp(nombre, other.nombre) == 0 &&
               std::strcmp(apellido, other.apellido) == 0 &&
               ciclo == other.ciclo &&
               eliminado == other.eliminado;
    }
};

template <typename KT>
class SequentialFile
{
private:
    std::string filename;
    std::string tempFilename = "temp_data.csv";

    Record<KT> parseLine(const std::string &line)
    {
        std::stringstream ss(line);
        std::string field;
        Record<KT> record;

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

    std::string recordToLine(const Record<KT> &record)
    {
        std::stringstream ss;
        ss << record.codigo << ','
           << record.nombre << ','
           << record.apellido << ','
           << record.ciclo << ','
           << (record.eliminado ? "1" : "0");
        return ss.str();
    }

    std::vector<Record<KT>> readAllRecords()
    {
        std::vector<Record<KT>> records;
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

    void writeAllRecords(const std::vector<Record<KT>> &records, const std::string &file)
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
        std::vector<Record<KT>> records = readAllRecords();
        std::vector<Record<KT>> activeRecords;

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
    SequentialFile(const std::string &file) : filename(file) {}

    Record<KT> search(KT key)
    {
        std::vector<Record<KT>> records = readAllRecords();
        for (const auto &record : records)
        {
            if (record.codigo == key && !record.eliminado)
            {
                return record;
            }
        }
        throw std::runtime_error("Record not found");
    }

    std::vector<Record<KT>> rangeSearch(KT begin_key, KT end_key)
    {
        std::vector<Record<KT>> records = readAllRecords();
        std::vector<Record<KT>> result;
        for (const auto &record : records)
        {
            if (record.codigo >= begin_key && record.codigo <= end_key && !record.eliminado)
            {
                result.push_back(record);
            }
        }
        return result;
    }

    bool add(const Record<KT> &record)
    {
        std::vector<Record<KT>> records = readAllRecords();
        records.push_back(record);
        writeAllRecords(records, filename);
        return true;
    }

    bool remove(KT key)
    {
        std::vector<Record<KT>> records = readAllRecords();
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
};