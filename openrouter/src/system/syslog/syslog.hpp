#ifndef SYSLOG_HPP
#define SYSLOG_HPP

#include <httplib.h>

namespace api
{
    namespace system
    {
        void get_syslog(const httplib::Request &request, httplib::Response &response);
    }
}

#endif // SYSLOG_HPP