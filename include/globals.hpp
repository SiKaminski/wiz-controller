#pragma once
#ifndef _GLOBALS_HPP_
#define _GLOBALS_HPP_

#include "controller.hpp"
#include "skutils/logger/logger.hpp"

namespace Global
{
    inline const char* ERR_BRIGHTNESS_OUT_OF_BOUNDS = "Brightness out of bounds";

    constexpr u_int16_t WIZ_UDP_BROADCAST_PORT {38899};

    inline Wiz::Controller* wizController = nullptr;
    inline Logger logger;
}

#endif // _GLOBALS_HPP_
