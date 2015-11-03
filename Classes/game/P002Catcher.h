#ifndef __P002_CATCHER_H__
#define __P002_CATCHER_H__

#include "ScrollLayer.h"
#include "ScrollItem.h"
#include "CircleLayer.h"

enum class CircleState
{
	ORIGIN,
	COLOR,
	DROP,
	DECO
};

class P002 : public PBase
{
protected:
	void initScrollLayer();
	void initEvent();

	void onClickLeftScrollItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll); // unused method

	bool onMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onCollisionEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onClickItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onDragOut(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	
	virtual void onClickNextButton(Node *node, WJTouchEvent *event) override;

protected:
	ScrollLayer *m_scrollLayer = nullptr;
	CircleLayer *m_circle = nullptr;
	CC_SYNTHESIZE(CircleState, m_circleState, CircleState);

protected:
	P002() :PBase(){
		m_circleState = CircleState::ORIGIN;
	}
	virtual ~P002(){}

	virtual bool init() override;
	virtual void onEnterTransitionDidFinish() override;
	virtual GameNumber getGameNumber() override;

public:
	CREATE_SCENE_FUNC(P002);
	static void asyncLoadSceneResource();

	friend class CircleLayer; // 为了能够使用show/hide nextbutton
};

#endif // __P002_CATCHER_H__
