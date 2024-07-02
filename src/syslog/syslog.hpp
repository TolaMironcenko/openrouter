#ifndef SYSLOG_HPP
#define SYSLOG_HPP

#include <httplib.h>

void syslog(const httplib::Request &request, httplib::Response &response);

#endif //SYSLOG_HPP