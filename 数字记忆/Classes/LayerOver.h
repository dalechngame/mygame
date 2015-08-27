#ifndef __LayerOver_H__
#define __LayerOver_H__


#include "cocos2d.h"
USING_NS_CC;


class LayerOver : public CCLayer
{
public:
	static CCScene * scene(bool res);
	static LayerOver  *create(bool res);
	bool init(bool res);

	void menuCallBack(CCObject * obj);
};


#endif
