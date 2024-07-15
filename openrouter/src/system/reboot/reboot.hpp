#ifndef REBOOT_HPP
#define REBOOT_HPP

#include <httplib.h>
namespace systemfuncs {
    void reboot(const httplib::Request &request, httplib::Response &response);
}

#endif // REBOOT_HPP
