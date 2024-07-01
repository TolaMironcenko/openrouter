#include "dns.hpp"
#include <colors.h>
#include <json.hpp>
#include "../../routes.hpp"
#include "../../types.hpp"

void get_local_domains(const httplib::Request &request, httplib::Response &response) {
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
        std::ifstream domainsfile("/etc/dnsmasq.conf.d/hosts");
        std::stringstream responsedata;
        responsedata << R"({"domains":[)";
        std::string ip_string = "";
        std::string domain_string = "";
        int iter = 0;
        while (!domainsfile.eof()) {
            domainsfile >> ip_string;
            domainsfile >> domain_string;
            if (domain_string == "") {
                continue;
            }
            responsedata << R"({"ip":")" << ip_string << R"(","domain":")" << domain_string << R"("},)";
            ip_string = "";
            domain_string = "";
            iter++;
        }
        if (iter != 0) {
            responsedata.str(responsedata.str().erase(responsedata.str().size()-1, 1) + "]}");
        } else {
            responsedata << "]}";
        }
        domainsfile.close();
        response.set_content(responsedata.str(), JSON_TYPE);
    }
}

void set_local_domains(const httplib::Request &request, httplib::Response &response) {
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
        std::ofstream domainsfile("/etc/dnsmasq.conf.d/hosts");
        nlohmann::json domains = json_body["domains"];
        for (nlohmann::json_abi_v3_11_3::json domain : domains) {
            std::string ip = domain["ip"];
            std::string dom = domain["domain"];
            domainsfile << ip << " " << dom << "\n";
        }
        domainsfile.close();
        system("/etc/init.d/dnsmasq restart");
        std::string responsedata = R"({"success":"true"})";
        response.set_content(responsedata, JSON_TYPE);
    }
}
