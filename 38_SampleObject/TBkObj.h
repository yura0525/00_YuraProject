#pragma once
#include "TObject.h"

class TBkObj : public TObject
{
public:
	bool Frame() { return true; }
public:
	TBkObj();
	~TBkObj();
};

