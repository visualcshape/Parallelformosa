#pragma once
#include "cocos2d.h"
#include "AppMacro.h"
#include "PFComponent.h"
#include "CMD.h"

USING_NS_CC;

class Troop : public PFComponent {
public:
	Troop();
	~Troop();

	CREATE_FUNC(Troop);
	virtual bool init();

	static Troop* addTroop(int TID);
	void go(int dir, int ohfs);

	void attackLogic();
	bool buildAttackPath();

protected:
	void goLeft();
	void goDown();
	void goRight();
	void goUp();

private:
	PFComponent* _target;
	Vector <CMD*> _moves;
};
