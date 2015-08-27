#ifndef __LayerBullet_H__  		
#define __LayerBullet_H__  		

#include "cocos2d.h"  			
USING_NS_CC;  				

class LayerBullet :public CCLayer 
{  					
public: 					
	CREATE_FUNC(LayerBullet); 		
	bool init();  				

	void startShoot();
	void stopShoot();

	void addBulletCallback(float dt);
	void bulletMoveFinished(CCNode *node);
	void removeBullet(CCSprite *bullet);

	//! ×Óµ¯µÄCCArray
	CCArray *m_pbulletArray;
	CCSpriteBatchNode *m_pbulletBatchNode;

};  					

#endif  					
