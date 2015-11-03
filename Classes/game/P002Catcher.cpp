#include "P002Catcher.h"

INIT_SCENE_LOADER_SYNC(P002);

void P002::asyncLoadSceneResource()
{
	WJLayerJsonPreload::getInstance()->asyncPreload("game/json/p002.json");
}

bool P002::init()
{
	if (!PBase::init())
	{
		return false;
	}

	initSceneJson("game/json/p002.json");

	initEvent();
	initScrollLayer();

	return true;
}

void P002::initScrollLayer()
{
	m_scrollLayer = ScrollLayer::create("P002_circle_", ScrollDragDir::DragLeft, 8, true, true);

	m_scrollLayer->setClickScrollItemEvent(CC_CALLBACK_3(P002::onClickItem, this));
	m_scrollLayer->setScrollItemDragOutCallBack(CC_CALLBACK_3(P002::onDragOut, this));
	m_scrollLayer->setScrollItemMovingCollisionEvent(CC_CALLBACK_3(P002::onMovingCollision, this));
	m_scrollLayer->setScrollItemCollisonEndedEvent(CC_CALLBACK_3(P002::onCollisionEnded, this));
	m_scrollLayer->initItemSelectFromIndex(0);

	SCROLL_POSITION_IN_JSON(m_scrollLayer);
	m_sceneJson->addChild(m_scrollLayer, ZORDER_PROGRESS);
}

void P002::initEvent()
{
	m_circle = CircleLayer::create(m_sceneJson, true, this);
	addChild(m_circle, ZORDER_PROGRESS);

}

GameNumber P002::getGameNumber()
{
	return GameNumber::P002;

}

void P002::onEnterTransitionDidFinish()
{
	PBase::onEnterTransitionDidFinish();

	m_scrollLayer->show(0.5f);
	showNextButton(false);
}

void P002::onClickLeftScrollItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
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
	//		m_scrollDoubleLayer->changeLeftScrollItem("P002_left_002", item);
	//	}
	//	else
	//	{
	//		m_scrollDoubleLayer->changeLeftScrollItem(WJUtils::stringAddInt("P002_leftcolor_", m_circle->getCurrentIndex(), 3), item);
	//	}
	//}
	//else if (m_circleState == CircleState::DROP)
	//{
	//	m_circle->startPlayCircleAni(1.5f);
	//}
}


bool P002::onMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	//CCLOG("moving collision");

	WJSprite *sprite = item->getMovingSprite();
	WJSkeletonAnimation *newSke = static_cast<WJSkeletonAnimation *>(sprite->getUserObject());
	int index = item->getItemIndex();

	switch (m_circleState)
	{
	case CircleState::ORIGIN:
		if (PUtils::isOneCollideWithOne(m_circle->getCurrentCircle(), sprite) &&
			(m_circle->getCurrentIndex() != index || m_circle->getCurrentColor() != m_circle->getOriginColor()))
		{
			return true;
		}

		break;
	case CircleState::COLOR:
		if (PUtils::isOneCollideWithOne(m_circle->getCurrentCircle(), sprite) && m_circle->getCurrentColor() != index)
		{
			return true;
		}

		break;
	case CircleState::DROP:

		Mark*mark;
		if (mark = m_circle->isCollided(newSke))
		{
			item->setItemUserData((void*)mark);

			if (index == mark->index)
			{
				return false;
			}
			return true;
		}

		break;
	case CircleState::DECO:
		if (PUtils::isOneCollideWithOne(m_circle->getCurrentCircle(), sprite))
		{
			return true;
		}
		break;

	default:
		break;
	}

	return false;
}

void P002::onCollisionEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	//CCLOG("collision end");
	int index = item->getItemIndex();

	WJSprite *sprite = item->getMovingSprite();
	WJBase *base = nullptr;
	if (item->getType() == DragoutType::WJSKELETON)
	{
		base = static_cast<WJSkeletonAnimation *>(sprite->getUserObject());
		base->followNode(nullptr);
	}
	else if (item->getType() == DragoutType::WJSPRITE)
	{
		base = static_cast<WJSprite *>(sprite->getUserObject());
		base->followNode(nullptr);
	}
	Node *newnode = dynamic_cast<Node*>(base);
	sprite->removeFromParent();

	switch (m_circleState)
	{
	case CircleState::ORIGIN:
		m_circle->changeCircle(index, newnode);

		break;
	case CircleState::COLOR:
		m_circle->changeColor(index, newnode);
		break;

	case CircleState::DROP:
		m_circle->changeDrop(newnode, (Mark*)(item->getItemUserData()), index);

		break;
	case CircleState::DECO:
		m_circle->changeDeco(index, newnode);
		break;

	default:
		break;

	}

}

void P002::onDragOut(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	//CCLOG("onDragOut");
	WJSprite *sprite = item->getMovingSprite();

	if (item->getType() == DragoutType::WJSKELETON)
	{
		WJSkeletonAnimation *newSke = static_cast<WJSkeletonAnimation *>(sprite->getUserObject());

		newSke->playAnimation("aniIdle", true);
	}

}

void P002::onClickItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	//CCLOG("collision end");
	int index = item->getItemIndex();
	WJSprite *sprite = item->getMovingSprite();
	Node * newSprite = item->getItem(sprite);

	switch (m_circleState)
	{
	case CircleState::ORIGIN:
		if (m_circle->getCurrentIndex() != index || m_circle->getCurrentColor() != m_circle->getOriginColor())
		{
			m_circle->changeCircle(index, newSprite);
		}

		break;
	case CircleState::COLOR:
		if (m_circle->getCurrentColor() != index)
		{
			m_circle->changeColor(index, newSprite);
		}
		break;

	case CircleState::DROP:
		m_circle->changeDrop(newSprite, m_circle->getRandomMark(index), index);

		break;
	case CircleState::DECO:

		break;

	default:
		break;

	}

}

void P002::onClickNextButton(Node *node, WJTouchEvent *event)
{
	PBase::onClickNextButton(node, event);

	if (!(m_scrollLayer->getLayerStatus() == ScrollLayerStatus::showedStatus))
	{
		return;
	}
	int index = m_circle->getCurrentIndex();
	switch (m_circleState)
	{
	case CircleState::ORIGIN:
		m_circleState = CircleState::COLOR;

		if (index == 0)
		{
			m_scrollLayer->reInit("P002_color_", ScrollDragDir::DragLeft, 3, true, true);
		}
		else
		{
			m_scrollLayer->reInit(WJUtils::stringAddInt("P002_color", index, 1).append("_"), ScrollDragDir::DragLeft, 3, true, true);
		}

		break;
	case CircleState::COLOR:
		m_circleState = CircleState::DROP;
		m_scrollLayer->reInit("P002_drop_", ScrollDragDir::DragLeft, 10, true, true);

		m_circle->startPlayCircleAni(1.5f);
		//hideNextButton();

		break;

	case CircleState::DROP:
		m_circleState = CircleState::DECO;
		m_scrollLayer->reInit("P002_deco_", ScrollDragDir::DragLeft, 10, true, true);

		//hideNextButton();

		break;
	case CircleState::DECO:
		showStarDialog(DiySaveType::P002_Dreamcatcher, m_circle->getCurrentLayer());
		break;

	}

}
