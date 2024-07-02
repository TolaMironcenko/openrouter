#include "ssh.hpp"
#include <colors.h>
#include <json.hpp>
#include "../../routes.hpp"
#include "../../types.hpp"

#define SSH_REQUIRED_STRING R"({"required":"[token,status]"})"

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
        std::ifstream sshstatusfile("/etc/openrouter/ssh/status");
        int ssh_status;
        sshstatusfile >> ssh_status;
        sshstatusfile.close();
        std::stringstream responsedata;
        responsedata << R"({"status":")" << ssh_status << R"("})";
        response.set_content(responsedata.str(), JSON_TYPE);
    }
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
        std::ofstream sshfileout("/etc/openrouter/ssh/status");
        std::string new_ssh_status = json_body["status"];
        if (new_ssh_status == "0") {
            system("/etc/init.d/ssh stop");
            system("rm /etc/rc.d/75ssh");
        } else {
            system("/etc/init.d/ssh start");
            system("ln -sv /etc/init.d/ssh /etc/rc.d/75ssh");
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
    }
}
