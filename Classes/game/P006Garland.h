#ifndef __P006_GARLAND_H__
#define __P006_GARLAND_H__

#include "ScrollLayer.h"
#include "ScrollItem.h"
#include "GarlandLayer.h"
#include "Scissor.h"

USING_NS_CC;

enum class DecoState
{
	FLOWER,
	DECO
};

class P006 : public PBase
{
protected:
	void initScrollLayer();
	void initEvent();

	bool onMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onClickItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onDragOut(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onCollisionEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	virtual void onClickNextButton(Node *node, WJTouchEvent *event) override;

protected:
	ScrollLayer *m_scrollLayer = nullptr;
	GarlandLayer *m_garland = nullptr; 	// 花环
	Scissor *m_staticCut = nullptr; 		// 剪刀 

	DecoState m_decoState = DecoState::FLOWER;
	static bool firstInto; 

protected:
	P006():PBase(){}
	virtual ~P006(){}

	virtual bool init() override;
	virtual void onEnterTransitionDidFinish() override;
	virtual GameNumber getGameNumber() override;

public:
	CREATE_SCENE_FUNC(P006);
	static void asyncLoadSceneResource();

	friend class GarlandLayer; // 为了能够使用show/hide nextbutton

};

#endif // __P006_GARLAND_H__
