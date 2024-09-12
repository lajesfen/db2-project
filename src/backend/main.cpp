#include "include/crow.h"
#include "formats/AVL.cpp"

int main() {
    crow::SimpleApp app;
    AVLFile<int> fileAVL("avl_data.dat");

    // GET
    // - by Key (Find)
    // - in Order
    // SET (ADD)
    // MODIFY
    // - by Key
    // DELETE
    // - by Key

    // GET Record by Primary Key (Find)
    CROW_ROUTE(app, "/avl/get/<int>")([&fileAVL](int pos) {
        auto res = fileAVL.find(pos).toString();

        return res;
    });

    // GET All Records
    CROW_ROUTE(app, "/avl/getAll")([&fileAVL] {
//        auto res = fileAVL.getAll().toString();

        return 0;
    });

    app.port(18080).multithreaded().run();
}