#ifndef _WHEELBIASED_H_
#define _WHEELBIASED_H_

#include "IWinnerChoice.h"

/**
 * Roulette biaisé
 */
class WheelBiased : public IWinnerChoice
{
public:
    struct ChanceInterval
    {
        unsigned int min;
        unsigned int max;
        unsigned int percent;
    };

    virtual int operator()(const std::vector<FitnessStickyContainer>& stickyList) const;

    const std::vector<ChanceInterval>& getChanceList() const;
    std::vector<ChanceInterval>& getChanceList();

    const ChanceInterval& getChanceByFitness(unsigned int fitness) const;
    ChanceInterval& getChanceByFitness(unsigned int fitness);

    /**
    * Insère un nouveau fitness
    * Si le même fitness est déjà présent il écrase la valeur
    */
    void pushInterval(const ChanceInterval&);

private:
    std::vector<ChanceInterval> m_chanceList;

    /**
     * Retourne la valuer de chance du fitness le plus proche du fitness passé en paramètre
     * le fitness retourné sera toujours inférieur ou égal au fitness passé en paramètre
     */
    int getChanceByFitnessOrGreater(unsigned int fitness) const;
};

#endif //_WHEELBIASED_H_