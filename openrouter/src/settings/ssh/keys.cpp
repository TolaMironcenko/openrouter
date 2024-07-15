#include "ssh.hpp"
#include <colors.h>
#include <json.hpp>
#include "../../routes.hpp"
#include "../../types.hpp"
#include <syslog.h>
#include "../../auth/auth.hpp"

#define KEYS_PATH "/root/.ssh/authorized_keys"

namespace settings {
    void get_ssh_keys(const httplib::Request &request, httplib::Response &response) {
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
            std::ifstream keysfile(KEYS_PATH);
            std::stringstream responsedata;
            responsedata << R"({"keys":[)";
            std::string key_string = "";
            int iter = 0;
            while (std::getline(keysfile, key_string)) {
                if (key_string == "") {
                    continue;
                }
                responsedata << R"(")" << key_string << R"(",)";
                key_string = "";
                iter++;
            }
            if (iter != 0) {
                responsedata.str(responsedata.str().erase(responsedata.str().size()-1, 1) + "]}");
            } else {
                responsedata << "]}";
            }
            keysfile.close();
            response.set_content(responsedata.str(), JSON_TYPE);
            return;
        }
        response.set_content(R"({"access":"reject"})", JSON_TYPE);
    }

    void set_ssh_keys(const httplib::Request &request, httplib::Response &response) {
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
            std::ofstream keysfile(KEYS_PATH);
            nlohmann::json keys = json_body["keys"];
            for (std::string key : keys) {
                keysfile << key << "\n";
            }
            keysfile.close();
            system("rc restart ssh");
            syslog(LOG_INFO, "SSH keys changed");
            std::string responsedata = R"({"success":"true"})";
            response.set_content(responsedata, JSON_TYPE);
            return;
        }
        response.set_content(R"({"access":"reject"})", JSON_TYPE);
    }
}
