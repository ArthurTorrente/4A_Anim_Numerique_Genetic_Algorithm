#include "ColorAlgoGen.h"

ColorAlgoGen::ColorAlgoGen(const cinder::Surface& computeImage)
    : IAlgoGen(computeImage)
{}

std::vector<Sticky> ColorAlgoGen::operator()(const std::vector<Sticky>& oldGenSticky) const
{
    std::vector<Sticky> nextGenStickies;

    for (auto& currentSticky : oldGenSticky)
    {

    }

    return nextGenStickies;
}