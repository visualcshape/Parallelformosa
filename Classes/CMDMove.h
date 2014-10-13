#pragma once
#include "cocos2d.h"
#include "Command.h"

USING_NS_CC;

class CMDMove :public Command{

public:
	CMDMove();
	~CMDMove();

	void init();
	void execute();
};
