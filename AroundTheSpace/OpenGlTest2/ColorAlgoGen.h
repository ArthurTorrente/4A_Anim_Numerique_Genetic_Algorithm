#ifndef _COLORALGOGEN_H_
#define _COLORALGOGEN_H_

#include "IAlgoGen.h"
#include "Constants.h"

class ColorAlgoGen : public IAlgoGen
{
public:
	ColorAlgoGen();

	virtual cinder::Surface operator()(const cinder::Surface&);

protected:
	cinder::Surface mutate(const cinder::Surface&) const;
	cinder::Surface crossOver(const cinder::Surface& s1, const cinder::Surface& s2) const;
};

#endif //_COLORALGOGEN_H_
