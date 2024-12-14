#include "telnet.hpp"
#include <colors.h>
#include "../../types.hpp"
#include <json.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <syslog.h>
#include "../../auth/auth.hpp"
#include "../../helpers.hpp"

#define TELNET_REQUIRED_STRING R"({"required":"[token,enabled]"})"

namespace api
{
    namespace settings
    {
        void get_telnet(const httplib::Request &request, httplib::Response &response)
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
            std::ifstream telnetfile("/etc/openrouter/telnet/enabled");
            int telnet_enabled;
            telnetfile >> telnet_enabled;
            telnetfile.close();
            std::stringstream responsedata;
            responsedata << R"({"enabled":")" << telnet_enabled << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
        }

        void set_telnet(const httplib::Request &request, httplib::Response &response)
        {
            std::cout << GREEN << request.path << RESET << "  " << request.method << std::endl;
            response.set_header("Access-Control-Allow-Origin", "*");
            if (!check_body(request))
            {
                response.set_content(ACCESS_REQUIRED_STRING, JSON_TYPE);
                return;
            }
            nlohmann::json json_body = nlohmann::json::parse(request.body);

            if (json_body["enabled"] == nullptr)
            {
                response.set_content(TELNET_REQUIRED_STRING, JSON_TYPE);
                return;
            }

            if (!authenticate(json_body["token"]))
            {
                response.set_content(R"({"access":"reject"})", JSON_TYPE);
                return;
            }
            std::ofstream telnetfileout("/etc/openrouter/telnet/enabled");
            std::string new_telnet_enabled = json_body["enabled"];
            if (new_telnet_enabled == "0")
            {
                system("rc stop telnet");
                system("rc del telnet");
                syslog(LOG_INFO, "%s", "Telnet is disabled");
            }
            else
            {
                system("rc start telnet");
                system("rc add telnet");
                syslog(LOG_INFO, "%s", "Telnet is enabled");
            }
            telnetfileout << stoi(new_telnet_enabled);
            telnetfileout.close();
            std::ifstream telnetfile("/etc/openrouter/telnet/enabled");
            int telnet_enabled;
            telnetfile >> telnet_enabled;
            telnetfile.close();
            std::stringstream responsedata;
            responsedata << R"({"enabled":")" << telnet_enabled << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
        }
    }
}
