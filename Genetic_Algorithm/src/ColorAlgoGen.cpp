#include "ColorAlgoGen.h"

#include <array>

#include "tools.h"

ColorAlgoGen::ColorAlgoGen()
{}

cinder::Surface ColorAlgoGen::operator()(const cinder::Surface& realImage)
{
    std::vector<SurfaceWrapper> nextGenPopulation;
    nextGenPopulation.reserve(m_popSize);

    /* Calcul du fitness */
    std::for_each(m_population.begin(), m_population.end(), [this, &realImage](SurfaceWrapper& s)
    {
        s.fitness = getFitness(realImage, s.image);
    });

    std::sort(m_population.begin(), m_population.end(), [](const SurfaceWrapper& s1, const SurfaceWrapper& s2)
    {
        return s1.fitness < s2.fitness;
    });
    /* ============================ */

    /* Copy algo */
    unsigned int copyRatio = tools::clamp<int>(
        static_cast<int>(getPercent(COPY) * m_popSize),
        0,
        m_popSize
        );

    if (copyRatio > 0)
    {
        std::transform(m_population.begin(), m_population.begin() + copyRatio, std::back_inserter(nextGenPopulation), [&realImage, this](const SurfaceWrapper& sw)
        {
            return sw;
        });
    }

    /* ============================ */

    /* Mutate algo */
    unsigned int mutateRatio = tools::clamp<int>(
        static_cast<int>(getPercent(MUTATE) * m_popSize),
        0,
        m_popSize - copyRatio
        );

    if (mutateRatio > 0)
    {
        std::transform(m_population.begin(), m_population.begin() + mutateRatio, std::back_inserter(nextGenPopulation), [&realImage, this](const SurfaceWrapper& sw)
        {
            cinder::Surface s = mutate(sw.image);
            
            return SurfaceWrapper(s, getFitness(realImage, s));
        });
    }
    /* ============================ */

    /* CrossOver algo */
    unsigned int crossOverRatio = tools::clamp<int>(
        static_cast<int>(getPercent(CROSSOVER) * m_popSize),
        0,
        m_popSize - copyRatio - mutateRatio
        );

    if (crossOverRatio > 0)
    {
        std::transform(m_population.begin(), m_population.begin() + crossOverRatio, std::back_inserter(nextGenPopulation), [&realImage, this](const SurfaceWrapper& sw)
        {
            auto& pop = getPop();

            cinder::Surface s = crossOver(sw.image, pop[RANDOMIZER.nextUint(pop.size())].image);

            return SurfaceWrapper(s, getFitness(realImage, s));
        });
    }
    /* ============================ */

    /* Random Algo */
    unsigned int randomRatio = tools::clamp<int>(
        static_cast<int>(getPercent(RANDOM) * m_popSize),
        0,
        m_popSize - copyRatio - mutateRatio - crossOverRatio
        );

    if (nextGenPopulation.size() + randomRatio < m_popSize)
    {
        randomRatio = m_popSize - nextGenPopulation.size();
    }

    if (randomRatio > 0)
    {
        std::transform(m_population.begin(), m_population.begin() + randomRatio, std::back_inserter(nextGenPopulation), [&realImage, this](const SurfaceWrapper& sw)
        {
            cinder::Surface s = getRandomSurface(sw.image.getWidth(), sw.image.getHeight());

            return SurfaceWrapper(s, getFitness(realImage, s));
        });
    }
    /* ============================ */

    /* Sort new pop */
    std::sort(nextGenPopulation.begin(), nextGenPopulation.end(), [](const SurfaceWrapper& s1, const SurfaceWrapper& s2)
    {
        return s1.fitness < s2.fitness;
    });
    /* ============================ */

    m_population = nextGenPopulation;

    return m_population.front().image;
}

cinder::Surface ColorAlgoGen::mutate(const cinder::Surface& s) const
{
    cinder::Surface mutateS = s.clone();
    auto mSize = mutateS.getSize();
    
    cinder::Vec2i pixelChoose(RANDOMIZER.nextUint(mSize.x), RANDOMIZER.nextUint(mSize.y));
    mutateS.setPixel(pixelChoose, cinder::ColorA8u(static_cast<uint8_t>(RANDOMIZER.nextUint(255)),
                                                   static_cast<uint8_t>(RANDOMIZER.nextUint(255)),
                                                   static_cast<uint8_t>(RANDOMIZER.nextUint(255)),
                                                   255
                                                   ));

    return mutateS;
}

cinder::Surface ColorAlgoGen::crossOver(const cinder::Surface& s1, const cinder::Surface& s2) const
{
    cinder::Surface crossOver = s1.clone();

    auto s1Iter = crossOver.getIter();
    auto s2Iter = s2.getIter();

    while (s1Iter.line() && s2Iter.line())
    {
        while (s1Iter.pixel() && s2Iter.pixel())
        {
            if(RANDOMIZER.nextBool())
            {
                s1Iter.r() = s2Iter.r();
                s1Iter.g() = s2Iter.g();
                s1Iter.b() = s2Iter.b();
            }
        }
    }

    return crossOver;
}