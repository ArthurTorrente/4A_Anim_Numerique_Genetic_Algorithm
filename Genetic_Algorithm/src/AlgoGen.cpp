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
    std::vector<Sticky> nextGen(stickies.size());

    for (std::vector<Sticky>::size_type i = 0; i < stickies.size(); ++i)
    {
        const Sticky& sticky = stickies[i];
    }

    return nextGen;
}