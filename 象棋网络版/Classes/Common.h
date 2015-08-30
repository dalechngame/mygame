#ifndef _COMMON_H_
#define _COMMON_H_

#include "cocos2d.h"
USING_NS_CC;

#define winSize CCDirector::sharedDirector()->getWinSize()
class Stone;

// ��������
enum TYPE{ CHE, MA, PAO, BING, JIANG, SHI, XIANG };

enum ZORDER{Z_CTRL = 100, Z_MENU = 200, Z_PLATE = 300, Z_STONE = 400};

enum SELECT{NORMAL = -1, SELECTED};
// ������Ϣ
struct Step
{// ѡ�������, Ŀ������, ��ǰ����, Ŀ������
	int moveid;	int killid;	int rowFrom;
	int colFrom;	int rowTo;	int colTo;
};

class Common
{
public:

	static void moveNode(CCNode* node, CCPoint pt);

	static CCPoint Plate2Screen(int row, int col);

	// ͨ����Ļ���꣬��ȡ�������꣬����ֵ�����false��ʾptScreen��������
	static bool Screen2Plate(const CCPoint& ptSceen, int& row, int& col);

	// ͨ���������꣬��ȡ�����ϵ�����ID���������-1��ʾ��λ����û�����ӣ��������-1�����Ƕ�Ӧ����id
	static int getStoneFromRowCol(int row, int col, Stone **s);

	// ��ȡ���������֮������Ӹ���
	static int getStoneCount(int row1, int col1, int row2, int col2, Stone **s);

	// �ж�����������ɫ�Ƿ���ͬ
	static bool isSameColor(int id1, int id2, Stone **s);
};

#endif