#ifndef DNS_HPP
#define DNS_HPP

#include <httplib.h>

namespace api
{
    namespace settings
    {
        void get_dns(const httplib::Request &request, httplib::Response &response);
        void set_dns(const httplib::Request &request, httplib::Response &response);

        void get_dns_servers(const httplib::Request &request, httplib::Response &response);
        void set_dns_servers(const httplib::Request &request, httplib::Response &response);

        void get_local_domains(const httplib::Request &request, httplib::Response &response);
        void set_local_domains(const httplib::Request &request, httplib::Response &response);
    }
}

#endif // DNS_HPP