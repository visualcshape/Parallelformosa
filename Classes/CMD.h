#pragma once
#include "cocos2d.h"

USING_NS_CC;

class CMD :public Ref{

public:
	CMD();
	~CMD();

	virtual bool execute() = 0;

protected:
	int _timing;
};
