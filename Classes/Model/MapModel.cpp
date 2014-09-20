#include "../Model/MapModel.h"

USING_NS_CC;

MapModel* MapModel::mm_pInstance;

MapModel::MapModel(){
}

MapModel::~MapModel(){
}

MapModel* MapModel::getModel(){
	if (mm_pInstance == NULL)
		mm_pInstance = new MapModel();
	return mm_pInstance;
}