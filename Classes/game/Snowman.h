#ifndef __SNOWMAN_H__
#define __SNOWMAN_H__

#include "PBase.h"
#include "MulGestureSprite.h"

class P003;
class Tools;

class Snowman : public WJLayer
{
protected:
	void initSnowman(WJLayerJson *layer);
	void initGlove();
	void onClickSnowman(Node *node, WJTouchEvent *event);
	void dealWithSnow(bool remove = false);

protected:
	WJLayerJson *m_json = nullptr;
	CC_SYNTHESIZE(PBase*, m_parent, PBase);						// P003引用
	CC_PROPERTY(ProgressStar, m_progressState, ProgressState); 	// 星星

	CC_SYNTHESIZE(WJSkeletonAnimation *, m_snowman ,Snowman);
	WJSprite *m_drift = nullptr;
	MulGestureSprite *m_gues = nullptr;
	static int enterTimes;

public:
	// 播放新手动画
	void startplayCircleAni(float delay = 0.f); // unused param
	void stopplayCircleAni();

	bool isShovelCollidedWithDrift(Tools *shovel);
	void makeSnowman(Tools *shovel);
	void removeDrift();
	float getSnowmanBottomPos();

public:
	Snowman(){
		m_json = nullptr;
		m_parent = nullptr;
		m_progressState = ProgressStar::noStar;
	}
	~Snowman(){}

	static Snowman* create(WJLayerJson *layer, PBase *parent);
	virtual bool init(WJLayerJson *layer, PBase *parent);
	virtual void onEnterTransitionDidFinish() override;

};

#endif // __SNOWMAN_H__
