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

template <typename RecordType>
class Bucket
{
public:
    vector<RecordType> records;
    int depth;

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

template <typename RecordType>
class ExtendibleHashing
{
private:
    vector<Bucket<RecordType> *> buckets;

public:
    ExtendibleHashing()
    {
        buckets.push_back(new Bucket<RecordType>(1));
        buckets.push_back(new Bucket<RecordType>(1));

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
        for (Bucket<RecordType> *bucket : buckets)
        {
            delete bucket;
        }
    }

    int hashFunction(int key) const
    {
        return key % (1 << MAX_DEPTH);
    }

    RecordType find(int codi)
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

        return RecordType{};
    }

    void insert(const RecordType &record)
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

        auto it = remove_if(records.begin(), records.end(), [&](const RecordType &rec)
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
        Bucket<RecordType> *oldBucket = buckets[bucketIndex];
        int oldDepth = oldBucket->depth;

        if (oldDepth >= MAX_DEPTH)
        {
            cout << "Cannot split, maximum depth reached!" << endl;
            return;
        }

        Bucket<RecordType> *newBucket = new Bucket<RecordType>(oldDepth + 1);
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

        vector<RecordType> recordsToRedistribute = oldBucket->records;
        oldBucket->clear();
        for (const RecordType &record : recordsToRedistribute)
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
            for (const RecordType &rec : buckets[i]->records)
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
            for (const Bucket<RecordType> *bucket : buckets)
            {
                for (const RecordType &record : bucket->records)
                {
                    outFile.write(reinterpret_cast<const char *>(&record), sizeof(RecordType));
                }
                RecordType emptyRecord = {0, "", "", 0};
                for (size_t i = bucket->records.size(); i < MAX_FILL; ++i)
                {
                    outFile.write(reinterpret_cast<const char *>(&emptyRecord), sizeof(RecordType));
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
                Bucket<RecordType> *bucket = new Bucket<RecordType>(0);
                for (size_t i = 0; i < MAX_FILL; ++i)
                {
                    RecordType record;
                    inFile.read(reinterpret_cast<char *>(&record), sizeof(RecordType));
                    if (inFile.gcount() == sizeof(RecordType))
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
                Bucket<RecordType> bucket(0);
                for (size_t i = 0; i < MAX_FILL; ++i)
                {
                    RecordType record;
                    inFile.read(reinterpret_cast<char *>(&record), sizeof(RecordType));
                    if (inFile.gcount() == sizeof(RecordType))
                    {
                        bucket.insert(record);
                    }
                }

                int depth;
                inFile.read(reinterpret_cast<char *>(&depth), sizeof(depth));
                if (inFile.gcount() == sizeof(depth))
                {
                    cout << bucketIndex << "      ";
                    for (const RecordType &rec : bucket.records)
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