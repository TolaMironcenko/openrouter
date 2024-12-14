#include "wifi.hpp"
#include <colors.h>
#include <json.hpp>
#include "../../types.hpp"
#include <syslog.h>
#include "../../auth/auth.hpp"
#include "../../helpers.hpp"

#define WIFI_REQUIRED_STRING R"({"required":"[token,status]"})"

namespace api
{
    namespace settings
    {
        void get_wifi_status(const httplib::Request &request, httplib::Response &response)
        {
            std::cout << GREEN << request.path << RESET << "  " << request.method << std::endl;
            response.set_header("Access-Control-Allow-Origin", "*");
            if (!check_body(request))
            {
                response.set_content(ACCESS_REQUIRED_STRING, JSON_TYPE);
                return;
            }
            nlohmann::json json_body = nlohmann::json::parse(request.body);

            if (!authenticate(json_body["token"]))
            {
                response.set_content(R"({"access":"reject"})", JSON_TYPE);
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

        void set_wifi_status(const httplib::Request &request, httplib::Response &response)
        {
            std::cout << GREEN << request.path << RESET << "  " << request.method << std::endl;
            response.set_header("Access-Control-Allow-Origin", "*");
            if (!check_body(request))
            {
                response.set_content(ACCESS_REQUIRED_STRING, JSON_TYPE);
                return;
            }
            nlohmann::json json_body = nlohmann::json::parse(request.body);

            if (json_body["status"] == nullptr)
            {
                response.set_content(WIFI_REQUIRED_STRING, JSON_TYPE);
                return;
            }

            if (!authenticate(json_body["token"]))
            {
                response.set_content(R"({"access":"reject"})", JSON_TYPE);
                return;
            }
            std::ofstream wififileout("/etc/openrouter/wifi/status");
            std::string new_wifi_status = json_body["status"];
            if (new_wifi_status == "0")
            {
                system("rc stop hostapd");
                system("rc del hostapd");
                syslog(LOG_INFO, "%s", "WIFI is disabled");
            }
            else
            {
                system("rc start hostapd");
                system("rc add hostapd");
                syslog(LOG_INFO, "%s", "WIFI is enabled");
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
