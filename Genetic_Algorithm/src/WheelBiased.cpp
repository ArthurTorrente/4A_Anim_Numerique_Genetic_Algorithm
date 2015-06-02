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

const std::vector<WheelBiased::FitnessChancePair>& WheelBiased::getChanceList() const
{
    return m_chanceList;
}

std::vector<WheelBiased::FitnessChancePair>& WheelBiased::getChanceList()
{
    return m_chanceList;
}

void WheelBiased::pushChance(const FitnessChancePair& chance)
{
    try
    {
        auto chanceFound = getChanceByFitness(chance.fitness);
        chanceFound.percentChance = chance.percentChance;
    }
    catch (std::exception&)
    {
        m_chanceList.push_back(chance);
    }
}

const WheelBiased::FitnessChancePair& WheelBiased::getChanceByFitness(unsigned int fitness) const
{
    auto isFound = std::find_if(m_chanceList.begin(), m_chanceList.end(), [fitness](const FitnessChancePair& chance)
    {
        if (chance.fitness == fitness)
            return true;

        return false;
    });

    if (isFound == m_chanceList.end())
        throw std::exception("Chance not found");

    return *isFound;
}

WheelBiased::FitnessChancePair& WheelBiased::getChanceByFitness(unsigned int fitness)
{
    auto isFound = std::find_if(m_chanceList.begin(), m_chanceList.end(), [fitness](const FitnessChancePair& chance)
    {
        if (chance.fitness == fitness)
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

    
}