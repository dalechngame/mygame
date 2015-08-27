#ifndef __Enemy_H__  		
#define __Enemy_H__  		

#include "cocos2d.h"  			
USING_NS_CC;  				

//! 创建敌人的工厂方法
class Enemy :public CCNode 
{  					
public: 					
	Enemy();
	~Enemy();
	static Enemy *create();

	void bindEnemySprite(CCSprite *sp, int life);
	CCSprite *getSprite();
	int getLife();
	void loseLife();
	CCRect getBoundingBox();

private:
	CCSprite *m_psprite;
	int m_nlife;
};  					

#endif  					
