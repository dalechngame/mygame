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


// ��Ϸ�ķ���ִ������
class GameCommand : public Ref
{
protected:
	PBase *m_base;

	GameCommand *m_nextCommand;
	// ��ǰ����ı��
	CC_SYNTHESIZE(int, m_commandIndex, CommandIndex);
	// ���֮��Ļص�
	virtual void commandCompletedCallBack();

public:

	GameCommand();
	// ��ʼ��
	virtual bool init();
	// ��ǰ�������һ������
	void setNextCommand(GameCommand *command);
	// ִ������
	virtual void execute(PBase *base);  // overright me
	// ִ�е�ǰ�������һ������
	void gotoNextCommand(PBase *base);
};

// ���������汣������
class GameStarCommand : public GameCommand
{
public:
	// ��ǰ����Ϸ���
	GameNumber gameNumber;
	// ��ǰ�����ǵ�����
	CC_SYNTHESIZE(ProgressStar, m_starNum, StarNum);

public:
	GameStarCommand();

	CREATE_COMMAND_FUNC(GameStarCommand);
	// ��ʼ��
	virtual bool init();
	// ִ������
	virtual void execute(PBase *base)  override;
};

// ����˹������Ϸ�������·��ĵ���
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
	// ��ǰ�����diy��key
	CC_SYNTHESIZE(string, m_diySaveKey, DiySaveKey);

	void onDiyDialogClosed(TipDialog *dialog);

public:

	GameShowDiyCommand();

	CREATE_COMMAND_FUNC(GameShowDiyCommand);

	virtual void execute(PBase *base) override;

};

#endif // __GAME_COMMAND_H__