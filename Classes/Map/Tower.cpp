#include "Tower.h"
#include "DataModel.h"

Tower* MachineGunTower::tower(){
	Tower* tower = Tower::create();
	tower->sprite = Sprite::create("MachineGunTurret.png");
	tower->addChild(tower->sprite, 0);
	tower->range = 200;
	return tower;
}

bool MachineGunTower::init(){
	if (!Sprite::init()) 
		return false;
	return true;
}
