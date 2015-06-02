#ifndef _WHEELBIASED_H_
#define _WHEELBIASED_H_

#include "IWinnerChoice.h"

/**
 * Roulette biais�
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
    * Ins�re un nouveau fitness
    * Si le m�me fitness est d�j� pr�sent il �crase la valeur
    */
    void pushChance(const FitnessChancePair&);

private:
    std::vector<FitnessChancePair> m_chanceList;

    /**
     * Retourne la valuer de chance du fitness le plus proche du fitness pass� en param�tre
     * le fitness retourn� sera toujours inf�rieur ou �gal au fitness pass� en param�tre
     */
    int getChanceByFitnessOrGreater(unsigned int fitness) const;
};

#endif //_WHEELBIASED_H_