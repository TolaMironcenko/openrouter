cmake_minimum_required (VERSION 2.6)  
project(openrouter)  
set(CMAKE_CXX_STANDARD 20)  
  
include_directories(./src/libs/)  
    
add_executable(  
    openrouter   
    src/openrouter.cpp 
    src/security/cors.cpp
    
    src/settings/telnet/enabled.cpp
    src/settings/telnet/port.cpp

    src/settings/ssh/status.cpp
    src/settings/ssh/keys.cpp
    src/settings/ssh/port.cpp

    src/settings/dns/enabled.cpp
    src/settings/dns/servers.cpp
    src/settings/dns/local_domains.cpp

    src/settings/wifi/status.cpp
    src/settings/wifi/name.cpp
    src/settings/wifi/password.cpp
    src/settings/wifi/qr.cpp
    src/settings/wifi/channel.cpp

    src/settings/ftp/ftp.cpp

    src/system/reboot/reboot.cpp

    src/system/syslog/syslog.cpp
    src/system/uptime/uptime.cpp

    src/OpenRouter/OpenRouter.cpp

    src/auth/auth.cpp

    src/libs/QR-Code-generator/qrcodegen.cpp
    src/libs/QR-Code-generator/qrtosvg.cpp
)  
