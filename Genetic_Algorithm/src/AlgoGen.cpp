#include "AlgoGen.h"

AlgoGen::AlgoGen(const cinder::Surface& image)
    : m_computeImage(image)
{}

cinder::Surface AlgoGen::getComputeImage() const
{
    return m_computeImage;
}

void AlgoGen::setComputeImage(const cinder::Surface& image)
{
    m_computeImage = image;
}

std::vector<Sticky> AlgoGen::operator()(const std::vector<Sticky>& stickies)
{
    std::vector<Sticky> nextGen;

    for (auto& sticky : stickies)
    {
        
    }

    return nextGen;
}