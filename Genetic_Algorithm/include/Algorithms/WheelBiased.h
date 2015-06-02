#ifndef _WHEELBIASED_H_
#define _WHEELBIASED_H_

#include "IWinnerChoice.h"

/**
 * Roulette biaisé
 */
class WheelBiased : public IWinnerChoice
{
public:
    struct FitnessChancePair
    {
        FitnessChancePair(int _fitness = 0, int _percentChange = 0)
            : fitness(_fitness),
            percentChance(_percentChange)
        {}

        /** Fitness cible*/
        unsigned int fitness;
        /** Pourcentage de change de tirer au sort */
        unsigned int percentChance;
    };

    virtual int operator()(const std::vector<FitnessStickyContainer>& stickyList) const;

    const std::vector<FitnessChancePair>& getChanceList() const;
    std::vector<FitnessChancePair>& getChanceList();

    const FitnessChancePair& getChanceByFitness(unsigned int fitness) const;
    FitnessChancePair& getChanceByFitness(unsigned int fitness);

    /**
    * Insère un nouveau fitness
    * Si le même fitness est déjà présent il écrase la valeur
    */
    void pushChance(const FitnessChancePair&);

private:
    std::vector<FitnessChancePair> m_chanceList;

    /**
     * Retourne la valuer de chance du fitness le plus proche du fitness passé en paramètre
     * le fitness retourné sera toujours inférieur ou égal au fitness passé en paramètre
     */
    int getChanceByFitnessOrGreater(unsigned int fitness) const;
};

#endif //_WHEELBIASED_H_