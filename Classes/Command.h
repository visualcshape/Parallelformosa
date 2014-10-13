#pragma once
#include "cocos2d.h"

USING_NS_CC;

class Command :public Ref{

public:
	Command();
	~Command();

	virtual void execute() = 0;
};
