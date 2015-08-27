#ifndef __Block_H__
#define __Block_H__
#include "cocos2d.h"
USING_NS_CC;

class Block :public CCSprite
{
public:
	//! block�ĳߴ�.��ɫ.	��������.��С.��ɫ
	static Block * create(CCSize size, ccColor3B color,
						  CCString str, float strSize, ccColor3B strColor);
	bool init(CCSize size, ccColor3B color,
			  CCString str, float strSize, ccColor3B strColor);

	//��¼block������
	static CCArray *array;
	static CCArray *getBlocksArray();

	//��¼����
	CC_SYNTHESIZE(int, _lineIndex, LineIndex);

	void moveDownAndCleanUp();
};

#endif

