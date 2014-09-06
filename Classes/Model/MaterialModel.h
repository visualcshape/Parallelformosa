#pragma once
#include "cocos2d.h"

USING_NS_CC;

class MaterialModel{
public:
	MaterialModel();
	~MaterialModel();

	static MaterialModel* getModel();
	Sprite* picOrangeGem;

protected:
	static MaterialModel* m_pInstance;
};
