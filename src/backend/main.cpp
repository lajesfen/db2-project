#include "include/crow.h"
#include "formats/AVL.cpp"

int main() {
    crow::SimpleApp app;
    AVLFile<int> file("avl_data.dat");

    CROW_ROUTE(app, "/avl/<int>")([&file](int pos) {
        auto res = file.getRecord(pos).toString();

        return res;
    });

    app.port(18080).multithreaded().run();
}