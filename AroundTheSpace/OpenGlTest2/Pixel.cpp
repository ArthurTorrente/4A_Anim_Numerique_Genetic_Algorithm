#include "stdafx.h"
#include "Pixel.h"

Pixel::Pixel()
{}

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

bool Pixel::operator==(const Pixel& p) const
{
    if (m_x == p.m_x && m_y == p.m_y)
        return true;

    else return false;
}