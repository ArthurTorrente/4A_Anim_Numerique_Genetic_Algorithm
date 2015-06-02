#ifndef _WINNERISBEST_H_
#define _WINNERISBEST_H_

#include "IWinnerChoice.h"

class WinnerIsBest : public IWinnerChoice
{
public:
    virtual int operator()(const std::vector<FitnessStickyContainer>& stickyList) const;
};

#endif //_WINNERISBEST_H_