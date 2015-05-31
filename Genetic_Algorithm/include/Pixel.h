#ifndef _PIXEL_H_
#define _PIXEL_H_

class Pixel
{
public:
    Pixel(unsigned char r = 0, unsigned char b = 0, unsigned char g = 0, unsigned char a = 0);
    ~Pixel();

    unsigned char getR();
    unsigned char getG();
    unsigned char getB();
    unsigned char getA();
    int getRGBA();

    unsigned char& getR() const;
    unsigned char& getG() const;
    unsigned char& getB() const;
    unsigned char& getA() const;
    int& getRGBA() const;

private:
    unsigned char m_rgba[4];
};

#endif //_PIXEL_H_