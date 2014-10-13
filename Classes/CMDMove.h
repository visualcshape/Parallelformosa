#pragma once
#include "cocos2d.h"
#include "CMD.h"

USING_NS_CC;

class CMDMove :public CMD{

public:
	CMDMove();
	~CMDMove();

	void init();
	void execute();
};
