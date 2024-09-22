#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <cmath>
#include <fstream>
#include <array>
#include <sstream>
#include <cstring>
#include <algorithm>

#define MAX_DEPTH 3 // Profundidad máxima
#define MAX_FILL 4 // Factor de llenado
using namespace std;
struct Record
{
    int codigo;
    char nombre[12];
    char apellido[12];
    int ciclo;

    void showData() const
    {
        cout << "\nCodigo: " << codigo;
    }
};

class Bucket
{
public:
    vector<Record> records;
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
    vector<Bucket *> buckets;

public:
    ExtendibleHashing()
    {
        buckets.push_back(new Bucket(1));
        buckets.push_back(new Bucket(1));

        ofstream outFile("index.dat", ios::binary | ios::trunc);
        if (outFile)
        {
            for (int i = 0; i < (1 << MAX_DEPTH); ++i)
            {
                outFile.write(reinterpret_cast<const char *>(&i), sizeof(i));
                int a = i % 2;
                outFile.write(reinterpret_cast<const char *>(&a), sizeof(a));
            }
            cout << "Index saved to index.dat." << endl;
        }
        else
        {
            cout << "Failed to open index.dat for writing." << endl;
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
                cout << "Alredy exists " << endl;
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

    void remove(int codigo)
    {
        int hashValue = hashFunction(codigo);
        int bucketIndex = get_bucket_index(hashValue);

        auto &records = buckets[bucketIndex]->records;

        auto it = remove_if(records.begin(), records.end(), [&](const Record &rec)
                                 { return rec.codigo == codigo; });

        if (it != records.end())
        {
            records.erase(it, records.end());
            cout << "Record with codigo " << codigo << " has been deleted." << endl;
        }
        else
        {
            cout << "Record with codigo " << codigo << " not found." << endl;
        }
    }

    void split(int bucketIndex)
    {
        Bucket *oldBucket = buckets[bucketIndex];
        int oldDepth = oldBucket->depth;

        if (oldDepth >= MAX_DEPTH)
        {
            cout << "Cannot split, maximum depth reached!" << endl;
            return;
        }

        Bucket *newBucket = new Bucket(oldDepth + 1);
        buckets.push_back(newBucket);
        int newIndex = buckets.size() - 1;

        fstream indexFile("index.dat", ios::in | ios::out | ios::binary);
        if (!indexFile)
        {
            cerr << "Failed to open index.dat" << endl;
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

        cout << " ======================================= " << endl;

        indexFile.close();
        fstream xddd("index.dat", ios::in | ios::out | ios::binary);

        for (size_t i = 0; i < (1 << MAX_DEPTH); ++i)
        {
            int xdd, currentBucketIndex;
            xddd.read(reinterpret_cast<char *>(&xdd), sizeof(xdd));
            xddd.read(reinterpret_cast<char *>(&currentBucketIndex), sizeof(currentBucketIndex));
            cout << xdd << " : " << currentBucketIndex << endl;
            xddd.seekg(i * (sizeof(int) + sizeof(int)));
            xddd.read(reinterpret_cast<char *>(&xdd), sizeof(xdd));
            xddd.read(reinterpret_cast<char *>(&currentBucketIndex), sizeof(currentBucketIndex));
            cout << xdd << " -* " << currentBucketIndex << endl;
        }

        xddd.close();

        vector<Record> recordsToRedistribute = oldBucket->records;
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
        ifstream inFile("index.dat", ios::binary);
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
        ifstream inFile("index.dat", ios::binary);
        if (inFile)
        {
            cout << "\nContents of index.dat:\n";
            cout << "binary  #bucket\n";
            for (size_t i = 0; i < (1 << MAX_DEPTH); ++i)
            {
                int indexValue, bucketIndex;

                if (!inFile.read(reinterpret_cast<char *>(&indexValue), sizeof(indexValue)))
                {
                    cout << "Error reading index.dat." << endl;
                    break;
                }

                if (!inFile.read(reinterpret_cast<char *>(&bucketIndex), sizeof(bucketIndex)))
                {
                    cout << "Error reading index.dat." << endl;
                    break;
                }

                cout << bitset<MAX_DEPTH>(indexValue) << "    " << bucketIndex << "\n";
            }
            inFile.close();
        }
        else
        {
            cout << "Failed to open index.dat." << endl;
        }
    }

    void displayBuckets() const
    {
        cout << "\ndatafile.dat:\n";
        cout << "bucket  depth  records\n";
        for (size_t i = 0; i < buckets.size(); ++i)
        {
            cout << " " << i << "      " << buckets[i]->depth << "    ";
            for (const Record &rec : buckets[i]->records)
            {
                cout << rec.codigo << " ";
            }
            cout << "\n";
        }
    }

    void saveDatafile() const
    {
        ofstream outFile("datafile.dat", ios::binary | ios::trunc);
        if (outFile)
        {
            for (const Bucket *bucket : buckets)
            {
                for (const Record &record : bucket->records)
                {
                    outFile.write(reinterpret_cast<const char *>(&record), sizeof(Record));
                }
                Record emptyRecord = {0, "", "", 0};  
                for (size_t i = bucket->records.size(); i < MAX_FILL; ++i)
                {
                    outFile.write(reinterpret_cast<const char *>(&emptyRecord), sizeof(Record));
                }
                outFile.write(reinterpret_cast<const char *>(&bucket->depth), sizeof(bucket->depth));
            }
            cout << "Records saved to datafile.dat." << endl;
        }
        else
        {
            cout << "Failed to open datafile.dat for writing." << endl;
        }
    }

    void loadDatafile()
    {
        ifstream inFile("datafile.dat", ios::binary);
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
            cout << "Records loaded from datafile.dat." << endl;
        }
        else
        {
            cout << "Failed to open datafile.dat for reading." << endl;
        }
    }

    void printDatafile() const
    {
        ifstream inFile("datafile.dat", ios::binary);
        if (inFile)
        {
            cout << "Contents of datafile.dat:\n";
            cout << "bucket  records           depth\n";

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
                    cout << bucketIndex << "      ";
                    for (const Record &rec : bucket.records)
                    {
                        cout << rec.codigo << " ";
                    }
                    cout << "         " << depth << "\n";
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
            cout << "Failed to open datafile.dat for reading." << endl;
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
    
    hashTable.remove(64364845);
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
