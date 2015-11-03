#ifndef __P008_NAIL_H__
#define __P008_NAIL_H__

#include "PBase.h"
#include "ScrollLayer.h"
#include "HandLayer.h"

USING_NS_CC;

class P008 : public PBase
{
protected:

	HandLayer *m_hand;
	ScrollLayer *m_scrollLayer;
	int m_choseWringTime;

	virtual GameNumber getGameNumber() override;
	void initHand();

	// 指甲的样式
	void initNailStyle();
	void onClickNailStyleScrollItem(Node *node, WJTouchEvent *e, WJScrollLayer *scroll);
	void onClickHandNail(Node *node, WJTouchEvent *e);

	bool onScrollStyleMovingCollision(Node *node, WJTouchEvent *e, WJScrollLayer *scroll);
	void onScrollStyleCollisonEndedEvent(Node *node, WJTouchEvent *e, WJScrollLayer *scroll);

	void onChooseWristCallBack(Node *node, WJTouchEvent *e);
	virtual void onClickNextButton(Node *node, WJTouchEvent *event) override;

public:
	P008();
	virtual ~P008();
	virtual bool init() override;

	virtual void onEnterTransitionDidFinish() override;
	// 设置当前心心的数量
	void setStarNumWhenMakeNail(ProgressStar star);

public:
	CREATE_SCENE_FUNC(P008);
};

#endif // __P008_NAIL_H__
