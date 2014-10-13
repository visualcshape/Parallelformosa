#include "CommandModel.h"
#include "Command.h"

USING_NS_CC;

CommandModel* CommandModel::cm_pInstance;

CommandModel::CommandModel(){
	_commands = queue <CMD*>();
	while (!_commands.empty())
		_commands.pop();
}

CommandModel::~CommandModel(){
}

CommandModel* CommandModel::getModel(){
	if (cm_pInstance == nullptr)
		cm_pInstance = new CommandModel();
	return cm_pInstance;
}

void CommandModel::AddCommand(CMD* cmd){
	if (cm_pInstance == nullptr) return;
	CCLOG("CommandModel::AddCommand()");
	_commands.push(cmd);
}

void CommandModel::execute(float dt){
	if (cm_pInstance == nullptr) return;
	CC_ASSERT(cm_pInstance != nullptr);
	CCLOG("CommandModel::execute() = %d\n", SZ(_commands));
	if (!_commands.empty()){
		CMD* cmd = _commands.front();
		cmd->execute();
		delete cmd; 
		_commands.pop();
	}
}
