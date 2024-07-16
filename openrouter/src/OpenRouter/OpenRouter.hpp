#ifndef OPENROUTER_HPP
#define OPENROUTER_HPP

#include <iostream>
#include <httplib.h>

#define CORS_ENABLE

class OpenRouter {
    private:
        httplib::Server srv;

    public:
        OpenRouter();
        ~OpenRouter();
        int serve(const std::string &ip, int port, int socket_flags = 0);
};

#endif // OPENROUTER_HPP