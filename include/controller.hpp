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
            std::vector<Bulb::Device*> FilterDevicesByModulePrefix(std::string prefix);
            std::vector<Bulb::Device*> FilterDevicesByHomeID(std::string homeId);
            std::vector<Bulb::Device*> FilterDevicesByRoomID(std::string roomId);

            void ToggleLights();
            void SetColor(Color color);
            void SetBrightness(ushort brightness);

            // Update the state of bulbs, return the new state across all bulbs
            // Bulb::State UpdateAllBulbStates(const Color* color, const ushort* brightness);

        private:
            void InitBulbs();

            // Return bulb json response
            std::vector<json_t*> SearchForBulbs();

            std::vector<Bulb::Device*>  mBulbs; 
            std::vector<json_t*>        mSearchResponses;
            std::string                 mDevicePrefix;
            std::string                 mHomeId; 
            std::string                 mRoomId; 
    };
}

#endif // _WIZ_CONTROLLER_HPP_
