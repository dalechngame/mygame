#ifndef __Plane_H__
#define __Plane_H__

#include "cocos2d.h"
USING_NS_CC;

class Plane :public CCSprite
{
public:
	static Plane* getInstance();
	static Plane* _splane;
	bool init();
	CC_SYNTHESIZE(int, life, Life);
	void animatePlane();

	void planeUpdate();
	void blowUp();
	void removePlane();
};


#endif




