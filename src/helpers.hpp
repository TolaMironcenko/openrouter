#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <httplib.h>
#include <json.hpp>

static bool check_body(const httplib::Request &request)
{
    if (request.body.empty())
        return false;
    nlohmann::json json_body = nlohmann::json::parse(request.body);
    if (json_body["token"] == nullptr)
        return false;
    return true;
}

#endif // HELPERS_HPP