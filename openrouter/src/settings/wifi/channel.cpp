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

#define WIFI_CHANNEL_REQUIRED_STRING R"({"required":"[token,channel]"})"

namespace settings {
    void get_wifi_channel(const httplib::Request &request, httplib::Response &response) {
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
            std::string wifi_channel;
            std::string wifisettingsbuffer;
            while (std::getline(wifisettingsfile, wifisettingsbuffer)) {
                if (wifisettingsbuffer.find("channel=") == 0) {
                    wifi_channel = wifisettingsbuffer;
                    wifi_channel.erase(0, 8);
                    break;
                }
            }
            wifisettingsfile.close();
            std::stringstream responsedata;
            responsedata << R"({"channel":")" << wifi_channel << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
            return;
        }
        response.set_content(R"({"access":"reject"})", JSON_TYPE);
    }

    void set_wifi_channel(const httplib::Request &request, httplib::Response &response) {
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

        if (json_body["channel"] == nullptr) {
            response.set_content(WIFI_CHANNEL_REQUIRED_STRING, JSON_TYPE);
            return;
        }

        if (authenticate(json_body["token"])) {
            std::ifstream wifisettingsfile(WIFI_SETTINGS_PATH);
            std::string old_channel;
            std::string wifisettingsbuffer;
            while (std::getline(wifisettingsfile, wifisettingsbuffer)) {
                if (wifisettingsbuffer.find("channel=") == 0) {
                    old_channel = wifisettingsbuffer;
                    old_channel.erase(0, 8);
                    break;
                }
            }
            wifisettingsfile.close();
            std::string new_channel = json_body["channel"];
            std::stringstream systemrequest;
            systemrequest << "sed -i \"/channel=" << old_channel << "/s//channel=" << new_channel << "/\" /etc/hostapd/hostapd.conf";
            std::cout << systemrequest.str() << std::endl;
            system(systemrequest.str().c_str());
            system("rc restart hostapd");
            std::string syslogstring = "WIFI channel changed to " + new_channel;
            syslog(LOG_INFO, syslogstring.c_str());

            std::ifstream changedwifisettingsfile(WIFI_SETTINGS_PATH);
            std::string wifi_channel;
            std::string changedwifisettingsbuffer;
            while (std::getline(changedwifisettingsfile, changedwifisettingsbuffer)) {
                if (changedwifisettingsbuffer.find("channel=") == 0) {
                    wifi_channel = changedwifisettingsbuffer;
                    wifi_channel.erase(0, 8);
                    break;
                }
            }
            changedwifisettingsfile.close();
            std::stringstream responsedata;
            responsedata << R"({"channel":")" << wifi_channel << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
            return;
        }
        response.set_content(R"({"access":"reject"})", JSON_TYPE);
    }
}
