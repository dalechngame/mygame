#ifndef __Block_H__
#define __Block_H__
#include "cocos2d.h"
USING_NS_CC;

class Block :public CCSprite
{
public:
	//! block的尺寸.颜色.	字体内容.大小.颜色
	static Block * create(CCSize size, ccColor3B color,
						  CCString str, float strSize, ccColor3B strColor);
	bool init(CCSize size, ccColor3B color,
			  CCString str, float strSize, ccColor3B strColor);

	//记录block的数量
	static CCArray *array;
	static CCArray *getBlocksArray();

	//记录行数
	CC_SYNTHESIZE(int, _lineIndex, LineIndex);

	void moveDownAndCleanUp();
};

#endif

