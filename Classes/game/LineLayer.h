#ifndef __LINE_LAYER_H__
#define __LINE_LAYER_H__

#include "cocos2d.h"
#include "Common.h"
#include "PUtils.h"

USING_NS_CC;


class LineLayer : public WJLayer
{
private:
	// 是否可以绘制
	CC_SYNTHESIZE(bool, m_isDrawedAble, IsDrawedAble);
	// 是否完成了
	CC_SYNTHESIZE(bool, m_isCompleted, IsCompleted);
	// 绘制的起始点
	Vec2 m_startpoint;
	Vec2 m_endpoint;
	float m_endScaleY;
	// 当前线段的下一个
	CC_SYNTHESIZE(LineLayer*, m_nextLine, NextLine);
	// 第二个星星
	CC_SYNTHESIZE(WJSprite*, m_star2, Star2);

	CC_SYNTHESIZE(WJSprite*, m_lineBg, LineBg);

	WJSprite *m_star1;

	WJSprite *m_lineSprite;
	WJSprite *m_lineHeadSprite;

	float getLineScaleYFromTouch(Node *node, WJTouchEvent *event);
	void runActionLineScaleY(float toScaleY, float wholeTime);

	
	bool onLineLayerTouchAble(Node *node, WJTouchEvent *event);
	bool onLineLayerWillMoveTo(Node *node, WJTouchEvent *event);
	void onLineLayerTouchEnded(Node *node, WJTouchEvent *event);

	void onLineMoveEndPoint();
	void onClickEndStar(Node *node, WJTouchEvent *event);

public:

	LineLayer();
	~LineLayer();

	static LineLayer* create(WJSprite *star1, WJSprite *star2);
	virtual bool init(WJSprite *star1, WJSprite *star2);

	virtual void onEnterTransitionDidFinish() override;

	virtual void onEnter() override;
	virtual void onExit() override;

	virtual void update(float delta) override;

};

#endif // __HAND_LAYER_H__
