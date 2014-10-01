#include "ResourceModel.h"
#include "PFComponent.h"

bool PFComponent::init(){
	if (!Sprite::init())
		return false;
	return true;
}