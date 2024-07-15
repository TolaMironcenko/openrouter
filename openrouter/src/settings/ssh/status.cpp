#include "ssh.hpp"
#include <colors.h>
#include <json.hpp>
#include "../../routes.hpp"
#include "../../types.hpp"
#include <syslog.h>
#include "../../auth/auth.hpp"

#define SSH_REQUIRED_STRING R"({"required":"[token,status]"})"

namespace settings {
    void get_ssh_status(const httplib::Request &request, httplib::Response &response) {
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
            std::ifstream sshstatusfile("/etc/openrouter/ssh/status");
            int ssh_status;
            sshstatusfile >> ssh_status;
            sshstatusfile.close();
            std::stringstream responsedata;
            responsedata << R"({"status":")" << ssh_status << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
            return;
        }
        response.set_content(R"({"access":"reject"})", JSON_TYPE);
    }

    void set_ssh_status(const httplib::Request &request, httplib::Response &response) {
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
            response.set_content(SSH_REQUIRED_STRING, JSON_TYPE);
            return;
        }

        if (authenticate(json_body["token"])) {
            std::ofstream sshfileout("/etc/openrouter/ssh/status");
            std::string new_ssh_status = json_body["status"];
            if (new_ssh_status == "0") {
                system("rc stop ssh");
                system("rc del ssh");
                syslog(LOG_INFO, "SSH is disabled");
            } else {
                system("rc start ssh");
                system("rc add ssh");
                syslog(LOG_INFO, "SSH is enabled");
            }
            sshfileout << stoi(new_ssh_status);
            sshfileout.close();
            std::ifstream sshfile("/etc/openrouter/ssh/status");
            int ssh_status;
            sshfile >> ssh_status;
            sshfile.close();
            std::stringstream responsedata;
            responsedata << R"({"status":")" << ssh_status << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
            return;
        }
        response.set_content(R"({"access":"reject"})", JSON_TYPE);
    }
}
