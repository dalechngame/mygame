#ifndef __P011_DRAW_H__
#define __P011_DRAW_H__

#include "PBase.h"
#include "ScrollLayer.h"
#include "DrawLayer.h"
#include "ScrollItem.h"

USING_NS_CC;

enum class DrawLayerState
{
	drawingState,
	decorState
};

class P011_Draw : public PBase
{
protected:

	virtual GameNumber getGameNumber() override;

	ScrollLayer *m_scrollLayer;
	Vector<DrawLayer*> m_drawLayerArray;
	WJSprite *m_earseSprite;
	DrawLayerState m_drawState;

	void initScrollLayer();
	void initDrawLayer();

	bool onScrollLayerTouchAble(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	bool onScrollLayerMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollLayerTouchEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollItemDragOutCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollItemCollisonEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	bool onEarseTouchAble(Node *node, WJTouchEvent *event);
	bool onEarseWillMoveTo(Node *node, WJTouchEvent *event);
	void onEarseTouchEnded(Node *node, WJTouchEvent *event);

public:

	P011_Draw();
	virtual ~P011_Draw();

	virtual bool init() override;

	virtual void onEnterTransitionDidFinish() override;

public:
	CREATE_SCENE_FUNC(P011_Draw);
};

#endif // __P011_DRAW_H__
