#pragma once
#include "cocos2d.h"
#include "CMD.h"

USING_NS_CC;

class CMDAnimation :public CMD{

public:
	CMDAnimation();
	~CMDAnimation();

	void init();
	virtual bool execute();
};
