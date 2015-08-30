#ifndef __STONE_H__
#define __STONE_H__

#include "Common.h"

class Stone : public CCSprite
{
public:
	CREATE_FUNC(Stone);
	bool init();

	void initStone(int id);
	CCPoint fromPlate();

	int _id;
	bool _red;
	int _row;
	int _col;
	TYPE _type;
	bool _dead;

	static int _d;
	static int _offx;
	static int _offy;
};

#endif
