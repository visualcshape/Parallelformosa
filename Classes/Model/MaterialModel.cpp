#include "Model\MaterialModel.h"

USING_NS_CC;

MaterialModel* MaterialModel::m_pInstance;

MaterialModel::MaterialModel(){
	picOrangeGem = Sprite::create("Range.png");
}

MaterialModel::~MaterialModel(){
}

MaterialModel* MaterialModel::getModel(){
	if (m_pInstance == NULL)
		m_pInstance = new MaterialModel();
	return m_pInstance;
}