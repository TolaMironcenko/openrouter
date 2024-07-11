#ifndef QRTOSVG_HPP
#define QRTOSVG_HPP

#include <string>
#include "qrcodegen.hpp"

std::string toSvgString(const qrcodegen::QrCode &qr, int border);

#endif // RTOSVG_HPP