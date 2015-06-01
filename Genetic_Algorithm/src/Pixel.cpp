#include "Pixel.h"

Pixel::Pixel(int x, int y, const cinder::ColorA& color)
    : m_x(x),
    m_y(y),
    m_color(color)
{}

int Pixel::getX() const
{
    return m_x;
}

int Pixel::getY() const
{
    return m_y;
}

const cinder::ColorA& Pixel::getColor() const
{
    return m_color;
}

void Pixel::setX(int x)
{
    m_x = x;
}

void Pixel::setY(int y)
{
    m_y = y;
}

void Pixel::setColor(const cinder::ColorA& color)
{
    m_color = color;
}