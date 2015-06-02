#include "Algorithms/WheelBiased.h"

int WheelBiased::operator()(const std::vector<FitnessStickyContainer>& stickyList) const
{
    if (m_chanceList.size() == 0)
        return -1;

    std::vector<FitnessStickyContainer>::size_type stickiesCount = stickyList.size();

    for (std::vector<FitnessStickyContainer>::size_type i = 0; stickiesCount; ++i)
    {
        
    }
    
    return 0;
}

const std::vector<WheelBiased::ChanceInterval>& WheelBiased::getChanceList() const
{
    return m_chanceList;
}

std::vector<WheelBiased::ChanceInterval>& WheelBiased::getChanceList()
{
    return m_chanceList;
}

void WheelBiased::pushInterval(const ChanceInterval& interval)
{
    
}

const WheelBiased::ChanceInterval& WheelBiased::getChanceByFitness(unsigned int fitness) const
{
    auto isFound = std::find_if(m_chanceList.begin(), m_chanceList.end(), [fitness](const ChanceInterval& chance)
    {
        
        return false;
    });

    if (isFound == m_chanceList.end())
        throw std::exception("Chance not found");

    return *isFound;
}

WheelBiased::ChanceInterval& WheelBiased::getChanceByFitness(unsigned int fitness)
{
    auto isFound = std::find_if(m_chanceList.begin(), m_chanceList.end(), [fitness](const ChanceInterval& chance)
    {
        if (fitness >= chance.min && fitness <= chance.max)
            return true;

        return false;
    });

    if (isFound == m_chanceList.end())
        throw std::exception("Chance not found");

    return *isFound;
}

int WheelBiased::getChanceByFitnessOrGreater(unsigned int fitness) const
{
    if (m_chanceList.size() == 0)
        return -1;

    return -1;
}