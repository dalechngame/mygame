#ifndef __LayerGameMain_H__  		
#define __LayerGameMain_H__  		

#include "cocos2d.h"  	

#include "LayerBullet.h"
#include "LayerEnemy.h"
#include "LayerControl.h"
#include "LayerFood.h"
USING_NS_CC;  				

//! 击毁敌机获得得分数
const int  SMALL_SCORE = 1000;
const int  MID_SCORE = 6000;
const int  BIG_SCORE = 30000;

const int MAX_BIGBOOM_COUNT = 100000;
//! 炸弹图片的tag
const int TAG_BIGBOOM = 1000;
//! 炸弹图片计数的tag
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
	//! 总分数
	static int score;
	//! 炸弹个数
	int bigBoomCount;

	void addBackGround();
	//背景动起来
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

	//! 触摸事件回调

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

	void update(float dt);
	//! 更新炸弹个数
	void  updateBigBoomCount(int bigBoomCount);
	void boomMenuCallBack(CCObject * obj);
};  	

#endif  					
