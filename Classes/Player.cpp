#include "ResourceModel.h"
#include "Player.h"

Player::Player(){
}

Player::~Player(){
}

void Player::init(int uid){
	_uid = uid;
	L_str = G_mag = 3000;
	food = 30;
}


bool Player::canAddTroop(int TID){
	ResourceModel *rm = ResourceModel::getModel();
	return L_str >= rm->costLstr[TID] &&
		G_mag >= rm->costGmag[TID] &&
		food >= rm->costFood[TID];
}

void Player::consumeResource(int TID){
	ResourceModel *rm = ResourceModel::getModel();

	L_str -= rm->costLstr[TID];
	G_mag -= rm->costGmag[TID];;
	food -= rm->costFood[TID];;
}
