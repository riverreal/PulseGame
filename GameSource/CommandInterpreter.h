#pragma once
class CommandInterpreter
{
public:
	static CommandInterpreter& GetInstance();
	~CommandInterpreter();

	CommandInterpreter(CommandInterpreter const&) = delete;
	void operator= (CommandInterpreter const&) = delete;
	
	bool isRightNotes(int playerNum);
	bool isCenterNotes(int playerNum);
	bool isLeftNotes(int playerNum);
	bool isStrafeLeft(int playerNum);
	bool isStrafeRight(int playerNum);

	void setRightNotes(int playerNum,bool setFlag);
	void setCenterNotes(int playerNum, bool setFlag);
	void setLeftNotes(int playerNum, bool setFlag);
	void setStrafeLeft(int playerNum, bool setFlag);
	void setStrafeRight(int playerNum, bool setFlag);

private:
	CommandInterpreter() {};

	struct CommandFlags
	{
		bool nRightFlag = false;
		bool nCenterFlag = false;
		bool nLeftFlag = false;

		bool sRightFlag = false;
		bool sLeftFlag = false;

		bool humanFlag = false;
	};

	CommandFlags flagArray[2];

};