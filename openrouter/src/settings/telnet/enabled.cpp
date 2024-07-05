#include "telnet.hpp"
#include <colors.h>
#include "../../types.hpp"
#include "../../routes.hpp"
#include <json.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <syslog.h>

#define TELNET_REQUIRED_STRING R"({"required":"[token,enabled]"})"

void get_telnet(const httplib::Request &request, httplib::Response &response) {
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
        std::ifstream telnetfile("/etc/openrouter/telnet/enabled");
        int telnet_enabled;
        telnetfile >> telnet_enabled;
        telnetfile.close();
        std::stringstream responsedata;
        responsedata << R"({"enabled":")" << telnet_enabled << R"("})";
        response.set_content(responsedata.str(), JSON_TYPE);
    }
}

void set_telnet(const httplib::Request &request, httplib::Response &response) {
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

    if (json_body["enabled"] == nullptr) {
        response.set_content(TELNET_REQUIRED_STRING, JSON_TYPE);
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
        std::ofstream telnetfileout("/etc/openrouter/telnet/enabled");
        std::string new_telnet_enabled = json_body["enabled"];
        if (new_telnet_enabled == "0") {
            system("/etc/init.d/telnet stop");
            system("rm-rc.d 70telnet");
            syslog(LOG_INFO, "Telnet is disabled");
        } else {
            system("/etc/init.d/telnet start");
            system("add-rc.d telnet 70");
            syslog(LOG_INFO, "Telnet is enabled");
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
