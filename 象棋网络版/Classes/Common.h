#ifndef _COMMON_H_
#define _COMMON_H_

#include "cocos2d.h"
USING_NS_CC;

#define winSize CCDirector::sharedDirector()->getWinSize()
class Stone;

// 棋子类型
enum TYPE{ CHE, MA, PAO, BING, JIANG, SHI, XIANG };

enum ZORDER{Z_CTRL = 100, Z_MENU = 200, Z_PLATE = 300, Z_STONE = 400};

enum SELECT{NORMAL = -1, SELECTED};
// 步骤信息
struct Step
{// 选择的棋子, 目标棋子, 当前行列, 目标行列
	int moveid;	int killid;	int rowFrom;
	int colFrom;	int rowTo;	int colTo;
};

class Common
{
public:

	static void moveNode(CCNode* node, CCPoint pt);

	static CCPoint Plate2Screen(int row, int col);

	// 通过屏幕坐标，获取棋盘坐标，返回值如果是false表示ptScreen在棋盘外
	static bool Screen2Plate(const CCPoint& ptSceen, int& row, int& col);

	// 通过棋盘坐标，获取坐标上的棋子ID，如果返回-1表示该位置上没有棋子，如果不是-1，就是对应棋子id
	static int getStoneFromRowCol(int row, int col, Stone **s);

	// 获取两个坐标点之间的棋子个数
	static int getStoneCount(int row1, int col1, int row2, int col2, Stone **s);

	// 判断两个棋子颜色是否相同
	static bool isSameColor(int id1, int id2, Stone **s);
};

#endif