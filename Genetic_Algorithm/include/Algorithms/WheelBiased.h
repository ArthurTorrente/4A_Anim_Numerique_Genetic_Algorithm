#ifndef _WHEELBIASED_H_
#define _WHEELBIASED_H_

#include "IWinnerChoice.h"

/**
 * Roulette biais�
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
    * Ins�re un nouveau fitness
    * Si le m�me fitness est d�j� pr�sent il �crase la valeur
    */
    void pushInterval(const ChanceInterval&);

private:
    std::vector<ChanceInterval> m_chanceList;

    /**
     * Retourne la valuer de chance du fitness le plus proche du fitness pass� en param�tre
     * le fitness retourn� sera toujours inf�rieur ou �gal au fitness pass� en param�tre
     */
    int getChanceByFitnessOrGreater(unsigned int fitness) const;
};

#endif //_WHEELBIASED_H_