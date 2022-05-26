#pragma once
#include <string>

#include "mp5_string.h"
#include "mp5_value_type.h"

namespace mp5
{
    struct mp5_value
    {
        mp5_string value_name;
        mp5_value_type type;
        void* data = nullptr;
        int byteSize = 0;
    };
}