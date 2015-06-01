#ifndef _PIXEL_H_
#define _PIXEL_H_

#include "tools.h"

class Pixel
{
public:
    Pixel(unsigned char r = 0, unsigned char b = 0, unsigned char g = 0, unsigned char a = 0);
    ~Pixel();

    unsigned char& getR();
    unsigned char& getG();
    unsigned char& getB();
    unsigned char& getA();
    int& getRGBA();

    const unsigned char& getR() const;
    const unsigned char& getG() const;
    const unsigned char& getB() const;
    const unsigned char& getA() const;
    const int& getRGBA() const;

private:    
    rgbaColor m_rgba;
};

#endif //_PIXEL_H_