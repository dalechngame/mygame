#ifndef __P012_SUGAR_H__
#define __P012_SUGAR_H__

#include "PBase.h"
#include "CircleBy.h"
#include "ScrollLayer.h"

USING_NS_CC;

class P012 : public PBase
{
protected:

	WJSprite *m_breadpotSprite;
	WJSprite *m_bowlSprite;
	int m_putBreadNum;
	WJSkeletonAnimation *m_frostSkeleton;
	WJSprite *m_mixerSprite;

	EllipseConfig m_ellipseConfig;
	bool m_isPauseAction;
	int m_mixRecord;
	WJLayer *m_ballsPanel;
	WJSkeletonAnimation *m_dipperSkeleton;
	int m_basicNum;

	ScrollLayer *m_scrollLayer;
	virtual GameNumber getGameNumber() override;
	virtual void onClickNextButton(Node *node, WJTouchEvent *event) override;

	void initEvent();
	void restoreSavedPositionAndZorder(Node *node);

	// 面包片
	void showBreadInTheBowl();
	void setBreadVisible(int startIndex, int endIndex, bool visible);
	bool onBreadTouchAble(Node *node, WJTouchEvent *event);
	bool onBreadWillMoveTo(Node *node, WJTouchEvent *event);
	void onBreadTouchEnded(Node *node, WJTouchEvent *event);
	void onBreadMoveInCallBack(Node *node);

	// 倒奶油
	void onFrostMoveEnded();
	bool onFrostTouchAble(Node *node, WJTouchEvent *event);
	bool onFrostWillMoveTo(Node *node, WJTouchEvent *event);
	void onFrostTouchEnded(Node *node, WJTouchEvent *event);
	void onFrostPlayAniEnded();

	// 搅拌的棒子
	bool onMixerTouchAble(Node *node, WJTouchEvent *event);
	bool onMixerWillMoveTo(Node *node, WJTouchEvent *event);
	void onMixerTouchEnded(Node *node, WJTouchEvent *event);
	void onMixerSpriteMoveInEnd();

	// 勺子
	void onDipperMoveInEnded(Node *node);
	bool onDipperTouchAble(Node *node, WJTouchEvent *event);
	bool onDipperWillMoveTo(Node *node, WJTouchEvent *event);
	void onDipperTouchEnded(Node *node, WJTouchEvent *event);

	// 装饰
	int m_tickIndex;
	bool checkBothChangeStickTexture();
	void onBeginStick();
	void changeStickTexture(int index, const char * file, int itemIndex);
	void onClickStickScrollItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	bool onScrollStickMovingCollision(Node *node, WJTouchEvent *e, WJScrollLayer *scroll);
	void onScrollStickCollisonEndedEvent(Node *node, WJTouchEvent *e, WJScrollLayer *scroll);

public:

	P012();
	virtual ~P012();

	virtual bool init() override;

	virtual void onEnterTransitionDidFinish() override;

public:

	CREATE_SCENE_FUNC(P012);
};

#endif // __P012_SUGAR_H__
