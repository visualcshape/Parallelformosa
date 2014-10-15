#pragma once
#include "cocos2d.h"
#include "CMD.h"
#include "BattleModel.h"
#include "ResourceModel.h"

USING_NS_CC;

class CMDFileStream :public CMD{

public:
	CMDFileStream();
	~CMDFileStream();

	static CMDFileStream* getInstance();

	void OpenStream(string relativePath);
	void append(string str);

	void init();
	virtual void execute();
	bool isStreamOn();

protected:
	void closeStream();

private:
	static CMDFileStream* cmdFS_pInstance;
	FILE* _fp;
	string _text;
};
