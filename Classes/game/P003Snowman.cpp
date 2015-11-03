#include "P003Snowman.h"

INIT_SCENE_LOADER_SYNC(P003);

static bool sideScrollIsFirstRun[6] = {
	true, true, true, true, true, true
};

void P003::asyncLoadSceneResource()
{
	WJLayerJsonPreload::getInstance()->asyncPreload("game/json/P003.json");
}

bool P003::init()
{
	if (!PBase::init())
	{
		return false;
	}

	initSceneJson("game/json/P003.json");

	initEvent();
	initScrollLayer();

	return true;
}

void P003::initScrollLayer()
{
	m_scrollDoubleLayer = ScrollDoubleLayer::create("P002_left_", ScrollDragDir::DragLeft, 4, true, sideScrollIsFirstRun);

	m_scrollDoubleLayer->setClickScrollItemEvent(CC_CALLBACK_3(P003::onClickItem, this));
	m_scrollDoubleLayer->setScrollItemDragOutCallBack(CC_CALLBACK_3(P003::onDragOut, this));
	m_scrollDoubleLayer->setScrollItemMovingCollisionEvent(CC_CALLBACK_3(P003::onMovingCollision, this));
	m_scrollDoubleLayer->setScrollItemCollisonEndedEvent(CC_CALLBACK_3(P003::onCollisionEnded, this));
	m_scrollDoubleLayer->initItemSelectFromIndex(0);

	SCROLL_POSITION_IN_JSON(m_scrollDoubleLayer);
	m_sceneJson->addChild(m_scrollDoubleLayer, ZORDER_PROGRESS);
}

void P003::initEvent()
{
	m_snowman = Snowman::create(m_sceneJson, this);
	addChild(m_snowman);

	m_tool = Tools::create(m_sceneJson, m_snowman);
	addChild(m_tool);
}

GameNumber P003::getGameNumber()
{
	return GameNumber::P003;
}

void P003::onEnterTransitionDidFinish()
{
	PBase::onEnterTransitionDidFinish();

}

void P003::onClickLeftScrollItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	//ScrollItem *item = (ScrollItem*)node;
	//int index = item->getItemIndex();
	////CCLOG("click left scrollitem %d", index);

	//m_circleState = (CircleState)index;

	//if (m_circleState != CircleState::DROP)
	//{
	//	m_circle->stopPlayCircleAni();
	//}
	//if (m_circleState == CircleState::COLOR)
	//{
	//	CCLOG("%d", m_circle->getCurrentIndex());
	//	if (m_circle->getCurrentIndex() == 0)
	//	{
	//		m_scrollDoubleLayer->changeLeftScrollItem("P003_left_002", item);
	//	}
	//	else
	//	{
	//		m_scrollDoubleLayer->changeLeftScrollItem(WJUtils::stringAddInt("P003_leftcolor_", m_circle->getCurrentIndex(), 3), item);
	//	}
	//}
	//else if (m_circleState == CircleState::DROP)
	//{
	//	m_circle->startPlayCircleAni(1.5f);
	//}
}


bool P003::onMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	//CCLOG("moving collision");

	WJSprite *sprite = item->getMovingSprite();

	return false;
}

void P003::onCollisionEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	//CCLOG("collision end");
	int index = item->getItemIndex();

	//WJSprite *sprite = item->getMovingSprite();
	//WJSkeletonAnimation *newSke = static_cast<WJSkeletonAnimation *>(sprite->getUserObject());

	//newSke->followNode(nullptr);
	//sprite->removeFromParent();


}

void P003::onDragOut(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	//CCLOG("onDragOut");
	WJSprite *sprite = item->getMovingSprite();

	/*if (item->getType() == DragoutType::WJSKELETON)
	{
	WJSkeletonAnimation *newSke = static_cast<WJSkeletonAnimation *>(sprite->getUserObject());

	newSke->playAnimation("aniIdle", true);
	}*/

}

void P003::onClickItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	//CCLOG("collision end");
	int index = item->getItemIndex();
	WJSprite *sprite = item->getMovingSprite();
	Node * newSprite = item->getItem(sprite);

}

void P003::onClickNextButton(Node *node, WJTouchEvent *event)
{
	PBase::onClickNextButton(node, event);

}
