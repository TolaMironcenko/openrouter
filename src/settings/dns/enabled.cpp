#include "dns.hpp"
#include <colors.h>
#include <json.hpp>
#include "../../routes.hpp"
#include "../../types.hpp"

#define DNS_REQUIRED_STRING R"({"required":"[token,enabled]"})"

void get_dns(const httplib::Request &request, httplib::Response &response) {
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
        std::ifstream dnsfile("/etc/openrouter/dns");
        int dns_enabled;
        dnsfile >> dns_enabled;
        dnsfile.close();
        std::stringstream responsedata;
        responsedata << R"({"enabled":")" << dns_enabled << R"("})";
        response.set_content(responsedata.str(), JSON_TYPE);
    }
}

void set_dns(const httplib::Request &request, httplib::Response &response) {
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
        response.set_content(DNS_REQUIRED_STRING, JSON_TYPE);
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
        std::ofstream dnsfileout("/etc/openrouter/dns");
        std::string new_dns_enabled = json_body["enabled"];
        if (new_dns_enabled == "0") {
            system("/etc/init.d/dnsmasq stop");
            system("rm /etc/rc.d/60dnsmasq");
        } else {
            system("/etc/init.d/dnsmasq restart");
            system("ln -sv /etc/init.d/dnsmasq /etc/rc.d/60dnsmasq");
        }
        dnsfileout << stoi(new_dns_enabled);
        dnsfileout.close();
        std::ifstream dnsfile("/etc/openrouter/dns");
        int dns_enabled;
        dnsfile >> dns_enabled;
        dnsfile.close();
        std::stringstream responsedata;
        responsedata << R"({"enabled":")" << dns_enabled << R"("})";
        response.set_content(responsedata.str(), JSON_TYPE);
    }
}
