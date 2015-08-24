#ifndef __Item_H__
#define __Item_H__

#include "Common.h"
class Mario;

class Item : public CCSprite
{
public:
	enum ItemType
	{
		IT_mushroom, IT_tortoise, IT_flower, IT_RewardGrow, IT_RewardAddLife, IT_flagpoint,
		IT_finalpoint, IT_LadderLR, IT_LadderUD, IT_flyfish, IT_tortoiseRound, IT_tortoiseFly, IT_firestring,
		IT_boss, IT_bridgestartpos,IT_BULLET, IT_RewardFlower
	};

	CCTMXTiledMap* getMap()
	{
		return (CCTMXTiledMap*)getParent();
	}
	// 工厂接口
	static Item* create(CCDictionary* dict);
	bool init();

	void update(float dt);
	virtual void move(float dt){}
	virtual void collision(){}
	virtual void wakeup(){}

	void setPositionByProperty(CCDictionary* dict);
	void runAnimation(const char* name);
	bool isLeftInWindow();
	bool isFarAwayFromMario();

	bool canMoveLeft(float dt);
	bool canMoveRight(float dt);
	bool canMoveDown(float dt);

	void moveLeft(float dt);
	void moveRight(float dt);
	void moveDown(float dt);

	// 设置无敌
	void setGodMode(int time)
	{
		_bGodMode = true;
		scheduleOnce(schedule_selector(Item::unsetGodMode), time);
	}
	void unsetGodMode(float dt)
	{
		_bGodMode = false;
	}

	ItemType _type;
	Common::DIRECTION _dir;
	int _speed;
	int _speedDown;
	int _speedAcc;
	bool _bGodMode;

	Mario* _mario;
	static CCArray* _itemReward;
	static CCArray *_pbulletArray;
	static Item* _Flag;
	static Item* _Boss;
	static Item* _bridge;
};


#endif
