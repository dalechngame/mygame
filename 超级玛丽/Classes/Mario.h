#ifndef __Mario_H__
#define __Mario_H__

#include "Common.h"
#include "Item.h"
class LayerGame;

class Mario : public CCSprite
{
public:
	CREATE_FUNC(Mario);
	bool init();

	Common::DIRECTION _dir;
	Common::DIRECTION _faceDir;
	enum WEAPON{ NONE, BIG, FIRE } _grow;
	enum { NORMAL, FLY, DEAD, CHANGSTA, SHOOT }_status;

	//! 移动控制函数
	void moveUp(float dt);
	void moveDown(float dt);
	void moveLeft(float dt);
	void moveRight(float dt);
	void stop();
	void jump();
	void fire();
	void die(bool dead = true);
	void updateStatus();

private:
	//! 障碍物判断
	bool canMoveDown(float dt);
	bool canMoveUp(float dt);
	bool canMoveLeft(float dt);
	bool canMoveRight(float dt);

	//! 碰到砖块
	void eatCoin(CCTMXLayer *layer, const CCPoint &ptTile);
	void hit(CCTMXLayer* layer, int gid, CCPoint ptTile);

	void dieFuncCall();
	void stopActionFuncCall();
	void changeStaFuncCall(){
		_status = NORMAL;
		setFrame("walk", _faceDir, _grow);
		updateStatus();
	}
	void hasRewardFuncCall(CCNode*);
	bool _hasReward(CCNode*);

	void _changeScore(const int &score, LayerGame *game, const CCPoint &birthPoint);
	void moveMap(float);
	void setFrame(const char *frameName, Common::DIRECTION dir, WEAPON w, bool isAni = true);
	void setAction(const char *actionName, Common::DIRECTION dir, WEAPON w, bool forever = true);

public:
	void eatMushroom(Item::ItemType type);

	void autoRunDown(){
		_bAutoRun = true;
		_speedUp = 0;
		_speedDown = 10;

		updateStatus();
		schedule(schedule_selector(Mario::moveDown));
	}

	void autoRunRight() {
		_bAutoRun = false;
		schedule(schedule_selector(Mario::moveRight));
	}

	void unsetPlayAudio(float)
	{
		this->_playAudio = false;
	}

	void setPlayAudio(int time)
	{
		this->_playAudio = true;
		scheduleOnce(schedule_selector(Mario::unsetPlayAudio), time);
	}

	void unsetGodMode(float)
	{
		this->_bGodMode = false;
	}

	void setGodMode(int time)
	{
		this->_bGodMode = true;
		scheduleOnce(schedule_selector(Mario::unsetGodMode), time);
	}

	CCTMXTiledMap* getMap()
	{
		return (CCTMXTiledMap*)getParent();
	}

	static int _life;		// 命的数量
	int _speedUp;	// 向上运动的速度，如果速度不为0，说明mario要继续上升
	int _speedDown; // 向下的速度
	int _speedAcc;  // 重力加速度
	int _speed;     // 水平移动的速度
	bool _bflagRunAction;	//flagpoint是否在运动
	bool _playAudio; //正在播放声音
	bool _onLadder; // 是否在阶梯上
	bool _bGodMode; //无敌模式
	bool _bAutoRun; //自动模式

};

#endif

