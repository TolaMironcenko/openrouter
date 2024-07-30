#include "dns.hpp"
#include <colors.h>
#include <json.hpp>
#include "../../types.hpp"
#include <syslog.h>
#include "../../auth/auth.hpp"
#include "../../helpers.hpp"

namespace api
{
    namespace settings
    {
        void get_dns_servers(const httplib::Request &request, httplib::Response &response)
        {
            std::cout << GREEN << request.path << RESET << "  " << request.method << std::endl;
            response.set_header("Access-Control-Allow-Origin", "*");
            if (!check_body(request))
            {
                response.set_content(ACCESS_REQUIRED_STRING, JSON_TYPE);
                return;
            }
            nlohmann::json json_body = nlohmann::json::parse(request.body);

            if (!authenticate(json_body["token"]))
            {
                response.set_content(R"({"access":"reject"})", JSON_TYPE);
                return;
            }
            std::ifstream dnsserversfile("/etc/dnsmasq.conf.d/resolv.conf");
            std::stringstream responsedata;
            responsedata << R"({"servers":[)";
            std::string dns_server_string = "";
            int iter = 0;
            while (std::getline(dnsserversfile, dns_server_string))
            {
                dns_server_string.erase(0, 11);
                responsedata << "\"" << dns_server_string << R"(",)";
                dns_server_string = "";
                iter++;
            }
            if (iter != 0)
            {
                responsedata.str(responsedata.str().erase(responsedata.str().size() - 1, 1) + "]}");
            }
            else
            {
                responsedata << "]}";
            }
            dnsserversfile.close();
            response.set_content(responsedata.str(), JSON_TYPE);
        }

        void set_dns_servers(const httplib::Request &request, httplib::Response &response)
        {
            std::cout << GREEN << request.path << RESET << "  " << request.method << std::endl;
            response.set_header("Access-Control-Allow-Origin", "*");
            if (!check_body(request))
            {
                response.set_content(ACCESS_REQUIRED_STRING, JSON_TYPE);
                return;
            }
            nlohmann::json json_body = nlohmann::json::parse(request.body);

            if (!authenticate(json_body["token"]))
            {
                response.set_content(R"({"access":"reject"})", JSON_TYPE);
                return;
            }
            std::ofstream dnsserversfile("/etc/dnsmasq.conf.d/resolv.conf");
            nlohmann::json servers = json_body["servers"];
            for (const nlohmann::json_abi_v3_11_3::json &server : servers)
            {
                std::string srv = server;
                if (server == "")
                {
                    continue;
                }
                dnsserversfile << "nameserver " << srv << "\n";
            }
            dnsserversfile.close();
            system("rc restart dnsmasq");
            syslog(LOG_INFO, "%s", "DNS servers changed");
            std::string responsedata = R"({"success":"true"})";
            response.set_content(responsedata, JSON_TYPE);
        }
    }
}
