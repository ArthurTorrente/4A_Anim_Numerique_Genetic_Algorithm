#include "ColorAlgoGen.h"

#include <array>
#include "cinder\Rand.h"

#include "Algorithms\WinnerIsBest.h"

ColorAlgoGen::ColorAlgoGen()
    : m_numberOfChild(3)
{
    m_interval.push_back(Interval(COPY, 30));
    m_interval.push_back(Interval(COMBINAISON, 30));
    m_interval.push_back(Interval(MUTATE, 20));
    m_interval.push_back(Interval(RANDOM, 10));
}

std::vector<Stixel> ColorAlgoGen::operator()(const std::vector<Stixel>& oldGenSticky) const
{
    std::vector<Stixel> nextGenStickies;

    std::vector<FitnessStickyContainer> newSticky(3);
    WinnerIsBest winnerAlgo;
    Stixel winner;
    cinder::Rand randomizer(static_cast<unsigned int>(time(nullptr)));

    auto intervalList(m_interval);

    std::sort(intervalList.begin(), intervalList.end(), [](const Interval& i1, const Interval& i2)
    {
        return i2.percent < i1.percent;
    });

    for (auto it = oldGenSticky.begin(); it != oldGenSticky.end(); ++it)
    {
        for (unsigned int i = 0; i < 3; ++i)
        {
            unsigned int randomChoice = randomizer.nextUint(100);

            IntervalType action = COPY;

            for (auto& interval : intervalList)
            {
                if (interval.percent >= randomChoice)
                {
                    action = interval.type;
                    break;
                }
            }

            switch (action)
            {
            case MUTATE:
                newSticky[i].sticky = it->sticky.mutate();
                break;

            case COMBINAISON:
            {
                //Remplacer par voisin
                newSticky[i].sticky = it->sticky * oldGenSticky[randomizer.nextUint(oldGenSticky.size() - 1)].sticky;
            }
                break;

            case RANDOM:
                newSticky[i].sticky = it->sticky.random();
                break;

            case COPY:
                newSticky[i].sticky = it->sticky;
                break;
            }

            newSticky[i].fitness = getFitness(newSticky[i].sticky, (*it).pixel.getColor());
        }

        winner.sticky = newSticky[winnerAlgo(newSticky)].sticky;
        winner.pixel = it->pixel;
        
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

unsigned char& ColorAlgoGen::getInterval(ColorAlgoGen::IntervalType type)
{
    auto found = std::find_if(m_interval.begin(), m_interval.end(), [type](const Interval& interval)
    {
        if (interval.type == type)
            return true;

        return false;
    });

    if (found == m_interval.end())
        throw std::invalid_argument("Type not found");

    return found->percent;
}

unsigned char ColorAlgoGen::getInterval(ColorAlgoGen::IntervalType type) const
{
    auto found = std::find_if(m_interval.begin(), m_interval.end(), [type](const Interval& interval)
    {
        if (interval.type == type)
            return true;

        return false;
    });

    if (found == m_interval.end())
        throw std::invalid_argument("Type not found");

    return found->percent;
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

void ColorAlgoGen::mutatePopulation(std::vector<Stixel>::const_iterator start, std::vector<Stixel>::const_iterator end, std::vector<Stixel>& nextPopulation) const
{
    for (; start != end; ++start)
    {
        nextPopulation.push_back(
            Stixel(start->sticky.mutate(),
            start->pixel)
        );
    }
}

void ColorAlgoGen::combinePopulation(std::vector<Stixel>::const_iterator start, std::vector<Stixel>::const_iterator end, const std::vector<Stixel>& combinePopulation, std::vector<Stixel>& nextPopulation) const
{
    cinder::Rand randomizer(static_cast<unsigned int>(time(nullptr)));

    for (; start != end; ++start)
    {
        nextPopulation.push_back(
            Stixel(start->sticky * combinePopulation[randomizer.nextUint(combinePopulation.size() - 1)].sticky,
            start->pixel)
            );
    }
}

void ColorAlgoGen::radomPopulation(std::vector<Stixel>::const_iterator start, std::vector<Stixel>::const_iterator end, std::vector<Stixel>&nextPopulation) const
{
    for (; start != end; ++start)
    {
        nextPopulation.push_back(
            Stixel(start->sticky.random(),
            start->pixel)
            );
    }
}

void ColorAlgoGen::copyPopulation(std::vector<Stixel>::const_iterator start, std::vector<Stixel>::const_iterator end, std::vector<Stixel>& nextPopulation) const
{
    for (; start != end; ++start)
    {
        nextPopulation.push_back(
            Stixel(start->sticky,
            start->pixel)
            );
    }
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