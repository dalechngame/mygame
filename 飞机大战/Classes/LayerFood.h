#ifndef __LayerFood_H__  		
#define __LayerFood_H__  		

#include "cocos2d.h"  			
USING_NS_CC;  				

class LayerFood :public CCLayer 
{  					
public: 					
	CREATE_FUNC(LayerFood); 		
	bool init();  				

	void addBigBoom(float dt);
	void bigBoomMoveFinished(CCNode *pSender);
	void removeBigBoom(CCSprite *bigBoom);
public:
	//CCArray *multiBulletArray;
	//! ը���Ĵ洢����
	CCArray *m_pbigBoomArray;

};  					

#endif  					

