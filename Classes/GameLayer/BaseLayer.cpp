#include "../GameLayer/BaseLayer.h"

USING_NS_CC;

BaseLayer::BaseLayer(){
}

BaseLayer::~BaseLayer(){
}

bool BaseLayer::init(){
	if (!Layer::init())
		return false;

	/*Size visibleSize = Director::getInstance()->getVisibleSize();
	Sprite *bg = Sprite::create("background.png");

	bg->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg, 0);*/
	
	return true;
}
