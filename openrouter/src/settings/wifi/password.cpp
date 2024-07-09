#include "wifi.hpp"
#include <colors.h>
#include "../../types.hpp"
#include "../../routes.hpp"
#include <json.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <syslog.h>

#define WIFI_SETTINGS_PATH "/etc/hostapd/hostapd.conf"

void get_wifi_password(const httplib::Request &request, httplib::Response &response) {
    std::cout << GREEN << request.path << RESET << "  " << request.method << std::endl;
    response.set_header("Access-Control-Allow-Origin", "*");
    if (request.body.empty()) {
        response.set_content(ACCESS_REQUIRED_STRING, JSON_TYPE);
        return;
    }

    nlohmann::json json_body = nlohmann::json::parse(request.body);
    if (json_body["token"] == nullptr) {
        response.set_content(ACCESS_REQUIRED_STRING, JSON_TYPE);
        return;
    }

    httplib::Client cli(AUTH_SERVICE);

    std::stringstream body;
    body <<  R"({"token":)" << json_body["token"] << R"(})";

    httplib::Result res = cli.Post("/api/access", body.str().c_str(), JSON_TYPE);
    if (res->status == httplib::OK_200) {
        nlohmann::json resjson = nlohmann::json::parse(res->body);
        if (resjson["access"] == "reject") {
            response.set_content(resjson.dump().c_str(), JSON_TYPE);
            return;
        }
        std::ifstream wifisettingsfile(WIFI_SETTINGS_PATH);
        std::string wifi_password;
        std::string wifisettingsbuffer;
        while (std::getline(wifisettingsfile, wifisettingsbuffer)) {
            if (wifisettingsbuffer.find("wpa_passphrase=") == 0) {
                wifi_password = wifisettingsbuffer;
                wifi_password.erase(0, 15);
                break;
            }
        }
        wifisettingsfile.close();
        std::stringstream responsedata;
        responsedata << R"({"password":")" << wifi_password << R"("})";
        response.set_content(responsedata.str(), JSON_TYPE);
    }
}
