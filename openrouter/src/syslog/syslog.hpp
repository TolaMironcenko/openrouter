#ifndef SYSLOG_HPP
#define SYSLOG_HPP

#include <httplib.h>

void get_syslog(const httplib::Request &request, httplib::Response &response);

#endif //SYSLOG_HPP