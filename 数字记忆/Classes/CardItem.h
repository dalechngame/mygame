#ifndef __CardItem_H__
#define __CardItem_H__

#include "cocos2d.h"
USING_NS_CC;

class CardItem :public CCSprite
{
public:
	static CardItem * create(int idx);
	bool init(int idx);

	CCLabelTTF *ttf;
	CCSprite *bg;

	CC_SYNTHESIZE(int, _index, Idx);

	void showWhite();
	void showText();

};

#endif
