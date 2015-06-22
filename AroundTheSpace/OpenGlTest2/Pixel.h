#ifndef _PIXEL_H_
#define _PIXEL_H_

#include "cinder/Color.h"

class Pixel
{
public:
    Pixel();
    Pixel(int x, int y, const cinder::ColorA& color = cinder::ColorA::black());

    int getX() const;
    int getY() const;
    const cinder::ColorA& getColor() const;

    void setX(int x);
    void setY(int y);
    void setColor(const cinder::ColorA& color);

    bool operator==(const Pixel& p) const;
protected:
    int m_x;
    int m_y;
    cinder::ColorA m_color;
};

#endif //_PIXEL_H_