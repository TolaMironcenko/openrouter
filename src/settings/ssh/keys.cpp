#include "ssh.hpp"
#include <colors.h>
#include <json.hpp>
#include "../../types.hpp"
#include <syslog.h>
#include "../../auth/auth.hpp"
#include "../../helpers.hpp"

#define KEYS_PATH "/root/.ssh/authorized_keys"

namespace api
{
    namespace settings
    {
        void get_ssh_keys(const httplib::Request &request, httplib::Response &response)
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
            std::ifstream keysfile(KEYS_PATH);
            std::stringstream responsedata;
            responsedata << R"({"keys":[)";
            std::string key_string = "";
            int iter = 0;
            while (std::getline(keysfile, key_string))
            {
                if (key_string.empty())
                {
                    continue;
                }
                responsedata << R"(")" << key_string << R"(",)";
                key_string = "";
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
            keysfile.close();
            response.set_content(responsedata.str(), JSON_TYPE);
        }

        void set_ssh_keys(const httplib::Request &request, httplib::Response &response)
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
            std::ofstream keysfile(KEYS_PATH);
            nlohmann::json keys = json_body["keys"];
            for (std::string key : keys)
            {
                keysfile << key << "\n";
            }
            keysfile.close();
            system("rc restart ssh");
            syslog(LOG_INFO, "%s", "SSH keys changed");
            std::string responsedata = R"({"success":"true"})";
            response.set_content(responsedata, JSON_TYPE);
        }
    }
}
