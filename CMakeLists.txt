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

    src/settings/ftp/ftp.cpp

    src/reboot/reboot.cpp

    src/syslog/syslog.cpp
)  
