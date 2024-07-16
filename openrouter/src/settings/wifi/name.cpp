#include "wifi.hpp"
#include <colors.h>
#include "../../types.hpp"
#include <json.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <syslog.h>
#include "../../auth/auth.hpp"

#define WIFI_NAME_REQUIRED_STRING R"({"required":"[token,name]"})"

namespace settings {
    void get_wifi_name(const httplib::Request &request, httplib::Response &response) {
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
            std::string wifi_name;
            std::string wifisettingsbuffer;
            while (std::getline(wifisettingsfile, wifisettingsbuffer)) {
                if (wifisettingsbuffer.find("ssid=") == 0) {
                    wifi_name = wifisettingsbuffer;
                    wifi_name.erase(0, 5);
                    break;
                }
            }
            wifisettingsfile.close();
            std::stringstream responsedata;
            responsedata << R"({"name":")" << wifi_name << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
            return;
        }
        response.set_content(R"({"access":"reject"})", JSON_TYPE);
    }

    void set_wifi_name(const httplib::Request &request, httplib::Response &response) {
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

        if (json_body["name"] == nullptr) {
            response.set_content(WIFI_NAME_REQUIRED_STRING, JSON_TYPE);
            return;
        }

        if (authenticate(json_body["token"])) {
            std::ifstream wifisettingsfile(WIFI_SETTINGS_PATH);
            std::string old_wifi_name;
            std::string wifisettingsbuffer;
            while (std::getline(wifisettingsfile, wifisettingsbuffer)) {
                if (wifisettingsbuffer.find("ssid=") == 0) {
                    old_wifi_name = wifisettingsbuffer;
                    old_wifi_name.erase(0, 5);
                    break;
                }
            }
            wifisettingsfile.close();
            std::string new_wifi_name = json_body["name"];
            std::stringstream systemrequest;
            systemrequest << "sed -i \"/ssid=" << old_wifi_name << "/s//ssid=" << new_wifi_name << "/\" /etc/hostapd/hostapd.conf";
            std::cout << systemrequest.str() << std::endl;
            system(systemrequest.str().c_str());
            system("rc restart hostapd");
            std::string syslogstring = "WIFI name changed to " + new_wifi_name;
            syslog(LOG_INFO, "%s", syslogstring.c_str());

            std::ifstream changedwifisettingsfile(WIFI_SETTINGS_PATH);
            std::string wifi_name;
            std::string changedwifisettingsbuffer;
            while (std::getline(changedwifisettingsfile, changedwifisettingsbuffer)) {
                if (changedwifisettingsbuffer.find("ssid=") == 0) {
                    wifi_name = changedwifisettingsbuffer;
                    wifi_name.erase(0, 5);
                    break;
                }
            }
            changedwifisettingsfile.close();
            std::stringstream responsedata;
            responsedata << R"({"name":")" << wifi_name << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
            return;
        }
        response.set_content(R"({"access":"reject"})", JSON_TYPE);
    }
}