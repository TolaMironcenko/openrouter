#include "dns.hpp"
#include <colors.h>
#include <json.hpp>
#include "../../routes.hpp"
#include "../../types.hpp"

// #define DNS_REQUIRED_STRING R"({"required":"[token,enabled]"})"

void get_dns_servers(const httplib::Request &request, httplib::Response &response) {
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
        std::ifstream dnsserversfile("/etc/dnsmasq.conf.d/resolv.conf");
        std::stringstream responsedata;
        responsedata << R"({"servers":[")";
        std::string dns_server_string = "";
        while (std::getline(dnsserversfile, dns_server_string)) {
            dns_server_string.erase(0, 11);
            responsedata << dns_server_string << R"(",")";
            dns_server_string = "";
        }
        responsedata.str(responsedata.str().erase(responsedata.str().size()-2, 2) + "]}");
        dnsserversfile.close();
        response.set_content(responsedata.str(), JSON_TYPE);
    }
}

void set_dns_servers(const httplib::Request &request, httplib::Response &response) {
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
        std::ofstream dnsserversfile("/etc/dnsmasq.conf.d/resolv.conf");
        nlohmann::json servers = json_body["servers"];
        for (nlohmann::json_abi_v3_11_3::json server : servers) {
            std::string srv = server;
            dnsserversfile << "nameserver " << srv << "\n";
        }
        dnsserversfile.close();
        system("/etc/init.d/dnsmasq restart");
        std::string responsedata = R"({"success":"true"})";
        response.set_content(responsedata, JSON_TYPE);
    }
}
