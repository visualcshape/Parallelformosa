#pragma once
#include "cocos2d.h"

USING_NS_CC;

class BaseLayer : public Layer{
public:
	BaseLayer();
	~BaseLayer();
	virtual bool init();

	CREATE_FUNC(BaseLayer);
};
