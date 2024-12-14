#include "OpenRouter.hpp"
#include <colors.h>
#include "../settings/settings.hpp"
#include "../system/system.hpp"
#include <syslog.h>
#include "../security/cors.hpp"

OpenRouter::OpenRouter()
{
    this->srv.Post("/api/settings/telnet/get", api::settings::get_telnet);
    this->srv.Post("/api/settings/telnet/set", api::settings::set_telnet);
    this->srv.Post("/api/settings/telnet/port/get", api::settings::get_telnet_port);
    this->srv.Post("/api/settings/telnet/port/set", api::settings::set_telnet_port);

    this->srv.Post("/api/settings/ssh/status/get", api::settings::get_ssh_status);
    this->srv.Post("/api/settings/ssh/status/set", api::settings::set_ssh_status);
    this->srv.Post("/api/settings/ssh/keys/get", api::settings::get_ssh_keys);
    this->srv.Post("/api/settings/ssh/keys/set", api::settings::set_ssh_keys);
    this->srv.Post("/api/settings/ssh/port/get", api::settings::get_ssh_port);
    this->srv.Post("/api/settings/ssh/port/set", api::settings::set_ssh_port);

    this->srv.Post("/api/settings/dns/get", api::settings::get_dns);
    this->srv.Post("/api/settings/dns/set", api::settings::set_dns);
    this->srv.Post("/api/settings/dns/servers/get", api::settings::get_dns_servers);
    this->srv.Post("/api/settings/dns/servers/set", api::settings::set_dns_servers);
    this->srv.Post("/api/settings/dns/domains/get", api::settings::get_local_domains);
    this->srv.Post("/api/settings/dns/domains/set", api::settings::set_local_domains);

    this->srv.Post("/api/settings/ftp/get", api::settings::get_ftp);
    this->srv.Post("/api/settings/ftp/set", api::settings::set_ftp);

    this->srv.Post("/api/settings/wifi/status/get", api::settings::get_wifi_status);
    this->srv.Post("/api/settings/wifi/status/set", api::settings::set_wifi_status);
    this->srv.Post("/api/settings/wifi/name/get", api::settings::get_wifi_name);
    this->srv.Post("/api/settings/wifi/name/set", api::settings::set_wifi_name);
    this->srv.Post("/api/settings/wifi/password/get", api::settings::get_wifi_password);
    this->srv.Post("/api/settings/wifi/password/set", api::settings::set_wifi_password);
    this->srv.Post("/api/settings/wifi/qr/get", api::settings::get_wifi_qr);
    this->srv.Post("/api/settings/wifi/channel/get", api::settings::get_wifi_channel);
    this->srv.Post("/api/settings/wifi/channel/set", api::settings::set_wifi_channel);

    this->srv.Post("/api/reboot", api::system::reboot);

    this->srv.Get("/api/syslog", api::system::get_syslog);
#ifdef __linux__
    this->srv.Get("/api/uptime", api::system::get_uptime);
#endif // __linux__

    this->srv.Options("/(.*)", api::security::set_cors_headers);

    this->srv.set_mount_point("/", "../src/web");
    this->srv.set_mount_point("/js", "../src/web/js");
    this->srv.set_mount_point("/css", "../src/web/css");
}

OpenRouter::~OpenRouter() = default;

int OpenRouter::serve(const std::string &ip, int port, int socket_flags)
{
    std::cout << GREEN << "Starting" << RESET << " server on [ " << YELLOW
              << "0.0.0.0" << RESET << ":" << CYAN << "43244" << RESET << " ]" << std::endl;

    syslog(LOG_INFO, "openrouter administration software started.");
    this->srv.listen(ip, port, socket_flags);
    return 0;
}
