#pragma once
#ifndef _WIZ_HPP_
#define _WIZ_HPP_

#include "bulb.hpp"
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
            Controller(std::string devicePrefix);
            ~Controller();

            void ConfirmBulbChoices();
            void FilterDevicesByHomeID(std::string homeId);
            void FilterDevicesByRoomID(std::string roomId);
        private:
            // Return bulb json response
            std::vector<json_t*> SearchForBulbs();
            std::vector<json_t*> SearchForBulbs(std::string devicePrefix);

            std::vector<Bulb*> mBulbs; 
            std::vector<json_t*> mSearchResponses;
            std::string mHomeId; 
            std::string mRoomId; 
    };
}

#endif // _WIZ_HPP_
