#include "include/crow.h"
#include "include/crow/middlewares/cors.h"
#include "formats/AVL.cpp"

int main() {
    crow::App<crow::CORSHandler> app;

    // GET
    // - by Key (Find)
    // - in Order
    // SET (ADD)
    // MODIFY
    // - by Key
    // DELETE
    // - by Key

    // GET Record by Primary Key (Find)
//    CROW_ROUTE(app, "/avl/get/<int>")([&fileAVL](int pos) {
//        auto res = fileAVL.find(pos).toString();
//
//        return res;
//    });

    // GET All Records
//    CROW_ROUTE(app, "/avl/getAll")([&fileAVL] {
//
//        return 0;
//    });

    CROW_ROUTE(app, "/test/social")([] {
        SocialRecord social = {1, "Presidencia de la República del Perú", "Presidencia", "www.gob.pe/presidencia", "www.facebook.com/PresidenciaPeru", "www.twitter.com/presidenciaperu", "www.instagram.com/presidenciaperu", "www.linkedin.com/company/74524807/admin/", "www.youtube.com/presidenciaperu", "www.tiktok.com/@presidenciaperu"};

        return social.toJSON().dump();
    });

    app.port(18080).multithreaded().run();
}