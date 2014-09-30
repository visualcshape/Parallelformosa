//
//  MainUIInfoModel.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/1.
//
//

#include "MainUIInfoModel.h"

MainUIInfoModel* MainUIInfoModel::_instance = nullptr;

MainUIInfoModel::MainUIInfoModel(){
    _scorllingLabelText = "";
    _changedData = MainUIInfoModel::ChangedData::NONE;
}

MainUIInfoModel::~MainUIInfoModel(){
    if(_instance!=nullptr){
        delete _instance;
        _instance = nullptr;
    }
}

MainUIInfoModel* MainUIInfoModel::getInstance(){
    if(_instance==nullptr){
        _instance = new MainUIInfoModel();
        return _instance;
    }
    return _instance;
}

string MainUIInfoModel::getScrollingText(){
    return _scorllingLabelText;
}

void MainUIInfoModel::setScrollingText(string text){
    _scorllingLabelText = text;
    _changedData = SCROLLING_TEXT;
    Notify();
}

MainUIInfoModel::ChangedData MainUIInfoModel::getChagedData(){
    return _changedData;
}