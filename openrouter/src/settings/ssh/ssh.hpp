#ifndef SSH_HPP
#define SSH_HPP

#include <httplib.h>

void get_ssh_status(const httplib::Request &request, httplib::Response &response);
void set_ssh_status(const httplib::Request &request, httplib::Response &response);

void get_ssh_keys(const httplib::Request &request, httplib::Response &response);
void set_ssh_keys(const httplib::Request &request, httplib::Response &response);

void get_ssh_port(const httplib::Request &request, httplib::Response &response);
void set_ssh_port(const httplib::Request &request, httplib::Response &response);

#endif // SSH_HPP
