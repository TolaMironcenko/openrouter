#include <iostream>
#include <httplib.h>
#include <colors.h>
#include "settings/telnet/telnet.hpp"
#include "settings/dns/dns.hpp"
#include "settings/ftp/ftp.hpp"
#include "reboot/reboot.hpp"

#define CORS_ENABLE
#include "security/cors.hpp"

int main() {
    httplib::Server srv;

    srv.Post("/api/settings/telnet/get", get_telnet);
    srv.Post("/api/settings/telnet/set", set_telnet);
    srv.Post("/api/settings/telnet/port/get", get_telnet_port);

    srv.Post("/api/settings/dns/get", get_dns);
    srv.Post("/api/settings/dns/set", set_dns);
    srv.Post("/api/settings/dns/servers/get", get_dns_servers);
    srv.Post("/api/settings/dns/servers/set", set_dns_servers);
    srv.Post("/api/settings/dns/domains/get", get_local_domains);
    srv.Post("/api/settings/dns/domains/set", set_local_domains);

    srv.Post("/api/settings/ftp/get", get_ftp);
    srv.Post("/api/settings/ftp/set", set_ftp);

    srv.Post("/api/reboot", reboot);

    srv.Options("/(.*)", set_cors_headers);

    srv.set_mount_point("/" , "../src/web");
    srv.set_mount_point("/js", "../src/web/js");
    srv.set_mount_point("/css", "../src/web/css");

    std::cout << GREEN << "Starting" << RESET << " server on [ " << YELLOW
            << "0.0.0.0" << RESET << ":" << CYAN << "43244" << RESET << " ]" << std::endl;

    srv.listen("0.0.0.0", 80);
    return 0;
}
