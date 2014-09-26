#include "BaseLayer.h"

USING_NS_CC;

BaseLayer::BaseLayer(){
}

BaseLayer::~BaseLayer(){
}

bool BaseLayer::init(){
	if (!Layer::init())
		return false;
	
	return true;
}
