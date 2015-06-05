#include "ColorAlgoGen.h"

#include <array>

#include "tools.h"
#include "Algorithms\WinnerIsBest.h"

ColorAlgoGen::ColorAlgoGen()
{
    m_interval.push_back(Interval(COPY, 20));
    m_interval.push_back(Interval(COMBINAISON, 65));
    m_interval.push_back(Interval(MUTATE, 5));
    m_interval.push_back(Interval(RANDOM, 10));
}

static bool haveSimilarPixel(const std::vector<Stixel>& s)
{
    for (unsigned int i = 0; i < s.size(); ++i)
    {
        for (unsigned int j = 0; j < s.size(); ++j)
        {
            if (i != j)
            {
                if (s[i].pixel == s[j].pixel)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

IAlgoGen::StixelsWrapper ColorAlgoGen::operator()(const std::vector<Stixel>& oldGenSticky) const
{
    if (oldGenSticky.size() == 0)
        return StixelsWrapper(oldGenSticky, 0);

    /* Associe un sticky à un fitness */
    std::vector<FitnessStickyContainer> fitnessSticky;
    /* Tableau qui sera utilisé pour stocker la nouvelle generation de stickies */
    StixelsWrapper newGen;

    fitnessSticky.reserve(oldGenSticky.size());
    newGen.stixel.reserve(oldGenSticky.size());

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

    auto itSticky = fitnessSticky.begin();
    auto ENDSTICKY = fitnessSticky.end() - 1;
    auto beginBest = fitnessSticky.begin();

    /* Keep best */
    int keepBest = static_cast<int>(getPercent(COPY) * fitnessPopSize);
    
    if (keepBest > 0)
    {
        std::transform(itSticky, itSticky + keepBest, std::back_inserter(newGen.stixel), [&beginBest](const FitnessStickyContainer& f)
        {
            Sticky newStick(f.stixel->sticky);
            newStick.ChangeColor(beginBest->stixel->sticky.getColor());

            Stixel newStixel(newStick, f.stixel->pixel);

            ++beginBest;

            return newStixel;
        });

        itSticky += keepBest;
        beginBest = fitnessSticky.begin();
    }

    /* ==================== */

    /* Mutation */
    int mutateRatio = static_cast<int>(getPercent(MUTATE) * fitnessPopSize);
    mutateRatio = tools::clamp<int>(mutateRatio, 0, fitnessSticky.size() - keepBest);

    if (mutateRatio > 0)
    {
        std::transform(itSticky, itSticky + mutateRatio, std::back_inserter(newGen.stixel), [&beginBest](const FitnessStickyContainer& f)
        {
            Sticky newStick(f.stixel->sticky);
            newStick.ChangeColor(beginBest->stixel->sticky.mutate().getColor());

            Stixel newStixel(newStick, f.stixel->pixel);
            ++beginBest;

            return newStixel;
        });

        itSticky += mutateRatio;
        beginBest = fitnessSticky.begin();
    }
    /* ============================= */

    /* Combinaison */
    int combineRatio = static_cast<int>(getPercent(COMBINAISON) * fitnessPopSize);
    combineRatio = tools::clamp<int>(combineRatio, 0, fitnessSticky.size() - keepBest - mutateRatio);

    if (combineRatio > 0)
    {
        std::transform(itSticky, itSticky + combineRatio, std::back_inserter(newGen.stixel), [&newGen, &keepBest, &beginBest](const FitnessStickyContainer& f)
        {
            Sticky newStick(f.stixel->sticky);
            newStick.ChangeColor((beginBest->stixel->sticky * newGen.stixel[RANDOMIZER.nextUint(keepBest - 1)].sticky).getColor());

            Stixel newStixel(newStick, f.stixel->pixel);
            ++beginBest;

            return newStixel;
        });

        itSticky += combineRatio;
        beginBest = fitnessSticky.begin();
    }
    /* ============================= */

    /* Combinaison */
    int randomRatio = static_cast<int>(getPercent(RANDOM) * fitnessPopSize);
    randomRatio = tools::clamp<int>(randomRatio, 0, fitnessSticky.size() - keepBest - mutateRatio - combineRatio);

    if (randomRatio > 0)
    {
        std::transform(itSticky, itSticky + randomRatio, std::back_inserter(newGen.stixel), [&beginBest](const FitnessStickyContainer& f)
        {
            Sticky newStick(f.stixel->sticky);
            newStick.ChangeColor(beginBest->stixel->sticky.random().getColor());

            Stixel newStixel(newStick, f.stixel->pixel);
            ++beginBest;

            return newStixel;
        });

        itSticky += randomRatio;
        beginBest = fitnessSticky.begin();
    }
    /* ============================= */

    /* Fill with */
    int fillRatio = tools::clamp<int>(fitnessSticky.size() - keepBest - mutateRatio - combineRatio - randomRatio, 0, fitnessSticky.size());

    if (fillRatio > 0)
    {
        std::transform(itSticky, itSticky + fillRatio, std::back_inserter(newGen.stixel), [&beginBest](const FitnessStickyContainer& f)
        {
            IntervalType randChoice = static_cast<IntervalType>(RANDOMIZER.nextUint(4));

            switch (randChoice)
            {
            case COPY:
            {
                Sticky newStick(f.stixel->sticky);
                newStick.ChangeColor(beginBest->stixel->sticky.getColor());

                Stixel newStixel(newStick, f.stixel->pixel);
                ++beginBest;

                return newStixel;
            }
            break;

            case MUTATE:
            {
                Sticky newStick(f.stixel->sticky);
                newStick.ChangeColor(beginBest->stixel->sticky.mutate().getColor());

                Stixel newStixel(newStick, f.stixel->pixel);
                ++beginBest;

                return newStixel;
            }
            break;

            case COMBINAISON:
            {
                Sticky newStick(f.stixel->sticky);
                newStick.ChangeColor((beginBest->stixel->sticky * beginBest->stixel->sticky.random()).getColor());

                Stixel newStixel(newStick, f.stixel->pixel);
                ++beginBest;

                return newStixel;
            }
            break;

            case RANDOM:
            default:
            {
                Sticky newStick(f.stixel->sticky);
                newStick.ChangeColor(beginBest->stixel->sticky.random().getColor());

                Stixel newStixel(newStick, f.stixel->pixel);
                ++beginBest;

                return newStixel;
            }
            }
        });
    }
    /* ============================= */

    /* Calcul du fitness de la nouvelle population */
    std::for_each(newGen.stixel.begin(), newGen.stixel.end(), [&newGen, this](const Stixel& s)
    {
        newGen.fitness += getFitness(s.sticky.getColor(), s.pixel.getColor());
    });
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