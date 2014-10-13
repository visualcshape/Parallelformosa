#pragma once
#include "cocos2d.h"
#include "Command.h"

USING_NS_CC;

class CMDAnimation :public Command{

public:
	CMDAnimation();
	~CMDAnimation();

	void init();
	void execute();
};
