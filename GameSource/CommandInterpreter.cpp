#include "CommandInterpreter.h"

CommandInterpreter& CommandInterpreter::GetInstance()
{
	static CommandInterpreter instance;
	return instance;
}
CommandInterpreter::~CommandInterpreter()
{}

//get
bool CommandInterpreter::isRightNotes(int playerNum)
{
	return flagArray[playerNum].nRightFlag;
}

bool CommandInterpreter::isCenterNotes(int playerNum)
{
	return flagArray[playerNum].nCenterFlag;
}

bool CommandInterpreter::isLeftNotes(int playerNum)
{
	return flagArray[playerNum].nLeftFlag;
}

bool CommandInterpreter::isStrafeLeft(int playerNum)
{
	return flagArray[playerNum].sLeftFlag;
}

bool CommandInterpreter::isStrafeRight(int playerNum)
{
	return flagArray[playerNum].sRightFlag;
}
//set
void CommandInterpreter::setRightNotes(int playerNum, bool setFlag)
{
	flagArray[playerNum].nRightFlag = setFlag;
}
void CommandInterpreter::setCenterNotes(int playerNum, bool setFlag)
{
	flagArray[playerNum].nCenterFlag = setFlag;
}
void CommandInterpreter::setLeftNotes(int playerNum, bool setFlag)
{
	flagArray[playerNum].nLeftFlag = setFlag;
}
void CommandInterpreter::setStrafeLeft(int playerNum, bool setFlag)
{
	flagArray[playerNum].sLeftFlag = setFlag;
}
void CommandInterpreter::setStrafeRight(int playerNum, bool setFlag)
{
	flagArray[playerNum].sRightFlag = setFlag;
}


