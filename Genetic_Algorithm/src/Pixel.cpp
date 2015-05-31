#include "Pixel.h"

Pixel::Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    m_rgba.m_8U[0] = r;
    m_rgba.m_8U[1] = g;
    m_rgba.m_8U[2] = b;
    m_rgba.m_8U[3] = a;
}

Pixel::~Pixel()
{}

unsigned char& Pixel::getR()
{
    return m_rgba.m_8U[0];
}

unsigned char& Pixel::getG()
{
    return m_rgba.m_8U[1];
}

unsigned char& Pixel::getB()
{
    return m_rgba.m_8U[2];
}

unsigned char& Pixel::getA()
{
    return m_rgba.m_8U[3];
}

int& Pixel::getRGBA()
{
    return m_rgba.m_32U;
}

const unsigned char& Pixel::getR() const
{
    return m_rgba.m_8U[0];
}

const unsigned char& Pixel::getG() const
{
    return m_rgba.m_8U[1];
}

const unsigned char& Pixel::getB() const
{
    return m_rgba.m_8U[2];
}

const unsigned char& Pixel::getA() const
{
    return m_rgba.m_8U[3];
}

const int& Pixel::getRGBA() const
{
    return m_rgba.m_32U;
}