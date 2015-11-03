#ifndef __P012_SHOW_H__
#define __P012_SHOW_H__

#include "PBase.h"
#include "CircleBy.h"
#include "ScrollLayer.h"
#include "ScrollDoubleLayer.h"

USING_NS_CC;

enum class changeState
{
	changeBall,
	changDecor,
	changeBox
};

class P012_show : public PBase
{
protected:

	int m_ballIndex;
	ScrollLayer *m_scrollLayer;
	ScrollDoubleLayer *m_scrollDoubleLayer;
	changeState m_changeState;

	int m_globalZorderMax;

	virtual GameNumber getGameNumber() override;

	void initEvent();

	void onClickBallScrollItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	bool onScrollBallMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollBallCollisonEndedEvent(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	// 点击了一级菜单
	void onClickLeftScrollItemCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onClickScrollItemCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	bool onScrollLayerMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollItemCollisonEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	// 装饰品
	bool onScrollDecorMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollDecorCollisonEndedEvent(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onClickDecorScrollItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	int checkDecorSpritePoint(Node *node);
	bool onDecorSpriteTouchAble(Node *node, WJTouchEvent *event);
	void onDecorSpriteTouchEnded(Node *node, WJTouchEvent *event);

	// 底座
	void onClickBoxScrollItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	void playBallLayerAni(WJLayer *layer);

	virtual void onClickNextButton(Node *node, WJTouchEvent *event) override;

public:

	P012_show();
	virtual ~P012_show();

	virtual bool init() override;

	virtual void onEnterTransitionDidFinish() override;

public:

	CREATE_SCENE_FUNC(P012_show);
};

#endif // __P012_SUGAR_H__
