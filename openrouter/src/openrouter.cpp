#include <iostream>
#include <httplib.h>
#include <colors.h>
#include "settings/telnet/telnet.hpp"
#include "settings/ssh/ssh.hpp"
#include "settings/dns/dns.hpp"
#include "settings/ftp/ftp.hpp"
#include "settings/wifi/wifi.hpp"
#include "reboot/reboot.hpp"
#include "syslog/syslog.hpp"
#include <syslog.h>

#define CORS_ENABLE
#include "security/cors.hpp"

int main() {
    httplib::Server srv;

    srv.Post("/api/settings/telnet/get", get_telnet);
    srv.Post("/api/settings/telnet/set", set_telnet);
    srv.Post("/api/settings/telnet/port/get", get_telnet_port);
    srv.Post("/api/settings/telnet/port/set", set_telnet_port);

    srv.Post("/api/settings/ssh/status/get", get_ssh_status);
    srv.Post("/api/settings/ssh/status/set", set_ssh_status);
    srv.Post("/api/settings/ssh/keys/get", get_ssh_keys);
    srv.Post("/api/settings/ssh/keys/set", set_ssh_keys);
    srv.Post("/api/settings/ssh/port/get", get_ssh_port);
    srv.Post("/api/settings/ssh/port/set", set_ssh_port);

    srv.Post("/api/settings/dns/get", get_dns);
    srv.Post("/api/settings/dns/set", set_dns);
    srv.Post("/api/settings/dns/servers/get", get_dns_servers);
    srv.Post("/api/settings/dns/servers/set", set_dns_servers);
    srv.Post("/api/settings/dns/domains/get", get_local_domains);
    srv.Post("/api/settings/dns/domains/set", set_local_domains);

    srv.Post("/api/settings/ftp/get", get_ftp);
    srv.Post("/api/settings/ftp/set", set_ftp);

    srv.Post("/api/settings/wifi/status/get", get_wifi_status);
    srv.Post("/api/settings/wifi/status/set", set_wifi_status);
    srv.Post("/api/settings/wifi/name/get", get_wifi_name);
    srv.Post("/api/settings/wifi/name/set", set_wifi_name);
    srv.Post("/api/settings/wifi/password/get", get_wifi_password);
    srv.Post("/api/settings/wifi/password/set", set_wifi_password);
    srv.Post("/api/settings/wifi/qr/get", get_wifi_qr);

    srv.Post("/api/reboot", reboot);

    srv.Get("/api/syslog", get_syslog);

    srv.Options("/(.*)", set_cors_headers);

    srv.set_mount_point("/" , "../src/web");
    srv.set_mount_point("/js", "../src/web/js");
    srv.set_mount_point("/css", "../src/web/css");

    std::cout << GREEN << "Starting" << RESET << " server on [ " << YELLOW
            << "0.0.0.0" << RESET << ":" << CYAN << "43244" << RESET << " ]" << std::endl;

    syslog(LOG_INFO, "openrouter administration software started.");
    srv.listen("127.0.0.1", 43244);
    return 0;
}
