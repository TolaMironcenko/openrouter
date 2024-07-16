#include "OpenRouter.hpp"
#include <colors.h>
#include "../settings/settings.hpp"
#include "../system/system.hpp"
#include <syslog.h>
#include "../security/cors.hpp"

OpenRouter::OpenRouter() {
    this->srv.Post("/api/settings/telnet/get", settings::get_telnet);
    this->srv.Post("/api/settings/telnet/set", settings::set_telnet);
    this->srv.Post("/api/settings/telnet/port/get", settings::get_telnet_port);
    this->srv.Post("/api/settings/telnet/port/set", settings::set_telnet_port);

    this->srv.Post("/api/settings/ssh/status/get", settings::get_ssh_status);
    this->srv.Post("/api/settings/ssh/status/set", settings::set_ssh_status);
    this->srv.Post("/api/settings/ssh/keys/get", settings::get_ssh_keys);
    this->srv.Post("/api/settings/ssh/keys/set", settings::set_ssh_keys);
    this->srv.Post("/api/settings/ssh/port/get", settings::get_ssh_port);
    this->srv.Post("/api/settings/ssh/port/set", settings::set_ssh_port);

    this->srv.Post("/api/settings/dns/get", settings::get_dns);
    this->srv.Post("/api/settings/dns/set", settings::set_dns);
    this->srv.Post("/api/settings/dns/servers/get", settings::get_dns_servers);
    this->srv.Post("/api/settings/dns/servers/set", settings::set_dns_servers);
    this->srv.Post("/api/settings/dns/domains/get", settings::get_local_domains);
    this->srv.Post("/api/settings/dns/domains/set", settings::set_local_domains);

    this->srv.Post("/api/settings/ftp/get", settings::get_ftp);
    this->srv.Post("/api/settings/ftp/set", settings::set_ftp);

    this->srv.Post("/api/settings/wifi/status/get", settings::get_wifi_status);
    this->srv.Post("/api/settings/wifi/status/set", settings::set_wifi_status);
    this->srv.Post("/api/settings/wifi/name/get", settings::get_wifi_name);
    this->srv.Post("/api/settings/wifi/name/set", settings::set_wifi_name);
    this->srv.Post("/api/settings/wifi/password/get", settings::get_wifi_password);
    this->srv.Post("/api/settings/wifi/password/set", settings::set_wifi_password);
    this->srv.Post("/api/settings/wifi/qr/get", settings::get_wifi_qr);
    this->srv.Post("/api/settings/wifi/channel/get", settings::get_wifi_channel);
    this->srv.Post("/api/settings/wifi/channel/set", settings::set_wifi_channel);

    this->srv.Post("/api/reboot", systemfuncs::reboot);

    this->srv.Get("/api/syslog", systemfuncs::get_syslog);
    this->srv.Get("/api/uptime", systemfuncs::get_uptime);

    this->srv.Options("/(.*)", set_cors_headers);

    this->srv.set_mount_point("/" , "../src/web");
    this->srv.set_mount_point("/js", "../src/web/js");
    this->srv.set_mount_point("/css", "../src/web/css");
}

OpenRouter::~OpenRouter() = default;

int OpenRouter::serve(const std::string *ip, int port, int socket_flags) {
    std::cout << GREEN << "Starting" << RESET << " server on [ " << YELLOW
            << "0.0.0.0" << RESET << ":" << CYAN << "43244" << RESET << " ]" << std::endl;

    syslog(LOG_INFO, "openrouter administration software started.");
    srv.listen("127.0.0.1", 43244);
    return 0;
}
