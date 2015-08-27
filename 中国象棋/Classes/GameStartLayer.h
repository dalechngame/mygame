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

	// 移动规则
	/* 帅（将）：King
		仕（士）：Guard
		相（象）：Eleph（“Elephant”名字较长，取简写形式）
		马：Horse
		车：Rook
		炮：Cannon
		卒（兵）：Pawn                                                                     */
	bool canMove(int moveid, int row, int col, int killid);
	bool canMoveRook(int moveid, int row, int col);
	bool canMoveCannon(int moveid, int row, int col, int killid);
	bool canMoveHorse(int moveid, int row, int col);
	bool canMovePawn(int moveid, int row, int col);
	bool canMoveKing(int moveid, int row, int col, int killid);
	bool canMoveGuard(int moveid, int row, int col);
	bool canMoveEleph(int moveid, int row, int col);

	void computerMove(float);

	//! 获取两个坐标点之间的棋子个数
	int getStoneCount(int row1, int col1, int row2, int col2);


	//! 选择和移动
	void selectStone(CCTouch *touch);
	void moveStone(CCTouch *touch);
	// 通过棋盘坐标，获取坐标上的棋子ID,-1表示没有找到棋子
	int getStoneFromRowCol(int row, int col);
	//! 判断两个棋子颜色是否相同
	inline bool isSameColor(const int &id1, const int &id2)
	{
		return _s[id1]->_red == _s[id2]->_red;
	}

	//! 记录是否被选中, -1表示未被选中
	int _selectid;
	//! 标记目前是否红色方的turn
	bool _bRedTurn;
	//! 选中标记精灵
	CCSprite *_selectedSprite;


	//! 坐标转换函数
	bool Screen2Plate(const CCPoint& ptScreen, int& row, int& col);
	CCPoint Plate2Screen(int row, int col); 

	//! 悔棋相关
	void recordStep(int moveid, int killid, int rowFrom, int colFrom, int rowTo, int colTo);
	void regret(CCObject *);

	vector <Step *> _steps;

};  					
#endif  					
