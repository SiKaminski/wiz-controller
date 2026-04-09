#pragma once
#ifndef _TYPES_HPP_
#define _TYPES_HPP_

#include <sys/types.h>

struct Color {
    public:
        ushort R;
        ushort G;
        ushort B;

        bool Valid()
        {
            return R <= 255 && (G <= 255) && B <= 255;
        }
};

#endif // _TYPES_HPP_
