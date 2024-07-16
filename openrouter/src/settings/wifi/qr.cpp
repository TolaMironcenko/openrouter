#include "wifi.hpp"
#include <colors.h>
#include <json.hpp>
#include "../../types.hpp"
#include <QR-Code-generator/qrcodegen.hpp>
#include <QR-Code-generator/qrtosvg.hpp>
#include "../../auth/auth.hpp"

namespace settings {
    void get_wifi_qr(const httplib::Request &request, httplib::Response &response) {
        std::cout << GREEN << request.path << RESET << "  " << request.method << std::endl;
        response.set_header("Access-Control-Allow-Origin", "*");
        if (request.body.empty()) {
            response.set_content(ACCESS_REQUIRED_STRING, JSON_TYPE);
            return;
        }

        nlohmann::json json_body = nlohmann::json::parse(request.body);
        if (json_body["token"] == nullptr) {
            response.set_content(ACCESS_REQUIRED_STRING, JSON_TYPE);
            return;
        }

        if (authenticate(json_body["token"])) {
            std::ifstream wifisettingsfile(WIFI_SETTINGS_PATH);
            std::string wifi_name;
            std::string wifi_password;
            std::string wifisettingsbuffer;
            while (std::getline(wifisettingsfile, wifisettingsbuffer)) {
                if (wifisettingsbuffer.find("ssid=") == 0) {
                    wifi_name = wifisettingsbuffer;
                    wifi_name.erase(0, 5);
                }
                if (wifisettingsbuffer.find("wpa_passphrase=") == 0) {
                    wifi_password = wifisettingsbuffer;
                    wifi_password.erase(0, 15);
                    break;
                }
            }
            wifisettingsfile.close();

            std::stringstream toqrstring;
            toqrstring << "WIFI:T:WPA;P:" << wifi_password << ";S:" << wifi_name << ";";

            qrcodegen::QrCode qr0  = qrcodegen::QrCode::encodeText(toqrstring.str().c_str(), qrcodegen::QrCode::Ecc::MEDIUM);
            std::string responsedata = toSvgString(qr0, 4);
            response.set_content(responsedata, SVG_TYPE);
            return;
        }
        response.set_content(R"({"access":"reject"})", JSON_TYPE);
    }
}
