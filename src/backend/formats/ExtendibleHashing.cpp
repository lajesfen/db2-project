#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <cmath>
#include <fstream>
#include <array>
#include <sstream>
#include <cstring>

#define MAX_DEPTH 7 // Profundidad máxima
#define MAX_FILL 10  // Factor de llenado
using namespace std;
struct Record
{
    int codigo;
    char nombre[12];
    char apellido[12];
    int ciclo;

    void showData() const
    {
        std::cout << "\nCodigo: " << codigo;
    }
};

class Bucket
{
public:
    std::vector<Record> records;
    int depth;

    Bucket(int d) : depth(d) {}

    bool isFull() const
    {
        return records.size() >= MAX_FILL;
    }

    void insert(const Record &record)
    {
        records.push_back(record);
    }

    void clear()
    {
        records.clear();
    }
};

class ExtendibleHashing
{
private:
    std::vector<Bucket *> buckets;

public:
    ExtendibleHashing()
    {
        buckets.push_back(new Bucket(1));
        buckets.push_back(new Bucket(1));

        std::ofstream outFile("index.dat", std::ios::binary | std::ios::trunc);
        if (outFile)
        {
            for (int i = 0; i < (1 << MAX_DEPTH); ++i)
            {
                outFile.write(reinterpret_cast<const char *>(&i), sizeof(i));
                int a = i % 2;
                outFile.write(reinterpret_cast<const char *>(&a), sizeof(a));
            }
            std::cout << "Index saved to index.dat." << std::endl;
        }
        else
        {
            std::cout << "Failed to open index.dat for writing." << std::endl;
        }
    }

    ~ExtendibleHashing()
    {
        for (Bucket *bucket : buckets)
        {
            delete bucket;
        }
    }

    int hashFunction(int key) const
    {
        return key % (1 << MAX_DEPTH);
    }

    Record find(int codi)
    {
        int hashValue = hashFunction(codi);
        int bucketIndex = get_bucket_index(hashValue);

        for (const auto &rec : buckets[bucketIndex]->records)
        {
            if (rec.codigo == codi)
            {
                return rec;
            }
        }

                    return Record{};


    }

    void insert(const Record &record)
    {
        int hashValue = hashFunction(record.codigo);
        int bucketIndex = get_bucket_index(hashValue);

        for (const auto &rec : buckets[bucketIndex]->records)
        {
            if (rec.codigo == record.codigo)
            {
                std::cout << "Alredy exists " << std::endl;
                return;
            }
        }

        if (buckets[bucketIndex]->isFull())
        {
            split(bucketIndex);
            bucketIndex = get_bucket_index(hashValue);
        }

        buckets[bucketIndex]->insert(record);
    }

    void split(int bucketIndex)
    {
        Bucket *oldBucket = buckets[bucketIndex];
        int oldDepth = oldBucket->depth;

        if (oldDepth >= MAX_DEPTH)
        {
            std::cout << "Cannot split, maximum depth reached!" << std::endl;
            return;
        }

        Bucket *newBucket = new Bucket(oldDepth + 1);
        buckets.push_back(newBucket);
        int newIndex = buckets.size() - 1;

        std::fstream indexFile("index.dat", std::ios::in | std::ios::out | std::ios::binary);
        if (!indexFile)
        {
            std::cerr << "Failed to open index.dat" << std::endl;
            return;
        }

        for (size_t i = 0; i < (1 << MAX_DEPTH); ++i)
        {
            int xdd, currentBucketIndex;
            indexFile.read(reinterpret_cast<char *>(&xdd), sizeof(xdd));
            indexFile.read(reinterpret_cast<char *>(&currentBucketIndex), sizeof(currentBucketIndex));

            if (currentBucketIndex == bucketIndex)
            {
                if ((i >> oldDepth) & 1)
                {
                    indexFile.seekg(i * (sizeof(int) + sizeof(int)));
                    indexFile.write(reinterpret_cast<const char *>(&xdd), sizeof(xdd));
                    indexFile.write(reinterpret_cast<const char *>(&newIndex), sizeof(newIndex));
                }
            }
            indexFile.seekg(i * (sizeof(int) + sizeof(int)));
            indexFile.read(reinterpret_cast<char *>(&xdd), sizeof(xdd));
            indexFile.read(reinterpret_cast<char *>(&currentBucketIndex), sizeof(currentBucketIndex));
        }

        std::cout << " ======================================= " << std::endl;

        indexFile.close();
        std::fstream xddd("index.dat", std::ios::in | std::ios::out | std::ios::binary);

        for (size_t i = 0; i < (1 << MAX_DEPTH); ++i)
        {
            int xdd, currentBucketIndex;
            xddd.read(reinterpret_cast<char *>(&xdd), sizeof(xdd));
            xddd.read(reinterpret_cast<char *>(&currentBucketIndex), sizeof(currentBucketIndex));
            std::cout << xdd << " : " << currentBucketIndex << std::endl;
            xddd.seekg(i * (sizeof(int) + sizeof(int)));
            xddd.read(reinterpret_cast<char *>(&xdd), sizeof(xdd));
            xddd.read(reinterpret_cast<char *>(&currentBucketIndex), sizeof(currentBucketIndex));
            std::cout << xdd << " -* " << currentBucketIndex << std::endl;
        }

        xddd.close();

        std::vector<Record> recordsToRedistribute = oldBucket->records;
        oldBucket->clear();
        for (const Record &record : recordsToRedistribute)
        {
            insert(record);
        }

        oldBucket->depth++;
        newBucket->depth = oldBucket->depth;
    }

    int get_bucket_index(int i)
    {
        std::ifstream inFile("index.dat", std::ios::binary);
        if (inFile)
        {
            int bucketIndex;
            inFile.seekg(i * (sizeof(int) + sizeof(int)) + sizeof(int));
            inFile.read(reinterpret_cast<char *>(&bucketIndex), sizeof(bucketIndex));
            inFile.close();
            return bucketIndex;
        }

        return -1;
    }

    void display_index_dat() const
    {
        std::ifstream inFile("index.dat", std::ios::binary);
        if (inFile)
        {
            std::cout << "\nContents of index.dat:\n";
            std::cout << "binary  #bucket\n";
            for (size_t i = 0; i < (1 << MAX_DEPTH); ++i)
            {
                int indexValue, bucketIndex;

                if (!inFile.read(reinterpret_cast<char *>(&indexValue), sizeof(indexValue)))
                {
                    std::cout << "Error reading index.dat." << std::endl;
                    break;
                }

                if (!inFile.read(reinterpret_cast<char *>(&bucketIndex), sizeof(bucketIndex)))
                {
                    std::cout << "Error reading index.dat." << std::endl;
                    break;
                }

                std::cout << std::bitset<MAX_DEPTH>(indexValue) << "    " << bucketIndex << "\n";
            }
            inFile.close();
        }
        else
        {
            std::cout << "Failed to open index.dat." << std::endl;
        }
    }

    void displayBuckets() const
    {
        std::cout << "\ndatafile.dat:\n";
        std::cout << "bucket  depth  records\n";
        for (size_t i = 0; i < buckets.size(); ++i)
        {
            std::cout << " " << i << "      " << buckets[i]->depth << "    ";
            for (const Record &rec : buckets[i]->records)
            {
                std::cout << rec.codigo << " ";
            }
            std::cout << "\n";
        }
    }

    void saveDatafile() const
    {
        std::ofstream outFile("datafile.dat", std::ios::binary | std::ios::trunc);
        if (outFile)
        {
            for (const Bucket *bucket : buckets)
            {
                for (const Record &record : bucket->records)
                {
                    outFile.write(reinterpret_cast<const char *>(&record), sizeof(Record));
                }
                Record emptyRecord = {0, "", "", 0}; // Registro vacío
                for (size_t i = bucket->records.size(); i < MAX_FILL; ++i)
                {
                    outFile.write(reinterpret_cast<const char *>(&emptyRecord), sizeof(Record));
                }
                outFile.write(reinterpret_cast<const char *>(&bucket->depth), sizeof(bucket->depth));
            }
            std::cout << "Records saved to datafile.dat." << std::endl;
        }
        else
        {
            std::cout << "Failed to open datafile.dat for writing." << std::endl;
        }
    }

    void loadDatafile()
    {
        std::ifstream inFile("datafile.dat", std::ios::binary);
        if (inFile)
        {
            while (!inFile.eof())
            {
                Bucket *bucket = new Bucket(0);
                for (size_t i = 0; i < MAX_FILL; ++i)
                {
                    Record record;
                    inFile.read(reinterpret_cast<char *>(&record), sizeof(Record));
                    if (inFile.gcount() == sizeof(Record))
                    {
                        bucket->insert(record);
                    }
                }

                // Leer la profundidad del bucket
                int depth;
                inFile.read(reinterpret_cast<char *>(&depth), sizeof(depth));
                if (inFile.gcount() == sizeof(depth))
                {
                    bucket->depth = depth;
                    buckets.push_back(bucket);
                }
                else
                {
                    delete bucket;
                    break;
                }
            }
            std::cout << "Records loaded from datafile.dat." << std::endl;
        }
        else
        {
            std::cout << "Failed to open datafile.dat for reading." << std::endl;
        }
    }

    void printDatafile() const
    {
        std::ifstream inFile("datafile.dat", std::ios::binary);
        if (inFile)
        {
            std::cout << "Contents of datafile.dat:\n";
            std::cout << "bucket  records           depth\n";

            int bucketIndex = 0;
            while (!inFile.eof())
            {
                Bucket bucket(0);
                for (size_t i = 0; i < MAX_FILL; ++i)
                {
                    Record record;
                    inFile.read(reinterpret_cast<char *>(&record), sizeof(Record));
                    if (inFile.gcount() == sizeof(Record))
                    {
                        bucket.insert(record);
                    }
                }

                int depth;
                inFile.read(reinterpret_cast<char *>(&depth), sizeof(depth));
                if (inFile.gcount() == sizeof(depth))
                {
                    std::cout << bucketIndex << "      ";
                    for (const Record &rec : bucket.records)
                    {
                        std::cout << rec.codigo << " ";
                    }
                    std::cout << "         " << depth << "\n";
                    bucketIndex++;
                }
                else
                {
                    break;
                }
            }
            inFile.close();
        }
        else
        {
            std::cout << "Failed to open datafile.dat for reading." << std::endl;
        }
    }
};




vector<Record> leerCSV(const string &filename)
{
    ifstream file(filename);
    vector<Record> records;
    string line;
    if (!file.is_open())
    {
        cerr << "No se pudo abrir el archivo.\n";
        return records;
    }
    if (getline(file, line))
    {
 
    }

    while (getline(file, line))
    {
        stringstream ss(line);
        string token;
        Record record;

        try
        {
            getline(ss, token, ',');
            record.codigo = stoi(token);

            getline(ss, token, ',');
            strncpy(record.nombre, token.c_str(), sizeof(record.nombre));
            record.nombre[sizeof(record.nombre) - 1] = '\0';

            getline(ss, token, ',');
            strncpy(record.apellido, token.c_str(), sizeof(record.apellido));
            record.apellido[sizeof(record.apellido) - 1] = '\0';

            getline(ss, token, ',');
            record.ciclo = stoi(token);

            records.push_back(record);
        }
        catch (const invalid_argument &e)
        {
            cerr << "Error de conversión: " << e.what() << " en la línea: " << line << endl;
        }
        catch (const out_of_range &e)
        {
            cerr << "Valor fuera de rango: " << e.what() << " en la línea: " << line << endl;
        }
    }
    file.close();
    return records;
}

void readFile(string filename)
{
    ExtendibleHashing hashTable;

    cout << "------------------------------------------\n";
    vector<Record> records = leerCSV("datos.csv");
    bool passed = true;
    for (auto &record : records)
    {
        hashTable.insert(record);

        Record r = hashTable.find(record.codigo);
        r.showData();

        if (!(r.codigo == record.codigo))
        {
            passed = false;
            cout << "Error en el record con codigo: " << record.codigo << "\n";
            cout << "Se esperaba: \n";
            record.showData();
            cout << "\nSe obtuvo: \n";
            r.showData();
            cout << "\n";
            break;
        }
    }
    if (passed)
        cout << "Todos los records fueron leidos correctamente\n";
    
    hashTable.displayBuckets();
    hashTable.display_index_dat();
    hashTable.saveDatafile();
    hashTable.printDatafile();

}
int main()
{
    readFile("datafile.dat");
    // Agruegue mas casos de prueba.
    return 0;
}
