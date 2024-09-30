#include "include/crow.h"
#include "include/crow/middlewares/cors.h"
#include "parser/parser.cpp"
#include <vector>

int main() {
    crow::App<crow::CORSHandler> app;
    SQLParser parser;

    CROW_ROUTE(app, "/query").methods(crow::HTTPMethod::POST)([&parser](const crow::request &req) {
        auto res = parser.parse(req.body);

        return crow::response(res.dump(-1, ' ', false, json::error_handler_t::ignore));
    });

    CROW_ROUTE(app, "/ping").methods(crow::HTTPMethod::GET)([]() {
        return crow::response("Pong");
    });

    app.port(18080).multithreaded().run();
}