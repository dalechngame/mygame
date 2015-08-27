
#ifndef __AI_H__
#define __AI_H__

#include "Common.h"

class AI : public CCNode
{
public:
	CREATE_FUNC(AI);
	bool init();
	void onExit();

	CCTMXTiledMap* getMap()
	{
		return (CCTMXTiledMap*)getParent();
	}

	bool _createFlag;
	bool	_pause;

	CCArray* _tanks;
	CCArray* _bullets;

	// 产生敌军坦克
	void createTank(float);
	// 移动敌军坦克
	void moveTank(float);
	// 让敌军坦克发射子弹
	void Shoot(float);
	// 碰撞检测
	void update(float);

	void resume(float);
	void pause();
};

#endif

