#include "ssh.hpp"
#include <colors.h>
#include "../../types.hpp"
#include "../../routes.hpp"
#include <json.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <syslog.h>

#define SSH_PORT_REQUIRED_STRING R"({"required":"[token,port]"})"
#define SSH_PORT_PATH "/etc/openrouter/ssh/port"

namespace settings {
    void get_ssh_port(const httplib::Request &request, httplib::Response &response) {
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
            std::ifstream sshportfile(SSH_PORT_PATH);
            int ssh_port;
            sshportfile >> ssh_port;
            sshportfile.close();
            std::stringstream responsedata;
            responsedata << R"({"port":")" << ssh_port << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
        }
    }
    void set_ssh_port(const httplib::Request &request, httplib::Response &response) {
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
            response.set_content(SSH_PORT_REQUIRED_STRING, JSON_TYPE);
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
            std::ifstream oldsshportfile(SSH_PORT_PATH);
            int old_ssh_port;
            oldsshportfile >> old_ssh_port;
            oldsshportfile.close();
            std::ofstream sshportfileout(SSH_PORT_PATH);
            std::string new_ssh_port = json_body["port"];
            sshportfileout << stoi(new_ssh_port);
            sshportfileout.close();
            std::stringstream systemrequest;
            systemrequest << "sed -i \"/Port " << old_ssh_port << "/s//Port " << new_ssh_port << "/\" /etc/ssh/sshd_config";
            std::cout << systemrequest.str() << std::endl;
            system(systemrequest.str().c_str());
            system("rc restart ssh");
            std::string syslogstring = "SSH port changed to " + new_ssh_port;
            syslog(LOG_INFO, syslogstring.c_str());
            std::ifstream sshportfile(SSH_PORT_PATH);
            int ssh_port;
            sshportfile >> ssh_port;
            sshportfile.close();
            std::stringstream responsedata;
            responsedata << R"({"port":")" << ssh_port << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
        }
    }
}
