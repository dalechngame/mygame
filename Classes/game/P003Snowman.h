#ifndef __P003_SNOWMAN_H__
#define __P003_SNOWMAN_H__

#include "ScrollDoubleLayer.h"
#include "Snowman.h"
#include "Tools.h"

class P003 : public PBase
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
	ScrollDoubleLayer *m_scrollDoubleLayer = nullptr;
	Snowman *m_snowman = nullptr;
	Tools * m_tool = nullptr;

protected:
	P003():PBase(){}
	virtual ~P003(){}

	virtual bool init() override;
	virtual void onEnterTransitionDidFinish() override;
	virtual GameNumber getGameNumber() override;

public:
	CREATE_SCENE_FUNC(P003);
	static void asyncLoadSceneResource();

};

#endif // __P003_SNOWMAN_H__
