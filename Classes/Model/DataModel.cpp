#include "Model\DataModel.h"

USING_NS_CC;

DataModel* DataModel::m_pInstance;

DataModel::DataModel(){
}

DataModel::~DataModel(){
}

DataModel* DataModel::getModel(){
	if (m_pInstance == NULL)
		m_pInstance = new DataModel();
	return m_pInstance;
}