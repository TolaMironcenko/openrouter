#ifndef UPTIME_HPP
#define UPTIME_HPP

#include <httplib.h>

namespace api
{
    namespace system
    {
        void get_uptime(const httplib::Request &request, httplib::Response &response);
    }
}

#endif // UPTIME_HPP