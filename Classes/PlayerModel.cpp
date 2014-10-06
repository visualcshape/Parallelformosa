#include "PlayerModel.h"

USING_NS_CC;

PlayerModel* PlayerModel::pm_pInstance;

PlayerModel::PlayerModel(){
	height = -1;
}

PlayerModel::~PlayerModel(){
}

PlayerModel* PlayerModel::getModel(){
	if (pm_pInstance == nullptr)
		pm_pInstance = new PlayerModel();
	return pm_pInstance;
}