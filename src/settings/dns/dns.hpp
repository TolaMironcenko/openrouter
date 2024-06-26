#ifndef DNS_HPP
#define DNS_HPP

#include <httplib.h>

void get_dns(const httplib::Request &request, httplib::Response &response);
void set_dns(const httplib::Request &request, httplib::Response &response);

#endif // DNS_HPP