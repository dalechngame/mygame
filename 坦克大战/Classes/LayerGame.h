#ifndef __LayerGame_H__
#define __LayerGame_H__

#include "Common.h"

class Item;
class AI;
class TankFriend;

class LayerGame : public CCLayer
{
public:
	enum FAILURE_REASON { TIMEOUT, HOMEDESTROY, TANKDIE };
	static void gameOver(FAILURE_REASON reason);

	static LayerGame* create(unsigned int index);
	bool init(unsigned int index);
	void onExit();

	// Control
	void update(float);
	void TimeOut(float);
	void DirCtrlPress(CCObject*);
	void Shoot(CCObject*);
	void Setup(CCObject*);

	void addItem();
	void eatItem(Item*);
	void eatItemBomb();
	void eatItemSteel();

	void setPause();

	void setGodMode(int interval)
	{
		_godMode = true;
		scheduleOnce(schedule_selector(LayerGame::unsetGodMode), interval);
	}
	void unsetGodMode(float dt)
	{
		_godMode = false;
	}
	// model
	CCTMXTiledMap*	_map;
	TankFriend*			_tankFriend;
	bool						_godMode;

	CCArray*				_bullets;
	CCArray*				_items;
	AI*						_ai;

	int						_victoryCount;  // 胜利条件
	int						_curCount;      // 当前得分
	int						_index;

	static int				_life;
	static int				_score;
	CCLabelTTF*			_labelScore;
};

#endif
