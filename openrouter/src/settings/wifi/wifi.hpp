#ifndef WIFI_HPP
#define WIFI_HPP

#include <httplib.h>

void get_wifi_status(const httplib::Request &request, httplib::Response &response);

void get_wifi_name(const httplib::Request &request, httplib::Response &response);

void get_wifi_password(const httplib::Request &request, httplib::Response &response);

#endif // WIFI_HPP