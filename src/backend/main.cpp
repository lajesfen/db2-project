#include "include/crow.h"
#include "include/crow/middlewares/cors.h"
#include "formats/AVL.cpp"
#include "include/json.hpp"
#include <vector>

int main() {
    crow::App<crow::CORSHandler> app;

    CROW_ROUTE(app, "/query")([] {
        json data;
        SocialRecord social = {1, "Presidencia de la", "Presidencia", "www.gob.pe/presidencia", "www.facebook.com/PresidenciaPeru", "www.twitter.com/presidenciaperu", "www.instagram.com/presidenciaperu", "www.linkedin.com/company/74524807/admin/", "www.youtube.com/presidenciaperu", "www.tiktok.com/@presidenciaperu"};
        data.push_back(social.toJSON());

        social = {2, "Presidencia de", "Presidencia 2", "www.gob.pe/presidencia", "www.facebook.com/PresidenciaPeru", "www.twitter.com/presidenciaperu", "www.instagram.com/presidenciaperu", "www.linkedin.com/company/74524807/admin/", "www.youtube.com/presidenciaperu", "www.tiktok.com/@presidenciaperu"};
        data.push_back(social.toJSON());

        return crow::response(data.dump());
    });

    app.port(18080).multithreaded().run();
}