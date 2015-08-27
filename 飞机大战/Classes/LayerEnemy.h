#ifndef __LayerEnemy_H__  		
#define __LayerEnemy_H__  		
#include "cocos2d.h"  			
#include "Enemy.h"


USING_NS_CC;  			

//! 各种飞机的生命值
const int SMALL_MAXLIFE = 1;
const int MID_MAXLIFE = 3;
const int BIG_MAXLIFE = 5;

class LayerEnemy :public CCLayer 
{  					
public: 					
	CREATE_FUNC(LayerEnemy); 		
	bool init();  			

	//! 小飞机
	void addSmallEnemy(float dt);
	void smallEnemyMoveFinishedFCallBack(CCNode *node);
	void smallEnemyBlowUp(Enemy * smallEnemy);

	void removeSmallEnemyFCallBack(CCNode * target, void * data);
	//! 给removeAllEnemy调用
	void removeAllSmallEnemy();

	void removeAllEnemy();

	//! smallEnemy的存储数组
	CCArray * m_psmallArray;

};  					

#endif  					
