#include"stdafx.h"

#include "IAlgoGen.h"

#include "Constants.h"

IAlgoGen::IAlgoGen()
    : m_popSize(100)
{
    m_interval.reserve(4);

    m_interval.push_back(Interval(COPY, 30));
    m_interval.push_back(Interval(CROSSOVER, 60));
    m_interval.push_back(Interval(MUTATE, 3));
    m_interval.push_back(Interval(RANDOM, 8));
}

IAlgoGen::~IAlgoGen()
{}

unsigned char& IAlgoGen::getInterval(IntervalType type)
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

unsigned char IAlgoGen::getInterval(IntervalType type) const
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

float IAlgoGen::getPercent(IntervalType type) const
{
    try
    {
        unsigned char interval = getInterval(type);
        float r = static_cast<float>(interval) / 100.0f;
        return r;
    }
    catch (std::exception&)
    {
        return 0.0f;
    }
}

unsigned int& IAlgoGen::getPopSize()
{
    return m_popSize;
}

unsigned int IAlgoGen::getPopSize() const
{
    return m_popSize;
}

const std::vector<IAlgoGen::SurfaceWrapper>& IAlgoGen::getPop() const
{
    return m_population;
}

std::vector<IAlgoGen::SurfaceWrapper>& IAlgoGen::getPop()
{
    return m_population;
}

void IAlgoGen::setPopSize(unsigned int popSize)
{
    m_popSize = popSize;
}

bool IAlgoGen::setup(unsigned int width, unsigned int height)
{
    m_population.clear();
    m_population.reserve(m_popSize);

    for (unsigned int i = 0; i < m_popSize; ++i)
    {
        m_population.push_back(
            SurfaceWrapper(getRandomSurface(width, height), 0)
            );
    }

    return true;
}

uint64_t IAlgoGen::getFitness(const cinder::Surface& realImage, const cinder::Surface& popImage) const
{
    uint64_t fitness = 0;
    auto realIter = realImage.getIter();
    auto popIter = popImage.getIter();

    while (realIter.line() && popIter.line())
    {
        while (realIter.pixel() && popIter.pixel())
        {
            fitness += std::abs(popIter.r() - realIter.r());
            fitness += std::abs(popIter.g() - realIter.g());
            fitness += std::abs(popIter.b() - realIter.b());
        }
    }

    return fitness;
}

cinder::Surface IAlgoGen::getRandomSurface(unsigned int width, unsigned int height) const
{
    cinder::Surface randomSurface(width, height, true, cinder::SurfaceChannelOrder::RGBA);

    auto iter = randomSurface.getIter();

    while (iter.line())
    {
        while (iter.pixel())
        {
            iter.r() = static_cast<uint8_t>(RANDOMIZER.nextUint(255));
            iter.g() = static_cast<uint8_t>(RANDOMIZER.nextUint(255));
            iter.b() = static_cast<uint8_t>(RANDOMIZER.nextUint(255));
            iter.a() = 255;
        }
    }

    return randomSurface;
}