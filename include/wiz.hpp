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

            // Return bulb json response
            std::vector<json_t*> SearchForBulbs();
            std::vector<json_t*> SearchForBulbs(std::string devicePrefix);

            void ConfirmBulbChoices();
        private:
           Bulb* mBulbs; 
    };
}

#endif // _WIZ_HPP_
