#include "ssh.hpp"
#include <colors.h>
#include "../../types.hpp"
#include <json.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <syslog.h>
#include "../../auth/auth.hpp"
#include "../../helpers.hpp"

#define SSH_PORT_REQUIRED_STRING R"({"required":"[token,port]"})"
#define SSH_PORT_PATH "/etc/openrouter/ssh/port"

namespace api
{
    namespace settings
    {
        void get_ssh_port(const httplib::Request &request, httplib::Response &response)
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
            std::ifstream sshportfile(SSH_PORT_PATH);
            int ssh_port;
            sshportfile >> ssh_port;
            sshportfile.close();
            std::stringstream responsedata;
            responsedata << R"({"port":")" << ssh_port << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
        }

        void set_ssh_port(const httplib::Request &request, httplib::Response &response)
        {
            std::cout << GREEN << request.path << RESET << "  " << request.method << std::endl;
            response.set_header("Access-Control-Allow-Origin", "*");
            if (!check_body(request))
            {
                response.set_content(ACCESS_REQUIRED_STRING, JSON_TYPE);
                return;
            }
            nlohmann::json json_body = nlohmann::json::parse(request.body);

            if (json_body["port"] == nullptr)
            {
                response.set_content(SSH_PORT_REQUIRED_STRING, JSON_TYPE);
                return;
            }

            if (!authenticate(json_body["token"]))
            {
                response.set_content(R"({"access":"reject"})", JSON_TYPE);
                return;
            }
            std::ifstream oldsshportfile(SSH_PORT_PATH);
            int old_ssh_port;
            oldsshportfile >> old_ssh_port;
            oldsshportfile.close();
            std::ofstream sshportfileout(SSH_PORT_PATH);
            std::string new_ssh_port = json_body["port"];
            sshportfileout << stoi(new_ssh_port);
            sshportfileout.close();
            std::stringstream systemrequest;
            systemrequest << "sed -i \"/Port " << old_ssh_port << "/s//Port " << new_ssh_port << "/\" /etc/ssh/sshd_config";
            std::cout << systemrequest.str() << std::endl;
            system(systemrequest.str().c_str());
            system("rc restart ssh");
            std::string syslogstring = "SSH port changed to " + new_ssh_port;
            syslog(LOG_INFO, "%s", syslogstring.c_str());
            std::ifstream sshportfile(SSH_PORT_PATH);
            int ssh_port;
            sshportfile >> ssh_port;
            sshportfile.close();
            std::stringstream responsedata;
            responsedata << R"({"port":")" << ssh_port << R"("})";
            response.set_content(responsedata.str(), JSON_TYPE);
        }
    }
}
