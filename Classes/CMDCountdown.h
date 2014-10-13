#pragma once
#include "cocos2d.h"
#include "CMD.h"

USING_NS_CC;

class CMDCountdown :public CMD{

public:
	CMDCountdown();
	~CMDCountdown();

	void init();
	void execute();
};
