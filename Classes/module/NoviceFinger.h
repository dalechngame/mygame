// Created by YUANZHONGJI on 2015/09

#ifndef __NOVICE_FINGER_H__
#define __NOVICE_FINGER_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"

USING_NS_CC;

#define FINGER_SPEED           300.0f              // 提示手指的移动速度、

class NoviceFinger : public WJLayer
{
private:
	// 手指
	WJSprite *m_sprite;

public:
	// 手指移动提示
	void startMoveFingerTips(Vec2 &fromPointWorld, Vec2 &toPointWorld, int repeatCount, float interval);
	// 循环每隔6.0s提示2次手指移动的提示
	void startMoveFingerTipsForever(Vec2 &fromPointWorld, Vec2 &toPointWorld, float interval);
	// 停止手指的提示
	void stopMoveFingerTips();
	// 6.0s之后开始做手指提示的动画
	void delayStartMoveFingerTips(Vec2 &fromPointWorld, Vec2 &toPointWorld, int repeatCount, float interval, float delay);
	
	// 6.0s之后开始循环做手指的提示动画
	void delayStartMoveFingerTipsForever(Vec2 &fromPointWorld, Vec2 &toPointWorld, float interval, float delay);

private:

	void firstDelayMoveFingerForever(Vec2 &fromPointWorld, Vec2 &toPointWorld, int repeatCount, float interval, float delay);

public:

	static NoviceFinger* create();

	virtual bool init() override;

    virtual void onEnter() override;
    virtual void onExit() override;

	virtual void onEnterTransitionDidFinish() override;
};

#endif  // __NOVICE_FINGER_H__
