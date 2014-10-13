#pragma once
#include "cocos2d.h"
#include "Command.h"

USING_NS_CC;

class CMDResource :public Command{

public:
	CMDResource();
	~CMDResource();

	void init();
	void execute();
};
