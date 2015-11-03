#include "P006Garland.h"
#include "ScrollItem.h"

INIT_SCENE_LOADER_SYNC(P006);

 bool P006::firstInto;

void P006::asyncLoadSceneResource()
{
	WJLayerJsonPreload::getInstance()->asyncPreload("game/json/P006.json");
}

bool P006::init()
{
	if (!PBase::init())
	{
		return false;
	}

	initSceneJson("game/json/P006.json");
	firstInto = true;

	initEvent();
	initScrollLayer();

	return true;
}

void P006::initScrollLayer()
{
	m_scrollLayer = ScrollLayer::create("P006_flower_", ScrollDragDir::DragLeft, 8, true, true);

	m_scrollLayer->setScrollItemDragOutCallBack(CC_CALLBACK_3(P006::onDragOut, this));
	m_scrollLayer->setClickScrollItemEvent(CC_CALLBACK_3(P006::onClickItem, this));
	m_scrollLayer->setScrollItemMovingCollisionEvent(CC_CALLBACK_3(P006::onMovingCollision, this));
	m_scrollLayer->setScrollItemCollisonEndedEvent(CC_CALLBACK_3(P006::onCollisionEnded, this));
	m_scrollLayer->initItemSelectFromIndex(0);

	SCROLL_POSITION_IN_JSON(m_scrollLayer);
	m_sceneJson->addChild(m_scrollLayer, ZORDER_PROGRESS);
}

void P006::initEvent()
{
	m_garland = GarlandLayer::create(m_sceneJson, this);
	addChild(m_garland, ZORDER_PROGRESS);

	m_staticCut = Scissor::create(m_sceneJson, m_garland);
	addChild(m_staticCut);

	m_garland->setScissor(m_staticCut);
}

GameNumber P006::getGameNumber()
{
	return GameNumber::P006;

}

void P006::onEnterTransitionDidFinish()
{
	PBase::onEnterTransitionDidFinish();

}

bool P006::onMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	//CCLOG("moving collision");
	int index = item->getItemIndex();

	WJSprite *sprite = item->getMovingSprite();
	WJSprite *flower = static_cast<WJSprite *>(sprite->getUserObject());

	switch (m_decoState)
	{
	case DecoState::FLOWER:
		MarkFlower* mark;
		if (mark = m_garland->isFlowerCutWithRect(flower))
		{
			item->setItemUserData((void*)mark);
			CCLOG("moving collision");
			if (index == mark->index)
			{
				return false;
			}
			return true;
		}

		break;
	case DecoState::DECO:

		break;
	default:
		break;
	}
	return false;
}

void P006::onCollisionEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	//CCLOG("collision end");
	int index = item->getItemIndex();

	WJSprite *sprite = item->getMovingSprite();
	WJSprite *flower = static_cast<WJSprite *>(sprite->getUserObject());

	flower->followNode(nullptr);
	sprite->removeFromParent();

	switch (m_decoState)
	{
	case DecoState::FLOWER:
		m_garland->changeFlower(flower, (MarkFlower *)item->getItemUserData(), index);
		
		break;
	case DecoState::DECO:

		break;
	default:
		break;
	}

}

void P006::onDragOut(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	//CCLOG("onDragOut");
	WJSprite *sprite = item->getMovingSprite();

	//if (item->getType() == DragoutType::WJSKELETON)
	//{
	//	WJSkeletonAnimation *newSke = static_cast<WJSkeletonAnimation *>(sprite->getUserObject());

	//	newSke->playAnimation("aniIdle", true);
	//}

}

void P006::onClickItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	//CCLOG("collision end");
	int index = item->getItemIndex();

}

void P006::onClickNextButton(Node *node, WJTouchEvent *event)
{
	PBase::onClickNextButton(node, event);

	if ((m_scrollLayer->getLayerStatus() == ScrollLayerStatus::showedStatus) || firstInto)
	{
		switch (m_decoState)
		{
		case DecoState::FLOWER:
			firstInto = false;

			m_scrollLayer->show(0.5f);
			m_garland->startplayCircleAni(3.f);
			hideNextButton();

			break;
		case DecoState::DECO:


			break;
		default:

			break;
		}
	}
}
