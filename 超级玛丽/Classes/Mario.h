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

	//! �ƶ����ƺ���
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
	//! �ϰ����ж�
	bool canMoveDown(float dt);
	bool canMoveUp(float dt);
	bool canMoveLeft(float dt);
	bool canMoveRight(float dt);

	//! ����ש��
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

	static int _life;		// ��������
	int _speedUp;	// �����˶����ٶȣ�����ٶȲ�Ϊ0��˵��marioҪ��������
	int _speedDown; // ���µ��ٶ�
	int _speedAcc;  // �������ٶ�
	int _speed;     // ˮƽ�ƶ����ٶ�
	bool _bflagRunAction;	//flagpoint�Ƿ����˶�
	bool _playAudio; //���ڲ�������
	bool _onLadder; // �Ƿ��ڽ�����
	bool _bGodMode; //�޵�ģʽ
	bool _bAutoRun; //�Զ�ģʽ

};

#endif

