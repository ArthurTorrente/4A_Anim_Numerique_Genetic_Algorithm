#ifndef _WHEELBIASED_H_
#define _WHEELBIASED_H_

#include "IWinnerChoice.h"

/**
 * Roulette biaisé
 */
class WheelBiased : public IWinnerChoice
{
public:
    WheelBiased();

    int operator()(const std::vector<FitnessStickyContainer>& stickyList) const;
private:
};

#endif //_WHEELBIASED_H_