#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>

template <typename TK, typename RecordType>
class AVLFile
{
private:
    std::string filename;
    long pos_root;
    const long offset = sizeof(pos_root);

public:
    AVLFile(std::string filename)
    {
        this->filename = filename;
        this->pos_root = -1;

        std::fstream file(filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open())
        {
            file.open(filename, std::ios::out | std::ios::binary);
            file.write((char *)&pos_root, offset);
            file.close();
        }
        else
        {
            loadRootPos();
        }
        file.close();
    }

    ~AVLFile()
    {
        saveRootPos();
    }

    RecordType find(TK key)
    {
        return find(pos_root, key);
    }

    RecordType find(long pos, TK key)
    {
        if (pos == -1)
        {
            RecordType record;
            record.id = -1;
            return record;
        }

        auto temp = getRecord(pos);
        if (key < temp.id)
        {
            return find(temp.left, key);
        }
        else if (key > temp.id)
        {
            return find(temp.right, key);
        }
        else
        {
            return temp;
        }
    }

    RecordType getRecord(long pos)
    {
        std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
        RecordType record;

        file.seekg(offset + pos * sizeof(RecordType), std::ios::beg);
        file.read((char *)&record, sizeof(RecordType));
        file.close();
        return record;
    }

    void setRecord(long pos, RecordType record)
    {
        std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);

        file.seekp(offset + pos * sizeof(RecordType), std::ios::beg);
        file.write((char *)&record, sizeof(RecordType));
        file.close();
    }

    void insert(RecordType record)
    {
        insert(pos_root, record);
    }

    void insert(long &pos, RecordType record)
    {
        if (pos == -1)
        {
            pos = getSize();
            setRecord(pos, record);
        }
        else
        {
            auto temp = getRecord(pos);

            if (record.id < temp.id)
            {
                if (temp.left == -1)
                {
                    temp.left = getSize();
                    setRecord(temp.left, record);
                }
                else
                {
                    insert(temp.left, record);
                }
            }
            else if (record.id > temp.id)
            {
                if (temp.right == -1)
                {
                    temp.right = getSize();
                    setRecord(temp.right, record);
                }
                else
                {
                    insert(temp.right, record);
                }
            }

            updateHeight(temp);
            balance(pos);
            setRecord(pos, temp);
        }
    }

    std::vector<RecordType> searchByRange(TK lower, TK upper)
    {
        std::vector<RecordType> results;
        searchByRange(pos_root, lower, upper, results);
        return results;
    }

    void searchByRange(long pos, TK lower, TK upper, std::vector<RecordType> &results)
    {
        if (pos == -1)
            return;

        RecordType current = getRecord(pos);
        if (current.id >= lower)
            searchByRange(current.left, lower, upper, results);

        if (current.id >= lower && current.id <= upper)
            results.push_back(current);

        if (current.id <= upper)
            searchByRange(current.right, lower, upper, results);
    }

    auto loadRootPos()
    {
        std::ifstream file(filename, std::ios::binary);
        file.seekg(0, std::ios::beg);
        file.read((char *)&pos_root, offset);
    }

    auto saveRootPos()
    {
        std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
        file.seekp(0, std::ios::beg);
        file.write((char *)&pos_root, offset);
    }

    void updateHeight(RecordType &record)
    {
        int leftHeight = getHeight(record.left);
        int rightHeight = getHeight(record.right);
        record.height = std::max(leftHeight, rightHeight);
    }

    int getHeight(long pos)
    {
        if (pos == -1)
            return -1;
        RecordType record = getRecord(pos);
        return record.height;
    }

    int getBalanceFactor(long pos)
    {
        if (pos == -1)
            return 0;
        RecordType record = getRecord(pos);
        return getHeight(record.left) - getHeight(record.right);
    }

    void balance(long &pos)
    {
        if (pos == -1)
            return;

        RecordType record = getRecord(pos);
        int hb = getBalanceFactor(pos);
        if (hb > 1)
        {
            if (getBalanceFactor(record.left) < 0)
                rotateLeft(record.left);
            rotateRight(pos);
        }
        else if (hb < -1)
        {
            if (getBalanceFactor(record.right) > 0)
                rotateRight(record.right);
            rotateLeft(pos);
        }
        updateHeight(record);
        setRecord(pos, record);
    }

    void rotateRight(long &pos)
    {
        RecordType record = getRecord(pos);
        RecordType leftChild = getRecord(record.left);

        record.left = leftChild.right;
        leftChild.right = pos;
        updateHeight(record);
        updateHeight(leftChild);

        setRecord(pos, record);
        setRecord(record.left, leftChild);
    }

    void rotateLeft(long &pos)
    {
        RecordType record = getRecord(pos);
        RecordType rightChild = getRecord(record.right);

        record.right = rightChild.left;
        rightChild.left = pos;
        updateHeight(record);
        updateHeight(rightChild);

        setRecord(pos, record);
        setRecord(record.right, rightChild);
    }

    auto getSize()
    {
        std::ifstream file(filename, std::ios::binary);
        file.seekg(0, std::ios::end);

        return ((int)file.tellg() - offset) / sizeof(RecordType);
    }

    std::vector<RecordType> inorder()
    {
        std::vector<RecordType> res;
        inorder(pos_root, res);

        return res;
    }

    void inorder(long pos, std::vector<RecordType> &res)
    {
        if (pos == -1)
            return;

        RecordType temp = getRecord(pos);
        if (temp.left != -1)
        {
            inorder(temp.left, res);
        }
        res.push_back(temp);
        if (temp.right != -1)
        {
            inorder(temp.right, res);
        }
    }

    bool remove(TK key)
    {
        return remove(pos_root, key);
    }

    bool remove(long &pos, TK key)
    {
        if (pos == -1)
            return false;

        RecordType record = getRecord(pos);

        if (key < record.id)
        {
            if (!remove(record.left, key))
                return false;
        }
        else if (key > record.id)
        {
            if (!remove(record.right, key))
                return false;
        }
        else
        {
            if (record.left == -1 || record.right == -1)
            {
                long tempPos = (record.left != -1) ? record.left : record.right;

                if (tempPos == -1)
                {
                    pos = -1;
                }
                else
                {
                    pos = tempPos;
                }
            }
            else
            {
                long nextPos = getMinValueRecord(record.right);
                RecordType nextNode = getRecord(nextPos);

                record.id = nextNode.id;

                if (!remove(record.right, nextNode.id))
                    return false;
            }
        }

        if (pos != -1)
        {
            updateHeight(record);
            balance(pos);
            setRecord(pos, record);
        }

        return true;
    }

    auto getMinValueRecord(long pos)
    {
        RecordType record = getRecord(pos);
        long temp = pos;

        while (record.left != -1)
        {
            temp = record.left;
            record = getRecord(record.left);
        }
        return temp;
    }

    // para ubicar los avls
    friend int buscarAVLPorNombre(const std::vector<AVLFile> &avlVector, const std::string &nombreBuscado)
    {
        int i;
        for (i = 0; i < avlVector.size(); ++i)
        {
            if (avlVector[i].filename == nombreBuscado)
            {
                return i;
            }
        }
        return -1;
    }
};