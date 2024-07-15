#ifndef WIFI_HPP
#define WIFI_HPP

#include <httplib.h>

#define WIFI_SETTINGS_PATH "/etc/hostapd/hostapd.conf"

namespace settings {
    void get_wifi_status(const httplib::Request &request, httplib::Response &response);
    void set_wifi_status(const httplib::Request &request, httplib::Response &response);

    void get_wifi_name(const httplib::Request &request, httplib::Response &response);
    void set_wifi_name(const httplib::Request &request, httplib::Response &response);

    void get_wifi_password(const httplib::Request &request, httplib::Response &response);
    void set_wifi_password(const httplib::Request &request, httplib::Response &response);

    void get_wifi_qr(const httplib::Request &request, httplib::Response &response);

    void get_wifi_channel(const httplib::Request &request, httplib::Response &response);
    void set_wifi_channel(const httplib::Request &request, httplib::Response &response);
}

#endif // WIFI_HPP