#ifndef REBOOT_HPP
#define REBOOT_HPP

#include <httplib.h>

namespace api
{
    namespace system
    {
        void reboot(const httplib::Request &request, httplib::Response &response);
    }
}

#endif // REBOOT_HPP
