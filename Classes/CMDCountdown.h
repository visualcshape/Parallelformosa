#pragma once
#include "cocos2d.h"
#include "Command.h"

USING_NS_CC;

class CMDCountdown :public Command{

public:
	CMDCountdown();
	~CMDCountdown();

	void init();
	void execute();
};
