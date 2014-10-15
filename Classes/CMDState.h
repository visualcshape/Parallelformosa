#pragma once
#include "cocos2d.h"
#include "CMD.h"
#include "PFComponent.h"

USING_NS_CC;

class CMDState :public CMD{

public:
	CMDState();
	~CMDState();

	static CMDState* order(int timing, PFComponent* pfComponent, int adjustHp);

	void init();
	virtual bool execute();

private:
	PFComponent* _pfComponent;
	int _adjustHp;
};
