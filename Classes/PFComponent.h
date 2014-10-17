#pragma once
#include "cocos2d.h"
#include "AppMacro.h"
#include "CMD.h"

USING_NS_CC;

class PFComponent : public Sprite {
public:
	PFComponent();
	~PFComponent();

	CREATE_FUNC(PFComponent);
	virtual bool init();

	void AddHp(int value);
	void AddCMDState(CMD* cmdState);
	bool isDead();

	bool tryToAttack();
	bool canAtttack(PFComponent* target);

	CC_SYNTHESIZE(int, _id, ID);
	CC_SYNTHESIZE(int, _oid, OID); //object ID

    CC_SYNTHESIZE(std::string, _owner, Owner); //uid
	CC_SYNTHESIZE(int, _z, Z);
	CC_SYNTHESIZE(PII, _occupy, Occupy);
	CC_SYNTHESIZE(Sprite*, _sprite, Sprite);

	CC_SYNTHESIZE(int, _atk, Atk);
	CC_SYNTHESIZE(int, _def, Def);
	CC_SYNTHESIZE(int, _hp, Hp);

	CC_SYNTHESIZE(Point, _coord, Coord);
	//CC_SYNTHESIZE(Point, screenPosition, ScreenPosition);

protected:
	Vector <CMD*> _states;
};
