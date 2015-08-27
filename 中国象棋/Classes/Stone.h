#ifndef __STONE_H__
#define __STONE_H__

#include "cocos2d.h"

USING_NS_CC;

/************************************************************************/
/* ˧��������King
	�ˣ�ʿ����Guard
	�ࣨ�󣩣�Eleph����Elephant�����ֽϳ���ȡ��д��ʽ��
	��Horse
	����Rook
	�ڣ�Cannon
	�䣨������Pawn                                                                     */
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
		TYPE _type; ///> ��������
		int _row; ///> ��
		int _col; ///> ��
	} _proper;

	bool _dead;
	bool _red;	///> �Ƿ��ɫ��
	int _id;	///> id


	//! ��������
	void setProper(const int &id);
};

#endif
