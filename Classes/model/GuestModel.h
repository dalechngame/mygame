// Created by wangwenjie on 2013/12

#ifndef __GUEST_MODEL_H__
#define __GUEST_MODEL_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "WJSkeletonAnimation.h"
#include "ModelBase.h"

class GuestModel : public ModelBase
{
private:
	// 播放常态的动画
	void playGuestIdleAni();
	// 点击了客人之后
	void onClickGuestModel(Node *node, WJTouchEvent *event);

	virtual void onCompletedListener(int trackIndex, int loopCount) override;

	// 手抬起的事件+Food事件
	void onSkeletonEvent(int stateIndex, spEvent* pEvent, WJSkeletonAnimation *skeleton);

public:

	// 播放脸部的粒子效果
	void playParticleWithFace(const Vec2 &offset = Vec2::ZERO);

	// 播放动画(如果有其他的动画正在播放则不播放当前的动画)
	void playAniWithNotPlayingOther(const char* animationName, bool loop, int stateIndex);

	// 设置点击事件
	void setModelClickEvent();

public:

	GuestModel();

    static GuestModel* create(int modelNo, bool playIdle = true);

    virtual bool init();

    virtual void onEnter();
	virtual void onExit();
    virtual void onEnterTransitionDidFinish();
};

#endif // __GUEST_MODEL_H__
