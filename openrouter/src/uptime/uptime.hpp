#ifndef UPTIME_HPP
#define UPTIME_HPP

#include <httplib.h>

void get_uptime(const httplib::Request &request, httplib::Response &response);

#endif // UPTIME_HPP