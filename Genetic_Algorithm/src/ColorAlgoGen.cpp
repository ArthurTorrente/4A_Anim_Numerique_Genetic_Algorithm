#include "ColorAlgoGen.h"

#include <array>
#include "cinder\Rand.h"

#include "Algorithms\WinnerIsBest.h"

ColorAlgoGen::ColorAlgoGen(const cinder::Surface& computeImage)
    : IAlgoGen(computeImage),
    m_numberOfChild(3)
{}

std::vector<Stixel> ColorAlgoGen::operator()(const std::vector<Stixel>& oldGenSticky) const
{
    std::vector<Stixel> nextGenStickies;

    if (!m_computeImage)
        return nextGenStickies;

    std::vector<FitnessStickyContainer> newSticky(3);
    WinnerIsBest winnerAlgo;
    Stixel winner;
    cinder::Rand randomizer(static_cast<unsigned int>(time(nullptr)));

    for (auto it = oldGenSticky.begin(); it != oldGenSticky.end(); ++it)
    {
        for (unsigned int i = 0; i < 3; ++i)
        {
            unsigned int randomChoice = randomizer.nextUint(2);

            switch (randomChoice)
            {
            case 0:
                newSticky[i].sticky = (*it).sticky.mutate();
                break;

            case 1:
                newSticky[i].sticky = (*it).sticky * oldGenSticky[randomizer.nextUint(oldGenSticky.size() - 1)].sticky;
                break;

            case 2:
                newSticky[i].sticky = (*it).sticky.random();
                break;
            }

            newSticky[i].fitness = getFitness(newSticky[i].sticky, (*it).pixel.getColor());
        }

        winner.sticky = newSticky[winnerAlgo(newSticky)].sticky;
        winner.pixel = (*it).pixel;
        
        nextGenStickies.push_back(winner);
    }

    return nextGenStickies;
}

unsigned int& ColorAlgoGen::getNumberOfGenerateChild()
{
    return m_numberOfChild;
}

unsigned int ColorAlgoGen::getNumberOfGenerateChild() const
{
    return m_numberOfChild;
}

void ColorAlgoGen::setNumberOfGenerateChild(unsigned int n)
{
    m_numberOfChild = n;
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

        aBit = (uA & bitIte) == bitIte;
        bBit = (uB & bitIte) == bitIte;

        if (aBit == bBit)
            ++sameCount;
    }

    return sameCount;
}

unsigned int ColorAlgoGen::getFitness(const Sticky& s, const cinder::ColorA& c) const
{
    unsigned int fitness = 0;
    const cinder::ColorA& sColor(s.getColor());

    fitness += getNumberOfSameBit(sColor.r, c.r);
    fitness += getNumberOfSameBit(sColor.g, c.g);
    fitness += getNumberOfSameBit(sColor.b, c.b);
    fitness += getNumberOfSameBit(sColor.a, c.a);

    return fitness;
}