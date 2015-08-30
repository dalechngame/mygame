#include "Common.h"
#include "Stone.h"

CCPoint Common::Plate2Screen(int row, int col)
{
	int x = col * Stone::_d + Stone::_offx;
	int y = row * Stone::_d + Stone::_offy;

	return ccp(x, y);
}

bool Common::Screen2Plate(const CCPoint& ptSceen, int& row, int& col)
{
	// 遍历所有象棋坐标点，计算棋盘格子中心坐标点到点击的店的距离，如果小于半径，那么就对了
	int distance = Stone::_d*Stone::_d / 4;
	for (row = 0; row <= 9; ++row)
	for (col = 0; col <= 8; ++col)
	{
		CCPoint ptCenter = Plate2Screen(row, col);
		if (ptCenter.getDistanceSq(ptSceen) < distance)
		{
			return true;
		}
	}
	return false;
}

int Common::getStoneFromRowCol(int row, int col, Stone **s)
{
	for (int i = 0; i < 32; ++i)
	{
		if (s[i]->_row == row && s[i]->_col == col && !s[i]->_dead)
			return i;
	}
	return -1;
}

int Common::getStoneCount(int row1, int col1, int row2, int col2, Stone **s)
{
	int ret = 0;
	// 棋子当前位置和选中的位置必须在一条直线上
	if (row1 != row2 && col1 != col2) return -1;
	if (row1 == row2 && col1 == col2) return -1;

	if (row1 == row2)
	{
		int min, max;
		min = col1 < col2 ? col1 : col2;
		max = col1 > col2 ? col1 : col2;
		for (int col = min + 1; col < max; ++col)
		{
			int id = getStoneFromRowCol(row1, col, s);
			if (id != -1) ++ret;
		}
	}
	else
	{
		int min, max;
		min = row1 < row2 ? row1 : row2;
		max = row1 > row2 ? row1 : row2;
		for (int row = min + 1; row < max; ++row)
		{
			int id = getStoneFromRowCol(row, col1, s);
			if (id != -1) ++ret;
		}
	}

	return ret;
}

bool Common::isSameColor(int id1, int id2, Stone **s)
{
	return s[id1]->_red == s[id2]->_red;
}

void Common::moveNode(CCNode* node, CCPoint pt)
{
	node->setPosition(node->getPosition() + pt);
}