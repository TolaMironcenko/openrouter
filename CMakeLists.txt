cmake_minimum_required (VERSION 2.6)  
project(openrouter)  
set(CMAKE_CXX_STANDARD 20)  
  
include_directories(./src/libs/)  
    
add_executable(  
    openrouter   
    src/openrouter.cpp 
    src/security/cors.cpp
    src/settings/telnet/telnet.cpp
    src/reboot/reboot.cpp
)  
