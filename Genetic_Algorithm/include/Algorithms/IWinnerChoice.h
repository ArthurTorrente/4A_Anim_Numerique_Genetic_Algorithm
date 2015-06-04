#ifndef _IWINNERCHOICE_H
#define _IWINNERCHOICE_H

#include <vector>

#include "FitnessStickyContainer.h"

/**
 * Classe mère des algorithme de choix du vainqueur d'une population
 */
class IWinnerChoice
{
public:
    /**
     * Méthode de tirage au sort du vainqueur d'une liste de sticky
     */
    virtual int operator()(const std::vector<FitnessStickyContainer>& stickyList) const = 0;
};

#endif //_IWINNERCHOICE_H