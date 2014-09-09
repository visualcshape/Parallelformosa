#include "Map\Building.h"
#include "Model\DataModel.h"

//@subclass OrangeGem
Building* OrangeGem::build(int level){
	Building* building = Building::create();
	CCASSERT(CCFileUtils::getInstance()->isFileExist("PlanetCute/Gem Orange.png"), "PlanetCute/Gem Orange.png doesn't exist");
	building->sprite = Sprite::create("PlanetCute/Gem Orange.png");
	building->addChild(building->sprite, 0);
	building->range = 200;
	building->height = level;
	return building;
}

bool OrangeGem::init(){
	if (!Sprite::init())
		return false;
	return true;
}

//@subclass BlueGem
Building* BlueGem::build(int level){
	Building* building = Building::create();
	CCASSERT(CCFileUtils::getInstance()->isFileExist("PlanetCute/Gem Blue.png"), "PlanetCute/Gem Blue.png doesn't exist");
	building->sprite = Sprite::create("PlanetCute/Gem Blue.png");
	building->addChild(building->sprite, 0);
	building->range = 200;
	return building;
}

bool BlueGem::init(){
	if (!Sprite::init())
		return false;
	return true;
}

//@subclass GreenGem
Building* GreenGem::build(int level){
	Building* building = Building::create();
	CCASSERT(CCFileUtils::getInstance()->isFileExist("PlanetCute/Gem Green.png"), "PlanetCute/Gem Green.png doesn't exist");
	building->sprite = Sprite::create("PlanetCute/Gem Green.png");
	building->addChild(building->sprite, 0);
	building->range = 200;
	return building;
}

bool GreenGem::init(){
	if (!Sprite::init())
		return false;
	return true;
}

//@subclass Star
Building* Star::build(int level){
	CCASSERT(CCFileUtils::getInstance()->isFileExist("PlanetCute/Star.png"), "PlanetCute/Star.png doesn't exist");
	Building* building = Building::create();
	building->sprite = Sprite::create("PlanetCute/Star.png");
	building->addChild(building->sprite, 0);
	building->range = 200;
	return building;
}

bool Star::init(){
	if (!Sprite::init())
		return false;
	return true;
}