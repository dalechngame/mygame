
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

	//////////////////////////////ʵ���߼�����
	void SelectStone(CCTouch* touch);
	void MoveStone(CCTouch* touch);
	void recordStep(int moveid, int killid, int rowFrom, int colFrom, int rowTo, int colTo);
	
	int _selectid; //-1��ʾ֮ǰû�����ӱ�ѡ��
	bool _bRedTurn; 	// ���Ŀǰ��˭��
	bool _bRedSide;	//  ����Ǻ�ɫ�����Ǻ�ɫ
	CCSprite* _selectSprite; // ѡ�б�Ǿ���
	vector<Step*> _steps; 	// ��¼����켣��������
	Stone* _s[32];

};

#endif

