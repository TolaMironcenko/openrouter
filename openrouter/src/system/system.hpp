#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#ifdef __linux__
#include "uptime/uptime.hpp"
#endif // __linux__
#include "syslog/syslog.hpp"
#include "reboot/reboot.hpp"

#endif // SYSTEM_HPP