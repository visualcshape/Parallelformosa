#include "PlayerManager.h"

USING_NS_CC;

PlayerManager* PlayerManager::pmr_pInstance;

PlayerManager::PlayerManager(){
	_atkPlayer = _defPlayer = _curPlayer = nullptr;
}

PlayerManager::~PlayerManager(){
}

PlayerManager* PlayerManager::getInstance(){
	if (pmr_pInstance == nullptr)
		pmr_pInstance = new PlayerManager();
	return pmr_pInstance;
}
