#include "ftp.hpp"
#include <colors.h>
#include <json.hpp>
#include "../../routes.hpp"
#include "../../types.hpp"
#include <syslog.h>

#define FTP_REQUIRED_STRING R"({"required":"[token,enabled]"})"

void get_ftp(const httplib::Request &request, httplib::Response &response) {
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
        std::ifstream ftpfile("/etc/openrouter/ftp");
        int ftp_enabled;
        ftpfile >> ftp_enabled;
        ftpfile.close();
        std::stringstream responsedata;
        responsedata << R"({"enabled":")" << ftp_enabled << R"("})";
        response.set_content(responsedata.str(), JSON_TYPE);
    }
}

void set_ftp(const httplib::Request &request, httplib::Response &response) {
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
        response.set_content(FTP_REQUIRED_STRING, JSON_TYPE);
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
        std::ofstream ftpfileout("/etc/openrouter/ftp");
        std::string new_ftp_enabled = json_body["enabled"];
        if (new_ftp_enabled == "0") {
            system("/etc/init.d/ftp stop");
            system("rm /etc/rc.d/90ftp");
            syslog(LOG_INFO, "Ftp is disabled");
        } else {
            system("/etc/init.d/ftp start");
            system("ln -sv /etc/init.d/ftp /etc/rc.d/90ftp");
            syslog(LOG_INFO, "Ftp is enabled");
        }
        ftpfileout << stoi(new_ftp_enabled);
        ftpfileout.close();
        std::ifstream ftpfile("/etc/openrouter/ftp");
        int ftp_enabled;
        ftpfile >> ftp_enabled;
        ftpfile.close();
        std::stringstream responsedata;
        responsedata << R"({"enabled":")" << ftp_enabled << R"("})";
        response.set_content(responsedata.str(), JSON_TYPE);
    }
}
