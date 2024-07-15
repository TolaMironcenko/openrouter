#include "syslog.hpp"
#include <colors.h>
#include "../types.hpp"
#include "../routes.hpp"
#include <json.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#define SYSLOG_FILE "/var/log/messages"
namespace systemfuncs {
    void get_syslog(const httplib::Request &request, httplib::Response &response) {
        std::stringstream responsedata;
        std::ifstream syslogfile(SYSLOG_FILE);
        responsedata << syslogfile.rdbuf();
        syslogfile.close();
        response.set_content(responsedata.str(), PLAIN_TEXT_TYPE);
    }
}
