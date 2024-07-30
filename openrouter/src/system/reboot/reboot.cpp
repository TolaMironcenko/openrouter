#include "reboot.hpp"
#include <json.hpp>
#include <colors.h>
#include "../types.hpp"
#include "../../auth/auth.hpp"

namespace api
{
    namespace system
    {
        void reboot(const httplib::Request &request, httplib::Response &response)
        {
            std::cout << GREEN << request.path << RESET << "  " << request.method << std::endl;
            response.set_header("Access-Control-Allow-Origin", "*");
            if (request.body.empty())
            {
                response.set_content(ACCESS_REQUIRED_STRING, JSON_TYPE);
                return;
            }

            nlohmann::json json_body = nlohmann::json::parse(request.body);
            if (json_body["token"] == nullptr)
            {
                response.set_content(ACCESS_REQUIRED_STRING, JSON_TYPE);
                return;
            }

            if (authenticate(json_body["token"]))
            {
                std::system("reboot");
            }
            response.set_content(R"({"access":"reject"})", JSON_TYPE);
        }
    }
}
