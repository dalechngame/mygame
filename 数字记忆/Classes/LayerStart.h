#ifndef __LayerStart_H__
#define __LayerStart_H__

#include "cocos2d.h"
USING_NS_CC;
class LayerStart:public CCLayer
{
public:
	CREATE_FUNC(LayerStart);
	static CCScene * scene();
	bool init();

	void easy(CCObject* obj);
	void hard(CCObject* obj);
	void veryHard(CCObject* obj);
	void quit(CCObject* obj);

	enum DEGREE
	{
		EASY =5 ,HARD = 7,VERYHARD= 9
	};
};


#endif



