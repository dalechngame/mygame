#ifndef __GameStartLayer_H__  		
#define __GameStartLayer_H__  	

#include "cocos2d.h"  		
#include "Stone.h"
#include <vector>
using namespace std;
USING_NS_CC;  				

struct Step
{
	int moveid;
	int killid;
	int rowFrom;
	int colFrom;
	int rowTo;
	int colTo;
};

class GameStartLayer :public CCLayer 
{  					
public: 					
	static CCScene * scene();		
	CREATE_FUNC(GameStartLayer); 		

	void CreatePlate();
	void addCtrlPanel();

	inline void moveNode(CCNode *node, CCPoint pt)
	{
		node->setPosition(node->getPosition() + pt);
	}

	Stone * _s[32];

	bool init();  			
	void onExit();

	bool ccTouchBegan(CCTouch *, CCEvent *) override;
	void ccTouchEnded(CCTouch *, CCEvent *) override;

	// �ƶ�����
	/* ˧��������King
		�ˣ�ʿ����Guard
		�ࣨ�󣩣�Eleph����Elephant�����ֽϳ���ȡ��д��ʽ��
		��Horse
		����Rook
		�ڣ�Cannon
		�䣨������Pawn                                                                     */
	bool canMove(int moveid, int row, int col, int killid);
	bool canMoveRook(int moveid, int row, int col);
	bool canMoveCannon(int moveid, int row, int col, int killid);
	bool canMoveHorse(int moveid, int row, int col);
	bool canMovePawn(int moveid, int row, int col);
	bool canMoveKing(int moveid, int row, int col, int killid);
	bool canMoveGuard(int moveid, int row, int col);
	bool canMoveEleph(int moveid, int row, int col);

	void computerMove(float);

	//! ��ȡ���������֮������Ӹ���
	int getStoneCount(int row1, int col1, int row2, int col2);


	//! ѡ����ƶ�
	void selectStone(CCTouch *touch);
	void moveStone(CCTouch *touch);
	// ͨ���������꣬��ȡ�����ϵ�����ID,-1��ʾû���ҵ�����
	int getStoneFromRowCol(int row, int col);
	//! �ж�����������ɫ�Ƿ���ͬ
	inline bool isSameColor(const int &id1, const int &id2)
	{
		return _s[id1]->_red == _s[id2]->_red;
	}

	//! ��¼�Ƿ�ѡ��, -1��ʾδ��ѡ��
	int _selectid;
	//! ���Ŀǰ�Ƿ��ɫ����turn
	bool _bRedTurn;
	//! ѡ�б�Ǿ���
	CCSprite *_selectedSprite;


	//! ����ת������
	bool Screen2Plate(const CCPoint& ptScreen, int& row, int& col);
	CCPoint Plate2Screen(int row, int col); 

	//! �������
	void recordStep(int moveid, int killid, int rowFrom, int colFrom, int rowTo, int colTo);
	void regret(CCObject *);

	vector <Step *> _steps;

};  					
#endif  					
