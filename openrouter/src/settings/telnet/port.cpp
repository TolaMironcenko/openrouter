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

#define TELNET_PORT_REQUIRED_STRING R"({"required":"[token,port]"})"

namespace settings {
    void get_telnet_port(const httplib::Request &request, httplib::Response &response) {
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
            std::ifstream telnetportfile("/etc/openrouter/telnet/port");
            int telnet_port;
            telnetportfile >> telnet_port;
            telnetportfile.close();
            std::stringstream responsedata;
            responsedata << R"({"port":")" << telnet_port << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
            return;
        }
        response.set_content(R"({"access":"reject"})", JSON_TYPE);
    }

    void set_telnet_port(const httplib::Request &request, httplib::Response &response) {
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

        if (json_body["port"] == nullptr) {
            response.set_content(TELNET_PORT_REQUIRED_STRING, JSON_TYPE);
            return;
        }

        if (authenticate(json_body["token"])) {
            std::ofstream telnetportfileout("/etc/openrouter/telnet/port");
            std::string new_telnet_port = json_body["port"];
            telnetportfileout << stoi(new_telnet_port);
            telnetportfileout.close();
            system("rc restart telnet");
            std::string syslogstring = "Telnet port changed to " + new_telnet_port;
            syslog(LOG_INFO, syslogstring.c_str());
            std::ifstream telnetportfile("/etc/openrouter/telnet/port");
            int telnet_port;
            telnetportfile >> telnet_port;
            telnetportfile.close();
            std::stringstream responsedata;
            responsedata << R"({"port":")" << telnet_port << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
            return;
        }
        response.set_content(R"({"access":"reject"})", JSON_TYPE);
    }
}
