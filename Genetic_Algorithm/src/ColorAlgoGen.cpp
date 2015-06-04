#include "ColorAlgoGen.h"

#include <array>
#include "cinder\Rand.h"

#include "tools.h"
#include "Algorithms\WinnerIsBest.h"

ColorAlgoGen::ColorAlgoGen()
{
    m_interval.push_back(Interval(COPY, 30));
    m_interval.push_back(Interval(COMBINAISON, 30));
    m_interval.push_back(Interval(MUTATE, 20));
    m_interval.push_back(Interval(RANDOM, 10));
}

std::vector<Stixel> ColorAlgoGen::operator()(const std::vector<Stixel>& oldGenSticky) const
{
    if (oldGenSticky.size() == 0)
        return oldGenSticky;

    cinder::Rand randomizer(static_cast<unsigned int>(time(nullptr)));

    /* Associe un sticky à un fitness */
    std::vector<FitnessStickyContainer> fitnessSticky;
    /* Tableau qui sera utilisé pour stocker la nouvelle generation de stickies */
    std::vector<Stixel> newGen;

    fitnessSticky.reserve(oldGenSticky.size());
    newGen.reserve(oldGenSticky.size());

    /* Calcul du fitness pour tous les stixel */
    std::for_each(oldGenSticky.begin(), oldGenSticky.end(), [&fitnessSticky, this](const Stixel& s)
    {
        fitnessSticky.push_back(FitnessStickyContainer(&s));
        FitnessStickyContainer& last(fitnessSticky.back());

        last.fitness = getFitness(last.stixel->sticky, last.stixel->pixel.getColor());
    });
    /* ============================= */

    /* Generation de la population suivante */
    std::sort(fitnessSticky.begin(), fitnessSticky.end());
    float fitnessPopSize = static_cast<float>(fitnessSticky.size());

    /* Keep best */
    int keepBest = static_cast<int>(getPercent(COPY) * fitnessPopSize);
    
    if (keepBest > 0)
    {
        std::transform(fitnessSticky.begin(), fitnessSticky.begin() + keepBest, std::back_inserter(newGen), [](const FitnessStickyContainer& f)
        {
            return Stixel(*(f.stixel));
        });
    }
    /* ==================== */

    /* Mutation */
    int mutateRatio = static_cast<int>(getPercent(MUTATE) * fitnessPopSize);
    mutateRatio = tools::clamp<int>(mutateRatio, 0, fitnessSticky.size() - keepBest);

    if (mutateRatio > 0)
    {
        std::transform(fitnessSticky.begin(), fitnessSticky.begin() + mutateRatio, std::back_inserter(newGen), [](const FitnessStickyContainer& f)
        {
            return Stixel(f.stixel->sticky.mutate(), f.stixel->pixel);
        });
    }
    /* ============================= */

    /* Combinaison */
    int combineRatio = static_cast<int>(getPercent(COMBINAISON) * fitnessPopSize);
    combineRatio = tools::clamp<int>(combineRatio, 0, fitnessSticky.size() - keepBest - mutateRatio);

    if (combineRatio > 0)
    {
        std::transform(fitnessSticky.begin(), fitnessSticky.begin() + combineRatio, std::back_inserter(newGen), [&newGen, &randomizer, &keepBest](const FitnessStickyContainer& f)
        {
            return Stixel(
                f.stixel->sticky * newGen[randomizer.nextUint(keepBest - 1)].sticky,
                f.stixel->pixel);
        });
    }
    /* ============================= */

    /* Combinaison */
    int randomRatio = static_cast<int>(getPercent(RANDOM) * fitnessPopSize);
    randomRatio = tools::clamp<int>(randomRatio, 0, fitnessSticky.size() - keepBest - mutateRatio - combineRatio);

    if (randomRatio > 0)
    {
        std::transform(fitnessSticky.begin(), fitnessSticky.begin() + randomRatio, std::back_inserter(newGen), [](const FitnessStickyContainer& f)
        {
            return Stixel(
                f.stixel->sticky.random(),
                f.stixel->pixel);
        });
    }
    /* ============================= */

    /* Fill with */
    int fillRatio = tools::clamp<int>(fitnessSticky.size() - keepBest - mutateRatio - combineRatio - randomRatio, 0, fitnessSticky.size());

    if (fillRatio > 0)
    {
        std::transform(fitnessSticky.begin() + keepBest, fitnessSticky.begin() + (keepBest + fillRatio), std::back_inserter(newGen), [&randomizer](const FitnessStickyContainer& f)
        {
            IntervalType randChoice = static_cast<IntervalType>(randomizer.nextUint(4));

            switch (randChoice)
            {
            case COPY:
                return Stixel(
                    f.stixel->sticky,
                    f.stixel->pixel);
                break;

            case MUTATE:
                return Stixel(
                    f.stixel->sticky.mutate(),
                    f.stixel->pixel);
                break;

            case COMBINAISON:
                return Stixel(
                    f.stixel->sticky * f.stixel->sticky.random(),
                    f.stixel->pixel);
                break;

            case RANDOM:
                return Stixel(
                    f.stixel->sticky.random(),
                    f.stixel->pixel);
                break;

            default:
                return Stixel(
                    f.stixel->sticky.random(),
                    f.stixel->pixel);
            }
        });
    }
    /* ============================= */

    return newGen;
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

float ColorAlgoGen::getPercent(IntervalType type) const
{
    try
    {
        return static_cast<float>(getInterval(type)) / 100.0f;
    }
    catch (std::exception&)
    {
        return 0.0f;
    }
}

static unsigned int getNumberOfSameBit(unsigned char a, unsigned char b)
{
    bool aBit(false), bBit(false);

    unsigned int sameCount = 0;
    unsigned int bitCount = sizeof(unsigned char) * 8;
    
    for (unsigned int i = 0; i < bitCount; ++i)
    {
        unsigned int bitIte = 1 << i;

        aBit = (a & bitIte) == bitIte;
        bBit = (b & bitIte) == bitIte;

        if (aBit == bBit)
            ++sameCount;
    }

    return sameCount;
}

unsigned int ColorAlgoGen::getFitness(const Sticky& s, const cinder::ColorA& c) const
{
    unsigned int fitness = 0;
    const cinder::ColorA8u sColor(s.getColor());
    const cinder::ColorA8u cColor(c);

    fitness += getNumberOfSameBit(sColor.r, cColor.r);
    fitness += getNumberOfSameBit(sColor.g, cColor.g);
    fitness += getNumberOfSameBit(sColor.b, cColor.b);
    fitness += getNumberOfSameBit(sColor.a, cColor.a);

    return fitness;
}