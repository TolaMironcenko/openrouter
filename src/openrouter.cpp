#include <iostream>
#include <httplib.h>
#include <colors.h>

#define CORS_ENABLE
#include "security/cors.hpp"

int main() {
    httplib::Server srv;

    srv.Options("/(.*)", set_cors_headers);

    srv.set_mount_point("/" , "../src/web");
    srv.set_mount_point("/js", "../src/web/js");
    srv.set_mount_point("/css", "../src/web/css");

    std::cout << GREEN << "Starting" << RESET << " server on [ " << YELLOW
            << "localhost" << RESET << ":" << CYAN << "43243" << RESET << " ]" << std::endl;

    srv.listen("localhost", 43244);
    return 0;
}
