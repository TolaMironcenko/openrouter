#ifndef SETTINGS_TELNET_HPP
#define SETTINGS_TELNET_HPP

#include <httplib.h>

void get_telnet(const httplib::Request &request, httplib::Response &response);
void set_telnet(const httplib::Request &request, httplib::Response &response);

void get_telnet_port(const httplib::Request &request, httplib::Response &response);

#endif // SETTINGS_TELNET_HPP
