#pragma once
#include "cocos2d.h"

USING_NS_CC;

class MapModel{
public:
	MapModel();
	~MapModel();

	static MapModel* getModel();

protected:
	static MapModel * mm_pInstance;
};
