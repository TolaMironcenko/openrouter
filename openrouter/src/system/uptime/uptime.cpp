#include "uptime.hpp"
#include <sys/sysinfo.h>
#include <sstream>
#include "../types.hpp"

namespace systemfuncs {
    void get_uptime(const httplib::Request &request, httplib::Response &response) {
        struct sysinfo x;
        if (sysinfo(&x) == 0)
        {
            // std::cout << static_cast<unsigned long long>(x.uptime)/3600ULL << "h " 
            //       << static_cast<unsigned long long>(x.uptime)/60ULL%60ULL << "m "
            //       << static_cast<unsigned long long>(x.uptime)%60ULL << "s "
            //       << std::endl;
            std::stringstream responsedata;
            responsedata << R"({"uptime":{"hour":")" 
                        << static_cast<unsigned long long>(x.uptime)/3600ULL
                        << R"(","minute":")" 
                        << static_cast<unsigned long long>(x.uptime)/60ULL%60ULL
                        << R"(","second":")" 
                        << static_cast<unsigned long long>(x.uptime)%60ULL
                        << R"("}})";
            response.set_content(responsedata.str(), JSON_TYPE);
            return;
        }
        response.set_content(R"({"error":"unknown"})", JSON_TYPE);
    }
}
