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
#include "../../auth/auth.hpp"

#define WIFI_PASSWORD_REQUIRED_STRING R"({"required":"[token,password,newpassword]"})"

namespace settings {
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

        if (authenticate(json_body["token"])) {
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
            return;
        }
        response.set_content(R"({"access":"reject"})", JSON_TYPE);
    }

    void set_wifi_password(const httplib::Request &request, httplib::Response &response) {
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

        if (json_body["password"] == nullptr) {
            response.set_content(WIFI_PASSWORD_REQUIRED_STRING, JSON_TYPE);
            return;
        }

        if (json_body["newpassword"] == nullptr) {
            response.set_content(WIFI_PASSWORD_REQUIRED_STRING, JSON_TYPE);
            return;
        }

        if (authenticate(json_body["token"])) {
            std::ifstream wifisettingsfile(WIFI_SETTINGS_PATH);
            std::string old_wifi_password;
            std::string wifisettingsbuffer;
            while (std::getline(wifisettingsfile, wifisettingsbuffer)) {
                if (wifisettingsbuffer.find("wpa_passphrase=") == 0) {
                    old_wifi_password = wifisettingsbuffer;
                    old_wifi_password.erase(0, 15);
                    break;
                }
            }
            wifisettingsfile.close();
            std::string req_old_password = json_body["password"];
            if (old_wifi_password != req_old_password) {
                response.set_content(R"({"status":"reject"})", JSON_TYPE);
                return;
            }
            std::string new_wifi_password = json_body["newpassword"];
            std::stringstream systemrequest;
            systemrequest << "sed -i \"/wpa_passphrase=" << old_wifi_password << "/s//wpa_passphrase=" << new_wifi_password << "/\" /etc/hostapd/hostapd.conf";
            system(systemrequest.str().c_str());
            system("rc restart hostapd");
            std::string syslogstring = "WIFI password changed";
            syslog(LOG_INFO, syslogstring.c_str());
            std::stringstream responsedata;
            responsedata << R"({"success":"ok"})";
            response.set_content(responsedata.str(), JSON_TYPE);
            return;
        }
        response.set_content(R"({"access":"reject"})", JSON_TYPE);
    }
}
