#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <algorithm>

namespace tools
{
    template<typename T>
    T clamp(T value, T min, T max)
    {
        return std::min(max, std::max(value, min));
    }
}

#endif //_TOOLS_H_