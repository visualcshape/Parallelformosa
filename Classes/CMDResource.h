#pragma once
#include "cocos2d.h"
#include "CMD.h"
#include "PlayerModel.h"

USING_NS_CC;

class CMDResource :public CMD{

public:
	CMDResource();
	~CMDResource();

	static CMDResource* order(PlayerModel* player, int Lstr, int Gmag, int food);

	virtual void execute();

private:
	PlayerModel* _player;
	int _Lstr;
	int _Gmag;
	int _food;
};
