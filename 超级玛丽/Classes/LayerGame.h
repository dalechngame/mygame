
#ifndef __LayerGame_H__
#define __LayerGame_H__
#include "Common.h"

class GameInfo;
class Mario;
class LayerLoadRes;

class LayerGame : public CCLayer
{
public:
	static LayerGame* create(int idx);
	bool init(int idx);

private:

	void startGame();
	void loadResource();
	void addGameInfo();
	void addRes();
	void addMap();
	void addLeaderAndMon();
	void addCtrl();
	void addJumpAndFireMenu();
	void addstopMenu();
	void openAntiAlias();

	void openSchedule();
	void loadImageFuncCall(CCObject*);
	void setPauseFuncCall();

	void moveLeftCallBack(CCObject*);
	void moveRightCallBack(CCObject*);
	void jumpCallBack(CCObject*);
	void fireCallBack(CCObject*);
	void stopCallBack(CCObject *);

public:
	void GameOver(Common::OVERTYPE type);
	void disaCall();
	void checkMarioDie(float);
	void checkMarioTouchPole(float);
	void moveMarioUpd(float);
	void checkTimeOut(float);

protected:
	Mario* _mario;
	LayerLoadRes* _bar;
	CCTexture2D* _textureDirNone;
	CCTexture2D* _textureDirLeft;
	CCTexture2D* _textureDirRight;
	CCSprite* _menuShow;

	Common::DIRECTION _marioDir;
	int _totalfile;
	int _loadFileCount;
public:
	CCTMXTiledMap* _map;
	static int _idx;
	static CCLabelBMFont *monsterScore;

	CCSprite *coin;
};

#endif