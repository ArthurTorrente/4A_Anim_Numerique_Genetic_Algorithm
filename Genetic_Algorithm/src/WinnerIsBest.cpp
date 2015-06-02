#include "Algorithms/WinnerIsBest.h"

int WinnerIsBest::operator()(const std::vector<FitnessStickyContainer>& stickyList) const
{
    int winnerIndex = 0;
    unsigned int bigestFitness = stickyList.front().fitness;
    std::vector<FitnessStickyContainer>::size_type stickiesCount = stickyList.size();
    
    for (std::vector<FitnessStickyContainer>::size_type i = 1; i < stickiesCount; ++i)
    {
        if (stickyList[i].fitness > bigestFitness)
        {
            winnerIndex = i;
            bigestFitness = stickyList[i].fitness;
        }
    }

    return winnerIndex;
}