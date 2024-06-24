cmake_minimum_required (VERSION 2.6)  
project(openrouter)  
set(CMAKE_CXX_STANDARD 17)  
  
include_directories(./src/libs/)  
    
add_executable(  
    openrouter   
    src/openrouter.cpp 
    src/security/cors.cpp
)  
