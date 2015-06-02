#include "IAlgoGen.h"

IAlgoGen::IAlgoGen(const cinder::Surface& image)
    : m_computeImage(image)
{}

cinder::Surface IAlgoGen::getComputeImage() const
{
    return m_computeImage;
}

void IAlgoGen::setComputeImage(const cinder::Surface& image)
{
    m_computeImage = image;
}