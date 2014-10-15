#include "command.h"
#include "ResourceModel.h"

USING_NS_CC;


CMDFileStream* CMDFileStream::cmdFS_pInstance;

CMDFileStream::CMDFileStream(){
	init();
}

CMDFileStream::~CMDFileStream(){
}

CMDFileStream* CMDFileStream::getInstance(){
	if (cmdFS_pInstance == nullptr)
		cmdFS_pInstance = new CMDFileStream();
	return cmdFS_pInstance;
}

void CMDFileStream::init(){
	_fp = nullptr;
	_text = "";
}

void CMDFileStream::OpenStream(string relativePath){
	if (_fp != nullptr)
		closeStream();
	_fp = ResourceModel::getModel()->OpenFileW(relativePath);
	_text = "";
}

void CMDFileStream::closeStream(){
	if(_fp != nullptr)
		fclose(_fp);
	init();
}

bool CMDFileStream::isStreamOn(){
	return _fp != nullptr;
}

void CMDFileStream::append(string str){
	CCASSERT(_fp != nullptr, "CMDFileStream didn't open stream");
	_text += str;
}

void CMDFileStream::execute(){
	if (_fp == nullptr) return;
	CCLOG("CMDFileStream::execute()");
	fprintf(_fp, _text.c_str());
	closeStream();
}
