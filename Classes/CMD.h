#pragma once
#include "cocos2d.h"

USING_NS_CC;

class CMD :public Ref{

public:
	CMD();
	~CMD();

	virtual void execute() = 0;
};
