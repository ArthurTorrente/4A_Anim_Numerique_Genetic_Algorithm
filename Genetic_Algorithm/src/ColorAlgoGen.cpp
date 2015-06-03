#include "ColorAlgoGen.h"

ColorAlgoGen::ColorAlgoGen(const cinder::Surface& computeImage)
    : IAlgoGen(computeImage)
{}

std::vector<Sticky> ColorAlgoGen::operator()(const std::vector<Sticky>& oldGenSticky) const
{
    std::vector<Sticky> nextGenStickies;

    if (!m_computeImage)
        return nextGenStickies;

    for (auto& currentSticky : oldGenSticky)
    {

    }

    return nextGenStickies;
}

static unsigned int getNumberOfSameBit(float a, float b)
{
    bool aBit(false), bBit(false);

    unsigned int sameCount = 0;
    unsigned int bitCount = sizeof(float) * 8;
    
    unsigned int uA = static_cast<unsigned int>(a);
    unsigned int uB = static_cast<unsigned int>(b);
    
    for (unsigned int i = 0; i < bitCount; ++i)
    {
        unsigned int bitIte = 1 << i;

        aBit = uA & bitIte;
        bBit = uB & bitIte;

        if (aBit == bBit)
            ++sameCount;
    }

    return sameCount;
}

unsigned int ColorAlgoGen::getFitness(const Sticky& s, const cinder::ColorA& c)
{
    unsigned int fitness = 0;
    const cinder::ColorA& sColor(s.getColor());

    fitness += getNumberOfSameBit(sColor.r, c.r);
    fitness += getNumberOfSameBit(sColor.g, c.g);
    fitness += getNumberOfSameBit(sColor.b, c.b);
    fitness += getNumberOfSameBit(sColor.a, c.a);

    return fitness;
}