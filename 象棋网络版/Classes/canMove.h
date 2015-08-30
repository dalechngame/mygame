#ifndef _CANMOVE_H_
#define _CANMOVE_H_

#include "common.h"
class SceneGame;
class Stone;

class canMove
{
public:
	static canMove *createMove(SceneGame *game, int moveid, int row, int col, int killid);

	canMove(SceneGame *game, int moveid, int row, int col, int killid);

	virtual bool move() = 0;

	int _moveid;
	int _row;
	int _col;
	int _killid;

	SceneGame *_game;
	Stone *_stone;
	Stone **_s;
};

class canMoveChe :public canMove
{
public:

	canMoveChe(SceneGame *game, int moveid, int row, int col, int killid) :
	canMove(game, moveid, row, col, killid){}

	virtual bool move();
};


class canMovePao :public canMove
{
public:

	canMovePao(SceneGame *game, int moveid, int row, int col, int killid) :
	canMove(game, moveid, row, col, killid){}

	virtual bool move();

};


class canMoveMa :public canMove
{
public:
	canMoveMa(SceneGame *game, int moveid, int row, int col, int killid) :
	canMove(game, moveid, row, col, killid){}

	virtual bool move();
};


class canMoveBing :public canMove
{
public:
	canMoveBing(SceneGame *game, int moveid, int row, int col, int killid) :
	canMove(game, moveid, row, col, killid){}

	virtual bool move();
};

class canMoveJiang :public canMove
{
public:
	canMoveJiang(SceneGame *game, int moveid, int row, int col, int killid) :
	canMove(game, moveid, row, col, killid){}

	~canMoveJiang()
	{
		CC_SAFE_DELETE(m);
	}
	
	virtual bool move();

	canMove * m;
};

class canMoveXiang :public canMove
{
public:

	canMoveXiang(SceneGame *game, int moveid, int row, int col, int killid) :
	canMove(game, moveid, row, col, killid){}

	virtual bool move();
};

class canMoveShi :public canMove
{
public:
	canMoveShi(SceneGame *game, int moveid, int row, int col, int killid) :
	canMove(game, moveid, row, col, killid){}

	virtual bool move();
};

#endif

