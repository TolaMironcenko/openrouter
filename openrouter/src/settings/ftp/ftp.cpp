#include "ftp.hpp"
#include <colors.h>
#include <json.hpp>
#include "../../types.hpp"
#include <syslog.h>
#include "../../auth/auth.hpp"
#include "../../helpers.hpp"

#define FTP_REQUIRED_STRING R"({"required":"[token,enabled]"})"

namespace api
{
    namespace settings
    {
        void get_ftp(const httplib::Request &request, httplib::Response &response)
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
            std::ifstream ftpfile("/etc/openrouter/ftp");
            int ftp_enabled;
            ftpfile >> ftp_enabled;
            ftpfile.close();
            std::stringstream responsedata;
            responsedata << R"({"enabled":")" << ftp_enabled << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
        }

        void set_ftp(const httplib::Request &request, httplib::Response &response)
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
                response.set_content(FTP_REQUIRED_STRING, JSON_TYPE);
                return;
            }

            if (!authenticate(json_body["token"]))
            {
                response.set_content(R"({"access":"reject"})", JSON_TYPE);
                return;
            }
            std::ofstream ftpfileout("/etc/openrouter/ftp");
            std::string new_ftp_enabled = json_body["enabled"];
            if (new_ftp_enabled == "0")
            {
                system("rc stop ftp");
                system("rc del ftp");
                syslog(LOG_INFO, "%s", "Ftp is disabled");
            }
            else
            {
                system("rc start ftp");
                system("rc add ftp");
                syslog(LOG_INFO, "%s", "Ftp is enabled");
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
}
