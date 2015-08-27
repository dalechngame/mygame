#ifndef __LayerGameMain_H__  		
#define __LayerGameMain_H__  		

#include "cocos2d.h"  	

#include "LayerBullet.h"
#include "LayerEnemy.h"
#include "LayerControl.h"
#include "LayerFood.h"
USING_NS_CC;  				

//! ���ٵл���õ÷���
const int  SMALL_SCORE = 1000;
const int  MID_SCORE = 6000;
const int  BIG_SCORE = 30000;

const int MAX_BIGBOOM_COUNT = 100000;
//! ը��ͼƬ��tag
const int TAG_BIGBOOM = 1000;
//! ը��ͼƬ������tag
const int TAG_BIGBOOMCOUNT = 2000;

class LayerGameMain :public CCLayer 
{  					
public: 					
	static CCScene * scene();		
	CREATE_FUNC(LayerGameMain); 		
	bool init();  				

	enum BACKGROUND
	{
		BACK1, BACK2
	};
	//! �ܷ���
	static int score;
	//! ը������
	int bigBoomCount;

	void addBackGround();
	//����������
	void movingBackGround(float dt);
	void addHero();
	void addBulletLayer();
	void addEnemyLayer();
	void addCtrlLayer();
	void addFoodLayer();

	LayerBullet  * m_pbulletLayer;
	LayerEnemy   * m_penemyLayer;
	LayerControl * m_pctrlLayer;
	LayerFood    * m_pfoodLayer;

	//! �����¼��ص�

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

	void update(float dt);
	//! ����ը������
	void  updateBigBoomCount(int bigBoomCount);
	void boomMenuCallBack(CCObject * obj);
};  	

#endif  					
