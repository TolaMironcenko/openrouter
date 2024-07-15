#include "wifi.hpp"
#include <colors.h>
#include <json.hpp>
#include "../../routes.hpp"
#include "../../types.hpp"
#include <syslog.h>

#define WIFI_REQUIRED_STRING R"({"required":"[token,status]"})"
namespace settings {
    void get_wifi_status(const httplib::Request &request, httplib::Response &response) {
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
            std::ifstream wifistatusfile("/etc/openrouter/wifi/status");
            int wifi_status;
            wifistatusfile >> wifi_status;
            wifistatusfile.close();
            std::stringstream responsedata;
            responsedata << R"({"status":")" << wifi_status << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
        }
    }

    void set_wifi_status(const httplib::Request &request, httplib::Response &response) {
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

        if (json_body["status"] == nullptr) {
            response.set_content(WIFI_REQUIRED_STRING, JSON_TYPE);
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
            std::ofstream wififileout("/etc/openrouter/wifi/status");
            std::string new_wifi_status = json_body["status"];
            if (new_wifi_status == "0") {
                system("rc stop hostapd");
                system("rc del hostapd");
                syslog(LOG_INFO, "WIFI is disabled");
            } else {
                system("rc start hostapd");
                system("rc add hostapd");
                syslog(LOG_INFO, "WIFI is enabled");
            }
            wififileout << stoi(new_wifi_status);
            wififileout.close();
            std::ifstream wififile("/etc/openrouter/wifi/status");
            int wifi_status;
            wififile >> wifi_status;
            wififile.close();
            std::stringstream responsedata;
            responsedata << R"({"status":")" << wifi_status << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
        }
    }
}
