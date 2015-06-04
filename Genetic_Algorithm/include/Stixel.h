#ifndef _STIXEL_H_
#define _STIXEL_H_

#include "Sticky.h"
#include "Pixel.h"
struct Stixel
{
    Stixel()
    {}

    Stixel(const Sticky& s, const Pixel& p)
        : sticky(s),
        pixel(p)
    {}

    Stixel(const Stixel& s)
        : sticky(s.sticky),
        pixel(s.pixel)
    {}

    Sticky sticky;
    Pixel pixel;
};

#endif //_STIXEL_H_