#include "auth.hpp"
#include <sstream>
#include <httplib.h>
#include "../routes.hpp"
#include <json.hpp>
#include "../types.hpp"

bool authenticate(std::string token) {
    httplib::Client cli(AUTH_SERVICE);

    std::stringstream body;
    body <<  R"({"token":")" << token << R"("})";
    httplib::Result res = cli.Post("/api/access", body.str().c_str(), JSON_TYPE);
    if (res->status == httplib::OK_200) {
        nlohmann::json resjson = nlohmann::json::parse(res->body);
        if (resjson["access"] == "reject") {
            return false;
        }
        return true;
    }
    return false;
}
