#include "dns.hpp"
#include <colors.h>
#include <json.hpp>
#include "../../types.hpp"
#include <syslog.h>
#include "../../auth/auth.hpp"

namespace settings {
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

        if (authenticate(json_body["token"])) {
            std::ifstream domainsfile("/etc/dnsmasq.conf.d/hosts");
            std::stringstream responsedata;
            responsedata << R"({"domains":[)";
            std::string ip_string = "";
            std::string domain_string = "";
            int iter = 0;
            while (!domainsfile.eof()) {
                domainsfile >> ip_string;
                domainsfile >> domain_string;
                if (domain_string.empty()) {
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
            return;
        }
        response.set_content(R"({"access":"reject"})", JSON_TYPE);
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

        if (authenticate(json_body["token"])) {
            std::ofstream domainsfile("/etc/dnsmasq.conf.d/hosts");
            nlohmann::json domains = json_body["domains"];
            for (nlohmann::json_abi_v3_11_3::json domain : domains) {
                std::string ip = domain["ip"];
                std::string dom = domain["domain"];
                domainsfile << ip << " " << dom << "\n";
            }
            domainsfile.close();
            system("rc restart dnsmasq");
            syslog(LOG_INFO, "%s", "DNS local domains changed");
            std::string responsedata = R"({"success":"true"})";
            response.set_content(responsedata, JSON_TYPE);
            return;
        }
        response.set_content(R"({"access":"reject"})", JSON_TYPE);
    }
}
