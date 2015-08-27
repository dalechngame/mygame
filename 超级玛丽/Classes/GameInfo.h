
#ifndef __GameInfo_H__
#define __GameInfo_H__
#include "Common.h"

#define MUSHROOM_SCORE 100
#define TORTOISE_SCORE 200
#define COIN_SCORE 200
#define REWARD_SCORE 500
#define FLOWER_SCORE 1000

class GameInfo : public CCLayer
{
public:
	CREATE_FUNC(GameInfo);
	bool init();
	
	static void setScore(int score)
	{
		UScore += score;
		GameInfo::score->setString(CCString::createWithFormat("%d", UScore)->getCString());
	}
	static void setCoinNum(int num)
	{
		UCoin += num;
		GameInfo::coinNum->setString(CCString::createWithFormat("%d", UCoin)->getCString());
	}
	static void setLevel(int level)
	{
		ULevel += level;
		GameInfo::level->setString(CCString::createWithFormat("%d", ULevel)->getCString());
	}
	static void setTime(const char * ptime)
	{
		time->setString(ptime);
	}

	void scheduleTime(float) ;
	void disaCall();

	static CCLabelBMFont * score;
	static CCLabelBMFont * coinNum;
	static CCLabelBMFont * level;
	static CCLabelBMFont * time;

	CCLabelBMFont* markX;
	CCLabelTTF* scoreName;
	CCLabelTTF* levelName;
	CCLabelTTF* timeName;

	CCSprite *blinkCoin;
	static CCLabelBMFont *monsterScore;

	static int Utime;
	static int UScore;
	static int UCoin;
	static int ULevel;
};

#endif