#pragma once
#ifndef _WIZ_CONTROLLER_HPP_
#define _WIZ_CONTROLLER_HPP_

#include "bulb.hpp"
#include "types.hpp"

#include <string>
#include <vector>
#include <jansson.h>

namespace Wiz
{
    enum CMD {

    };

    class Controller
    {
        public:
            Controller();
            ~Controller();

            void ConfirmBulbChoices();
            std::vector<Bulb*> FilterDevicesByModulePrefix(std::string prefix);
            std::vector<Bulb*> FilterDevicesByHomeID(std::string homeId);
            std::vector<Bulb*> FilterDevicesByRoomID(std::string roomId);

            void ToggleLights();
            void SetColor(Color color);
        private:
            void InitBulbs();

            // Return bulb json response
            std::vector<json_t*> SearchForBulbs();

            std::vector<Bulb*>      mBulbs; 
            std::vector<json_t*>    mSearchResponses;
            std::string             mDevicePrefix;
            std::string             mHomeId; 
            std::string             mRoomId; 
    };
}

#endif // _WIZ_CONTROLLER_HPP_
