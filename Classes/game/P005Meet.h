#ifndef __P005_MEET_H__
#define __P005_MEET_H__

#include "PBase.h"
#include "ScrollLayer.h"
#include "ScrollDoubleLayer.h"
#include "ModelBase.h"
#include "PrincessModel.h"
#include "GuestModel.h"
#include "ScrollDiyItem.h"

USING_NS_CC;


class P005 : public PBase
{
protected:

	int m_nowMaxZorder;
	ModelBase* m_charas[7];

	PrincessModel *m_princess;

	ScrollDoubleLayer *m_scrollDoubleLayer;

	virtual GameNumber getGameNumber() override;

	void initEvent();
	void initCharas();
	void initZorder();

	bool onScrollLayerMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onClickMenuScrollItemCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onClickScrollItemCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollItemCollisonEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollItemDragOutCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	Node* onScrollDragOutInitNodeCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollItemTouchEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onFailedSelectItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	// 捕梦网相关
	void changeDreamCatcher(ScrollDiyItem *item);

	// 糖球相关
	void changeSugar(ScrollDiyItem *item);

	// 添加人物相关
	bool onPeopleScrollLayerMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void clickDiyMenuItemCallBack(ScrollMenuItem* item, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void replaceItemToDiyItem(ScrollMenuItem* item, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onModelTouchEndCallBack(Node *node, WJTouchEvent *event);
	void onClickModelScrollItem(ScrollDiyItem* item);
	float findEmptyMostPointFromStayModel(const Vec2 &point);
	float getKerelnFromDistance(float distance, float import = 1.0f);
	int getModelNumIntoScene();

	void putModelIntoScene(ModelBase *base, ScrollDiyItem *item);
	void removeModelFromScene(ModelBase *base, ScrollDiyItem *item);

public:

	P005();
	virtual ~P005();

	virtual bool init() override;

	virtual void onEnterTransitionDidFinish() override;

	virtual void onExit() override;

	static void asyncLoadSceneResource();

public:

	CREATE_SCENE_FUNC(P005);
};

#endif // __P005_MEET_H__
