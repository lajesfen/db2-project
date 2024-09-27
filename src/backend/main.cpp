#include "include/crow.h"
#include "include/crow/middlewares/cors.h"
#include "formats/AVL.cpp"
#include "loaders/DataLoader.cpp"
#include <vector>

int main() {
    crow::App<crow::CORSHandler> app;

    CROW_ROUTE(app, "/query").methods(crow::HTTPMethod::POST)([](const crow::request &req) {

        return crow::response("Pong");
    });

    app.port(18080).multithreaded().run();
}