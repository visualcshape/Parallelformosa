#pragma once
#include "cocos2d.h"
#include "Command.h"

USING_NS_CC;

class CMDState :public Command{

public:
	CMDState();
	~CMDState();

	void init();
	void execute();
};
