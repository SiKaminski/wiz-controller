#pragma once
#ifndef _GLOBALS_HPP_
#define _GLOBALS_HPP_

#include "wiz.hpp"
#include "skutils/logger/logger.hpp"

namespace Global
{
    constexpr int WIZ_UDP_BROADCAST_PORT {38899};

    inline Wiz::Controller* wizController = nullptr;
    inline Logger logger;
}

#endif // _GLOBALS_HPP_
