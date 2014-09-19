#include "../Model/DataModel.h"

USING_NS_CC;

DataModel* DataModel::dm_pInstance;

DataModel::DataModel(){
}

DataModel::~DataModel(){
}

DataModel* DataModel::getModel(){
	if (dm_pInstance == NULL)
		dm_pInstance = new DataModel();
	return dm_pInstance;
}