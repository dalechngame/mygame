#include "canMove.h"
#include "Stone.h"
#include "SceneGame.h"

canMove::canMove(SceneGame *game, int moveid, int row, int col, int killid) :
_game(game), _moveid(moveid), _row(row),
_col(col), _killid(killid)
{

	_stone = game->_s[moveid];
	_s = game->_s;
}

canMove* canMove::createMove(SceneGame *game, int moveid, int row, int col, int killid)
{
	Stone* s = game->_s[moveid];
	switch (s->_type)
	{
	case ::CHE:
		return new canMoveChe(game, moveid, row, col, killid);

	case ::MA:
		return new canMoveMa(game, moveid, row, col, killid);

	case ::PAO:
		return new canMovePao(game, moveid, row, col, killid);

	case ::BING:
		return new canMoveBing(game, moveid, row, col, killid);

	case ::JIANG:
		return new canMoveJiang(game, moveid, row, col, killid);

	case ::SHI:
		return new canMoveShi(game, moveid, row, col, killid);

	case ::XIANG:
		return new canMoveXiang(game, moveid, row, col, killid);

		break;
	}
	return false;
}

bool canMoveShi::move()
{
	int dRow = abs(_stone->_row - _row);
	int dCol = abs(_stone->_col - _col);
	int d = dRow * 10 + dCol;
	if (d != 11) return false;

	// 不能出九宫
	if (_col < 3 || _col > 5) return false;
	if (_row < 0 || _row > 2) return false;

	return true;
}

bool canMoveXiang::move()
{
	int dRow = abs(_stone->_row - _row);
	int dCol = abs(_stone->_col - _col);
	int d = dRow * 10 + dCol;
	if (d != 22) return false;

	// 象眼
	int cRow = (_row + _stone->_row) / 2;
	int cCol = (_col + _stone->_col) / 2;
	if (Common::getStoneFromRowCol(_row, _col, _s) != -1)
		return false;

	// 判断象不能过河
	if (_row > 4) return false;


	return true;
}

bool canMoveJiang::move()
{
	// 将照面
	if (_killid != -1)
	{
		Stone* kill = _s[_killid];
		if (kill->_type == JIANG)
		{
			m = new canMoveChe(_game, _moveid, _row, _col, _killid);
			return m->move();
		}
	}

	int dRow = abs(_stone->_row - _row);
	int dCol = abs(_stone->_col - _col);
	int d = dRow * 10 + dCol;

	// 走太远不行,只能走一格
	if (d != 1 && d != 10)
		return false;

	// 不能出九宫
	if (_col < 3 || _col > 5) return false;
	if (_row < 0 || _row > 2) return false;

	return true;
}

bool canMoveBing::move()
{
	int dRow = abs(_stone->_row - _row);
	int dCol = abs(_stone->_col - _col);
	int d = dRow * 10 + dCol;

	// 走太远不行,只能走一格
	if (d != 1 && d != 10)
		return false;

	// 不能后退
	if (_row < _stone->_row) return false;

	// 过河前不能平移
	if (_stone->_row <= 4 && _stone->_row == _row) return false;

	return true;
}

bool canMoveMa::move()
{
	int dRow = abs(_stone->_row - _row);
	int dCol = abs(_stone->_col - _col);
	int d = dRow * 10 + dCol;

	// 马走日条件
	if (d == 12 || d == 21)
	{
		// 蹩脚位置
		int cRow, cCol;
		if (d == 12)
		{
			cCol = (_col + _stone->_col) / 2;
			cRow = _stone->_row;
		}
		else
		{
			cCol = _stone->_col;
			cRow = (_stone->_row + _row) / 2;
		}

		// 没有蹩脚的棋子
		if (Common::getStoneFromRowCol(_row, _col, _s) == -1)
			return true;
	}

	return false;
}

bool canMovePao::move()
{
	if (_killid == -1)
		return Common::getStoneCount(_stone->_row, _stone->_col, _row, _col, _s) == 0;

	return Common::getStoneCount(_stone->_row, _stone->_col, _row, _col, _s) == 1;
}

bool canMoveChe::move()
{
	return Common::getStoneCount(_stone->_row, _stone->_col, _row, _col, _s) == 0;;
}
