// Created by YUANZHONGJI on 2015/09

#ifndef __LOCK_GAME_LAYER_H__
#define __LOCK_GAME_LAYER_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "Common.h"

USING_NS_CC;

class LockGameLayer : public WJLayer
{
private:

	WJLayer *m_coverLayer;
	WJLayer *m_lockLayer;
	GameNumber m_gameNumber;

	// 禁止屏幕点击
	void disableTouchOnScreen(float duration);
	void showLockLayerInGame();
	void onBigLockClicked(Node* pNode, WJTouchEvent* e);
	void onCoverLayerClick(Node *node, WJTouchEvent *event);

public:

	LockGameLayer();

	static LockGameLayer* create(GameNumber number);

	virtual bool init(GameNumber number);

    virtual void onEnter() override;
    virtual void onExit() override;

	virtual void onEnterTransitionDidFinish() override;

public:

	// 显示当前的加锁层
	void show();
	// 是否解锁
	bool isGameLocked();
};

#endif  // __LOCK_GAME_LAYER_H__
