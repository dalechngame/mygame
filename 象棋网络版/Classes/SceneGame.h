
#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "common.h"
#include "Stone.h"
#include <vector>
using namespace std;

class SceneGame : public CCLayer
{
public:
	
	CREATE_FUNC(SceneGame);
	bool init();
	bool ccTouchBegan(CCTouch*, CCEvent*);
	void ccTouchEnded(CCTouch*, CCEvent*);

	void CreatePlate();
	void addCtrlPanel();

	void Regret(CCObject*);
	void doRegret();
	void doRegret2();

	void startServer(CCObject*);
	void startClient(CCObject*);
	void CheckListen(float);
	void CheckRecv(float);

	//////////////////////////////实现逻辑函数
	void SelectStone(CCTouch* touch);
	void MoveStone(CCTouch* touch);
	void recordStep(int moveid, int killid, int rowFrom, int colFrom, int rowTo, int colTo);
	
	int _selectid; //-1表示之前没有棋子被选中
	bool _bRedTurn; 	// 标记目前该谁走
	bool _bRedSide;	//  标记是红色方还是黑色
	CCSprite* _selectSprite; // 选中标记精灵
	vector<Step*> _steps; 	// 记录走棋轨迹用来悔棋
	Stone* _s[32];

};

#endif

