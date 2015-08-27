#ifndef __STONE_H__
#define __STONE_H__

#include "cocos2d.h"

USING_NS_CC;

/************************************************************************/
/* 帅（将）：King
	仕（士）：Guard
	相（象）：Eleph（“Elephant”名字较长，取简写形式）
	马：Horse
	车：Rook
	炮：Cannon
	卒（兵）：Pawn                                                                     */
/************************************************************************/

class Stone : public CCSprite
{
public:
	CREATE_FUNC(Stone);
	bool init();


	void initStone(int id);
	CCPoint fromPlate();

	static int _d;
	static int _offx;
	static int _offy;


	enum TYPE{
		KING, GUARD, ELEPH, HORSE, ROOK, CANNON, PAWN
	};
	struct Proper
	{
		TYPE _type; ///> 棋子类型
		int _row; ///> 行
		int _col; ///> 列
	} _proper;

	bool _dead;
	bool _red;	///> 是否红色方
	int _id;	///> id


	//! 设置属性
	void setProper(const int &id);
};

#endif
