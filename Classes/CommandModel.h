#pragma once
#include "cocos2d.h"
#include "AppMacro.h"
#include "CMD.h"

USING_NS_CC;

class CommandModel :public Ref{

public:
	CommandModel();
	~CommandModel();

	void AddCommand(CMD* cmd);
	static CommandModel* getModel();
	void execute(float dt);

protected:
	static CommandModel* cm_pInstance;

private:
	queue <CMD*> _commands;
};
