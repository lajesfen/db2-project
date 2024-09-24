#include <vector>
#include "../data/HospitalRecord.h"
#include "../data/SocialRecord.h"

template <typename RecordType>
std::vector<RecordType> readFromCSV() {

};

template <>
inline std::vector<HospitalRecord> readFromCSV() {
    std::vector<HospitalRecord> res;

    // ToDo: Readline de cada argumento (Revisar los que se omiten) y agrega records a vector.

    return res;
}

template <>
inline std::vector<SocialRecord> readFromCSV() {
    std::vector<SocialRecord> res;

    // ToDo: Readline de cada argumento (Revisar los que se omiten) y agrega records a vector.

    return res;
}