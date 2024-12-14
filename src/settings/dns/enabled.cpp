#include "dns.hpp"
#include <colors.h>
#include <json.hpp>
#include "../../types.hpp"
#include <syslog.h>
#include "../../auth/auth.hpp"
#include "../../helpers.hpp"

#define DNS_REQUIRED_STRING R"({"required":"[token,enabled]"})"

namespace api
{
    namespace settings
    {
        void get_dns(const httplib::Request &request, httplib::Response &response)
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
            std::ifstream dnsfile("/etc/openrouter/dns");
            int dns_enabled;
            dnsfile >> dns_enabled;
            dnsfile.close();
            std::stringstream responsedata;
            responsedata << R"({"enabled":")" << dns_enabled << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
        }

        void set_dns(const httplib::Request &request, httplib::Response &response)
        {
            std::cout << GREEN << request.path << RESET << "  " << request.method << std::endl;
            response.set_header("Access-Control-Allow-Origin", "*");
            if (!check_body(request))
            {
                response.set_content(ACCESS_REQUIRED_STRING, JSON_TYPE);
                return;
            }
            nlohmann::json json_body = nlohmann::json::parse(request.body);

            if (json_body["enabled"] == nullptr)
            {
                response.set_content(DNS_REQUIRED_STRING, JSON_TYPE);
                return;
            }

            if (!authenticate(json_body["token"]))
            {
                response.set_content(R"({"access":"reject"})", JSON_TYPE);
                return;
            }
            std::ofstream dnsfileout("/etc/openrouter/dns");
            std::string new_dns_enabled = json_body["enabled"];
            if (new_dns_enabled == "0")
            {
                system("rc stop dnsmasq");
                system("rc del dnsmasq");
                syslog(LOG_INFO, "%s", "DNS is disabled");
            }
            else
            {
                system("rc restart dnsmasq");
                system("rc add dnsmasq");
                syslog(LOG_INFO, "%s", "DNS is enabled");
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
}
