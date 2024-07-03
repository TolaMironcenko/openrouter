#ifndef FTP_HPP
#define FTP_HPP

#include <httplib.h>

void get_ftp(const httplib::Request &request, httplib::Response &response);
void set_ftp(const httplib::Request &request, httplib::Response &response);

#endif // FTP_HPP