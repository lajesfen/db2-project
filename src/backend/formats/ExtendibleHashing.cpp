#ifndef EXTENDIBLE_HASH_H
#define EXTENDIBLE_HASH_H

#include <iostream>
#include <vector>
#include <fstream>
#include <bitset>
#include <cmath>
#include <algorithm>

<<<<<<< HEAD
#define MAX_DEPTH 3 // Profundidad máxima del directorio
#define MAX_FILL 4  // Capacidad máxima de cada bucket

// Clase Bucket
template <typename TK, typename RecordType>
class Bucket
{
public:
    std::vector<RecordType> records; // Almacenamiento de registros
    int depth;                       // Profundidad local del bucket

    Bucket(int d) : depth(d) {}

    bool isFull() const
    {
        return records.size() >= MAX_FILL;
    }

    void insert(const RecordType &record)
    {
        records.push_back(record);
    }

    void clear()
    {
        records.clear();
    }
};

// Clase de Hash Extensible
template <typename TK, typename RecordType>
class ExtendibleHashing
{
private:
    std::vector<Bucket<TK, RecordType> *> buckets;

public:
    ExtendibleHashing()
    {
        buckets.push_back(new Bucket<TK, RecordType>(1));
        buckets.push_back(new Bucket<TK, RecordType>(1));

        // Inicializar el archivo index.dat
        std::ofstream outFile("index.dat", std::ios::binary | std::ios::trunc);
        if (outFile)
        {
            for (int i = 0; i < (1 << MAX_DEPTH); ++i)
            {
                outFile.write(reinterpret_cast<const char *>(&i), sizeof(i));
                int a = i % 2;
                outFile.write(reinterpret_cast<const char *>(&a), sizeof(a));
            }
            outFile.close();
            std::cout << "Index guardado en index.dat." << std::endl;
        }
        else
        {
            std::cout << "Error al abrir index.dat." << std::endl;
        }
    }

    ~ExtendibleHashing()
    {
        for (auto bucket : buckets)
        {
            delete bucket;
        }
    }

    int hashFunction(TK key) const
    {
        return static_cast<int>(key) % (1 << MAX_DEPTH);
    }

    RecordType find(TK key)
    {
        int hashValue = hashFunction(key);
        int bucketIndex = get_bucket_index(hashValue);

        for (const auto &rec : buckets[bucketIndex]->records)
        {
            if (rec.key == key)
            {
                return rec;
            }
        }
        throw std::runtime_error("Registro no encontrado.");
    }

    void insert(const RecordType &record)
    {
        int hashValue = hashFunction(record.key);
        int bucketIndex = get_bucket_index(hashValue);

        for (const auto &rec : buckets[bucketIndex]->records)
        {
            if (rec.key == record.key)
            {
                std::cout << "El registro con clave " << record.key << " ya existe." << std::endl;
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

    void remove(TK key)
    {
        int hashValue = hashFunction(key);
        int bucketIndex = get_bucket_index(hashValue);

        auto &records = buckets[bucketIndex]->records;
        auto it = std::remove_if(records.begin(), records.end(), [&](const RecordType &rec)
                                 { return rec.key == key; });

        if (it != records.end())
        {
            records.erase(it, records.end());
            std::cout << "Registro con clave " << key << " eliminado." << std::endl;
        }
        else
        {
            std::cout << "Registro con clave " << key << " no encontrado." << std::endl;
        }
    }

    void split(int bucketIndex)
    {
        auto oldBucket = buckets[bucketIndex];
        int oldDepth = oldBucket->depth;

        if (oldDepth >= MAX_DEPTH)
        {
=======
#define MAX_DEPTH 3   // Profundidad máxima del directorio
#define MAX_FILL 4    // Capacidad máxima de cada bucket

// Clase Bucket
template <typename TK, typename RecordType>
class Bucket {
public:
    std::vector<RecordType> records;  // Almacenamiento de registros
    int depth;                   // Profundidad local del bucket

    Bucket(int d) : depth(d) {}

    bool isFull() const {
        return records.size() >= MAX_FILL;
    }

    void insert(const RecordType &record) {
        records.push_back(record);
    }

    void clear() {
        records.clear();
    }
};

// Clase de Hash Extensible
template <typename TK, typename RecordType>
class ExtendibleHashing {
private:
    std::vector<Bucket<TK, RecordType> *> buckets;

public:
    ExtendibleHashing() {
        buckets.push_back(new Bucket<TK, RecordType>(1));
        buckets.push_back(new Bucket<TK, RecordType>(1));

        // Inicializar el archivo index.dat
        std::ofstream outFile("index.dat", std::ios::binary | std::ios::trunc);
        if (outFile) {
            for (int i = 0; i < (1 << MAX_DEPTH); ++i) {
                outFile.write(reinterpret_cast<const char *>(&i), sizeof(i));
                int a = i % 2;
                outFile.write(reinterpret_cast<const char *>(&a), sizeof(a));
            }
            outFile.close();
            std::cout << "Index guardado en index.dat." << std::endl;
        } else {
            std::cout << "Error al abrir index.dat." << std::endl;
        }
    }

    ~ExtendibleHashing() {
        for (auto bucket : buckets) {
            delete bucket;
        }
    }

    int hashFunction(TK key) const {
        return static_cast<int>(key) % (1 << MAX_DEPTH);
    }

    RecordType find(TK key) {
        int hashValue = hashFunction(key);
        int bucketIndex = get_bucket_index(hashValue);

        for (const auto &rec : buckets[bucketIndex]->records) {
            if (rec.key == key) {
                return rec;
            }
        }
        throw std::runtime_error("Registro no encontrado.");
    }

    void insert(const RecordType &record) {
        int hashValue = hashFunction(record.key);
        int bucketIndex = get_bucket_index(hashValue);

        for (const auto &rec : buckets[bucketIndex]->records) {
            if (rec.key == record.key) {
                std::cout << "El registro con clave " << record.key << " ya existe." << std::endl;
                return;
            }
        }

        if (buckets[bucketIndex]->isFull()) {
            split(bucketIndex);
            bucketIndex = get_bucket_index(hashValue);
        }

        buckets[bucketIndex]->insert(record);
    }

    void remove(TK key) {
        int hashValue = hashFunction(key);
        int bucketIndex = get_bucket_index(hashValue);

        auto &records = buckets[bucketIndex]->records;
        auto it = std::remove_if(records.begin(), records.end(), [&](const RecordType &rec) {
            return rec.key == key;
        });

        if (it != records.end()) {
            records.erase(it, records.end());
            std::cout << "Registro con clave " << key << " eliminado." << std::endl;
        } else {
            std::cout << "Registro con clave " << key << " no encontrado." << std::endl;
        }
    }

    void split(int bucketIndex) {
        auto oldBucket = buckets[bucketIndex];
        int oldDepth = oldBucket->depth;

        if (oldDepth >= MAX_DEPTH) {
>>>>>>> c3653f536341bd6d88981d8cc24ea2c6d2c45708
            std::cout << "No se puede dividir más, profundidad máxima alcanzada." << std::endl;
            return;
        }

        auto newBucket = new Bucket<TK, RecordType>(oldDepth + 1);
        buckets.push_back(newBucket);
        int newIndex = buckets.size() - 1;

        std::fstream indexFile("index.dat", std::ios::in | std::ios::out | std::ios::binary);
<<<<<<< HEAD
        if (!indexFile)
        {
=======
        if (!indexFile) {
>>>>>>> c3653f536341bd6d88981d8cc24ea2c6d2c45708
            std::cerr << "Error al abrir index.dat" << std::endl;
            return;
        }

<<<<<<< HEAD
        for (size_t i = 0; i < (1 << MAX_DEPTH); ++i)
        {
=======
        for (size_t i = 0; i < (1 << MAX_DEPTH); ++i) {
>>>>>>> c3653f536341bd6d88981d8cc24ea2c6d2c45708
            int xdd, currentBucketIndex;
            indexFile.read(reinterpret_cast<char *>(&xdd), sizeof(xdd));
            indexFile.read(reinterpret_cast<char *>(&currentBucketIndex), sizeof(currentBucketIndex));

<<<<<<< HEAD
            if (currentBucketIndex == bucketIndex)
            {
                if ((i >> oldDepth) & 1)
                {
=======
            if (currentBucketIndex == bucketIndex) {
                if ((i >> oldDepth) & 1) {
>>>>>>> c3653f536341bd6d88981d8cc24ea2c6d2c45708
                    indexFile.seekg(i * (sizeof(int) + sizeof(int)));
                    indexFile.write(reinterpret_cast<const char *>(&xdd), sizeof(xdd));
                    indexFile.write(reinterpret_cast<const char *>(&newIndex), sizeof(newIndex));
                }
            }
        }
        indexFile.close();

        // Redistribuir registros
        std::vector<RecordType> recordsToRedistribute = oldBucket->records;
        oldBucket->clear();
<<<<<<< HEAD
        for (const RecordType &record : recordsToRedistribute)
        {
=======
        for (const RecordType &record : recordsToRedistribute) {
>>>>>>> c3653f536341bd6d88981d8cc24ea2c6d2c45708
            insert(record);
        }

        oldBucket->depth++;
        newBucket->depth = oldBucket->depth;
    }

<<<<<<< HEAD
    int get_bucket_index(int i)
    {
        std::ifstream inFile("index.dat", std::ios::binary);
        if (inFile)
        {
=======
    int get_bucket_index(int i) {
        std::ifstream inFile("index.dat", std::ios::binary);
        if (inFile) {
>>>>>>> c3653f536341bd6d88981d8cc24ea2c6d2c45708
            int bucketIndex;
            inFile.seekg(i * (sizeof(int) + sizeof(int)) + sizeof(int));
            inFile.read(reinterpret_cast<char *>(&bucketIndex), sizeof(bucketIndex));
            inFile.close();
            return bucketIndex;
        }
        return -1;
    }

<<<<<<< HEAD
    void displayDatafile() const
    {
        std::cout << "\nContenido de los buckets:\n";
        for (size_t i = 0; i < buckets.size(); ++i)
        {
            std::cout << "Bucket " << i << " (Profundidad: " << buckets[i]->depth << "):\n";
            for (const auto &record : buckets[i]->records)
            {
=======
    void displayDatafile() const {
        std::cout << "\nContenido de los buckets:\n";
        for (size_t i = 0; i < buckets.size(); ++i) {
            std::cout << "Bucket " << i << " (Profundidad: " << buckets[i]->depth << "):\n";
            for (const auto &record : buckets[i]->records) {
>>>>>>> c3653f536341bd6d88981d8cc24ea2c6d2c45708
                std::cout << "  Clave: " << record.key << ", Nombre: " << record.name << ", Cantidad: " << record.quantity << "\n";
            }
        }
    }
};

<<<<<<< HEAD
#endif // EXTENDIBLE_HASH_H
=======
#endif  // EXTENDIBLE_HASH_H
>>>>>>> c3653f536341bd6d88981d8cc24ea2c6d2c45708
