#ifndef __LayerEnemy_H__  		
#define __LayerEnemy_H__  		
#include "cocos2d.h"  			
#include "Enemy.h"


USING_NS_CC;  			

//! ���ַɻ�������ֵ
const int SMALL_MAXLIFE = 1;
const int MID_MAXLIFE = 3;
const int BIG_MAXLIFE = 5;

class LayerEnemy :public CCLayer 
{  					
public: 					
	CREATE_FUNC(LayerEnemy); 		
	bool init();  			

	//! С�ɻ�
	void addSmallEnemy(float dt);
	void smallEnemyMoveFinishedFCallBack(CCNode *node);
	void smallEnemyBlowUp(Enemy * smallEnemy);

	void removeSmallEnemyFCallBack(CCNode * target, void * data);
	//! ��removeAllEnemy����
	void removeAllSmallEnemy();

	void removeAllEnemy();

	//! smallEnemy�Ĵ洢����
	CCArray * m_psmallArray;

};  					

#endif  					
