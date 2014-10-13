#include "ResourceModel.h"
#include "PFComponent.h"

PFComponent::PFComponent(){
}

PFComponent::~PFComponent(){
}

bool PFComponent::init(){
	if (!Sprite::init())
		return false;
	return true;
}