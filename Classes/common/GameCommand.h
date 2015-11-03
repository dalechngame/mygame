// Created by yuanzhongji on 2013/07

#ifndef __GAME_COMMAND_H__
#define __GAME_COMMAND_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "Common.h"
#include "PBase.h"
#include "TipDialog.h"
#include "vector"
#include "ShowDiyDialog.h"

USING_NS_CC;

//typedef std::function<void ()> ExcuteGameCommand;

#define CREATE_COMMAND_FUNC(__TYPE__)					\
	static __TYPE__* create()	\
{													\
	__TYPE__ *command = new __TYPE__(); \
	if (command && command->init())		\
	{									\
		command->autorelease();		\
		return command;		\
	}	\
	CC_SAFE_DELETE(command);		\
	return NULL;		\
}		\


// 游戏的返回执行命令
class GameCommand : public Ref
{
protected:
	PBase *m_base;

	GameCommand *m_nextCommand;
	// 当前命令的编号
	CC_SYNTHESIZE(int, m_commandIndex, CommandIndex);
	// 完成之后的回调
	virtual void commandCompletedCallBack();

public:

	GameCommand();
	// 初始化
	virtual bool init();
	// 当前命令的下一个命令
	void setNextCommand(GameCommand *command);
	// 执行命令
	virtual void execute(PBase *base);  // overright me
	// 执行当前命令的下一个命令
	void gotoNextCommand(PBase *base);
};

// 主场景里面保存星星
class GameStarCommand : public GameCommand
{
public:
	// 当前的游戏编号
	GameNumber gameNumber;
	// 当前的星星的数量
	CC_SYNTHESIZE(ProgressStar, m_starNum, StarNum);

public:
	GameStarCommand();

	CREATE_COMMAND_FUNC(GameStarCommand);
	// 初始化
	virtual bool init();
	// 执行命令
	virtual void execute(PBase *base)  override;
};

// 完成了宫殿的游戏出现送衣服的弹窗
class GameSugarShowCommand : public GameCommand
{
public:

	Vector<__Integer*> sugarVector;

public:

	CREATE_COMMAND_FUNC(GameSugarShowCommand);

	virtual void execute(PBase *base) override;
};

class GameShowDiyCommand : public GameCommand
{
public:
	// 当前保存的diy的key
	CC_SYNTHESIZE(string, m_diySaveKey, DiySaveKey);

	void onDiyDialogClosed(TipDialog *dialog);

public:

	GameShowDiyCommand();

	CREATE_COMMAND_FUNC(GameShowDiyCommand);

	virtual void execute(PBase *base) override;

};

#endif // __GAME_COMMAND_H__