#include "P012Show.h"
#include "ScrollItem.h"

INIT_SCENE_LOADER_SYNC(P012_show);

// 一共为左侧item个数 + 2 个
static bool P012ShowScrollIsFirstRun[5] = {
	true, true, true, true, true
};

P012_show::P012_show()
{
	m_ballIndex = 0;
	m_changeState = changeState::changeBall;
	m_globalZorderMax = 1000;
}

P012_show::~P012_show()
{

}

bool P012_show::init()
{
	if ( !PBase::init() )
	{
		return false;
	}

	initSceneJson("game/json/p012_show.json");

	initEvent();

	setProgressStarNum(ProgressStar::twoStar, false);

	showNextButton();

	executeCommand();

	return true;
}


GameNumber P012_show::getGameNumber()
{
	return GameNumber::P012_show;
}

void P012_show::onEnterTransitionDidFinish()
{
	PBase::onEnterTransitionDidFinish();
	m_scrollDoubleLayer->show(1.0f);
}

void P012_show::initEvent()
{
	// 选择弹窗
	m_scrollDoubleLayer = ScrollDoubleLayer::create("P012_left_", ScrollDragDir::StopDrag, 3, true, P012ShowScrollIsFirstRun);
	m_scrollDoubleLayer->setScrollItemMovingCollisionEvent(CC_CALLBACK_3(P012_show::onScrollLayerMovingCollision, this));
	m_scrollDoubleLayer->setAutoDealCancel(AUTO_DEAL_CANCEL_START_INDEX, 1, AUTO_DEAL_CANCEL_END_INDEX);
	m_scrollDoubleLayer->setStackMaxNum(50);
	m_scrollDoubleLayer->setClickMenuScrollItemEvent(CC_CALLBACK_3(P012_show::onClickLeftScrollItemCallBack, this));
	m_scrollDoubleLayer->setClickScrollItemEvent(CC_CALLBACK_3(P012_show::onClickScrollItemCallBack, this));
	m_scrollDoubleLayer->setScrollItemCollisonEndedEvent(CC_CALLBACK_3(P012_show::onScrollItemCollisonEnded, this));
	SCROLL_POSITION_IN_JSON(m_scrollDoubleLayer);
	this->addChild(m_scrollDoubleLayer, 100);

	// 糖球layer
	WJLayer *layer;
	for (int i = 1; i <= 5; i++)
	{
		 layer = m_sceneJson->getSubLayer(WJUtils::stringAddInt("", i, 3).c_str());
		 layer->saveCurrentProperties();
	}
}

void P012_show::onClickLeftScrollItemCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	int index = item->getItemIndex();
	if (index == 0)
		m_changeState = changeState::changeBall;
	else if (index == 1)
		m_changeState = changeState::changDecor;
	else
		m_changeState = changeState::changeBox;
}

void P012_show::onClickScrollItemCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	if (m_changeState == changeState::changeBall)
		onClickBallScrollItem(node, touchEvent, scroll);
	else if (m_changeState == changeState::changDecor)
		onClickDecorScrollItem(node, touchEvent, scroll);
	else if (m_changeState == changeState::changeBox)
		onClickBoxScrollItem(node, touchEvent, scroll);
}

void P012_show::onClickBallScrollItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	m_ballIndex++;
	if (m_ballIndex >= 6)
		m_ballIndex = 1;
	
	WJSprite *sprite = m_sceneJson->getSubSprite(WJUtils::stringAddInt("b", m_ballIndex, 3).c_str());
	sprite->loadSpriteTexture(item->getItemFilePath().c_str());
	playBallLayerAni(m_sceneJson->getSubLayer(WJUtils::stringAddInt("", m_ballIndex, 3).c_str()));
}

void P012_show::onClickBoxScrollItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	int index = item->getItemIndex();
	m_sceneJson->getSubSprite("under")->loadSpriteTexture(item->getItemFilePath().c_str());
	if (index == 3)
	{
		m_sceneJson->getSubSprite("under_s")->loadSpriteTexture("base/none.png");
	}
	else
	{
		m_sceneJson->getSubSprite("under_s")->loadSpriteTexture(item->getItemFilePath().c_str());
		m_sceneJson->getSubSprite("under_s")->loadSpriteTextureAddSuffix("_", "s");
	}

	WJLayer *layer;
	for (int i = 1; i <= 5; i++)
	{
		layer = m_sceneJson->getSubLayer(WJUtils::stringAddInt("", i, 3).c_str());
		if (!layer->getActionByTag(NODE_RUN_WAVE_ACTION_TAG))
		{
			float time = 0.15f;
			layer->stopAllActions();
			layer->restoreSavedRotation(0.2f);
			Action *action = Sequence::create(
				DelayTime::create(i * 0.2f),
				RotateBy::create(time, 10),
				RotateBy::create(time, -10),
				RotateBy::create(time, -10),
				RotateBy::create(time, 10),
				NULL);
			layer->runAction(action);
		}
	}
}

void P012_show::onClickDecorScrollItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;

	WJSprite *newSprite = WJSprite::create(item->getItemFilePath().c_str());
	newSprite->setMoveAble(true);
	newSprite->noClickMoveEffect();
	newSprite->setScale(1.0f);
	newSprite->setOnTouchAble(CC_CALLBACK_2(P012_show::onDecorSpriteTouchAble, this));
	newSprite->setOnTouchEnded(CC_CALLBACK_2(P012_show::onDecorSpriteTouchEnded, this));
	// 随机一个层添加装饰品
	int randomInt = WJUtils::randomInt(5) + 1;
	WJLayer *layer = m_sceneJson->getSubLayer(WJUtils::stringAddInt("", randomInt, 3).c_str());
	WJLayer *checklayer= m_sceneJson->getSubLayer(WJUtils::stringAddInt("l", randomInt, 3).c_str());
	const Size &size = checklayer->getContentSize();

	Vec2 &toPoint = Vec2(checklayer->getPositionX(), checklayer->getPositionY());
	Rect rect = Rect(0, 0, size.width, size.height);
	do
	{
		toPoint = Vec2(WJUtils::randomInt(size.width) + checklayer->getPositionX(),
			WJUtils::randomInt(size.height) + checklayer->getPositionY());
		Point point = checklayer->convertToNodeSpace(layer->convertToWorldSpace(toPoint));
		if (rect.containsPoint(point))
			break;
	} while (1);

	newSprite->setUserTag(randomInt);
	newSprite->setPosition(toPoint);
	layer->addChild(newSprite, 100);
	newSprite->saveCurrentProperties();

	m_scrollDoubleLayer->addNodeIntoStack(newSprite);

	playBallLayerAni(layer);
}

void P012_show::playBallLayerAni(WJLayer *layer)
{
	// 帮帮球摆动
	PUtils::playNodeWaveEffect(layer);
}

bool P012_show::onDecorSpriteTouchAble(Node *node, WJTouchEvent *event)
{
	WJSprite *sprite = (WJSprite*)node;
	sprite->setGlobalZOrder(5000);

	return true;
}

void P012_show::onDecorSpriteTouchEnded(Node *node, WJTouchEvent *event)
{
	WJSprite *sprite = (WJSprite*)node;
	sprite->restoreSavedGlobalZOrder();

	int index = checkDecorSpritePoint(node);
	if (index != -1)
	{
		WJLayer *layer = m_sceneJson->getSubLayer(WJUtils::stringAddInt("", index, 3).c_str());
		Vec2 &point = layer->convertToNodeSpace(sprite->getPositionWorld());
		if (sprite->getUserTag() != index)
		{
			playBallLayerAni(layer);
		}

		sprite->retain();
		sprite->removeFromParentAndCleanup(true);
		sprite->setPosition(point);
		layer->addChild(sprite, 100);
		sprite->release();
		sprite->setUserTag(index);
		sprite->saveCurrentProperties();
	}
	else
	{
		// 装饰物掉落
		Vec2 &topoint = sprite->getParent()->convertToNodeSpace(Vec2(sprite->getPositionWorld().x, -200));
		sprite->setMoveAble(false);
		sprite->stopAllActions();
		sprite->runAction(Sequence::create(
			EaseBackOut::create(MoveTo::create(1.0f, topoint)),
			DelayTime::create(0.3f),
			RemoveSelf::create(),
			NULL));
		m_scrollDoubleLayer->deleteNodeFromInStack(sprite);
	}
}

int P012_show::checkDecorSpritePoint(Node *node)
{
	WJSprite *sprite = (WJSprite*)node;
	for (int i = 1; i <= 5; i++)
	{
		WJLayer *checklayer = m_sceneJson->getSubLayer(WJUtils::stringAddInt("l", i, 3).c_str());
		const Size &size = checklayer->getContentSize();
		Rect rect = Rect(0, 0, size.width, size.height);
		if (rect.containsPoint(checklayer->convertToNodeSpace(sprite->getPositionWorld())))
		{
			return i;
		}
	}
	return -1;
}

bool P012_show::onScrollLayerMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	if (m_changeState == changeState::changeBall)
		return onScrollBallMovingCollision(node, touchEvent, scroll);
	else if (m_changeState == changeState::changDecor)
		return onScrollDecorMovingCollision(node, touchEvent, scroll);

	return false;
}

bool P012_show::onScrollBallMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	WJSprite *sprite;
	for (int i = 1; i <= 5; i++)
	{
		sprite = m_sceneJson->getSubSprite(WJUtils::stringAddInt("b", i, 3).c_str());
		if (PUtils::isOneCollideWithOne(item->getMovingSprite()->getPositionWorld(), sprite, false))
		{
			item->setItemUserData((void*)i);
			return true;
		}
	}

	return false;
}

bool P012_show::onScrollDecorMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	WJSprite *sprite = (WJSprite*)item->getMovingSprite()->getUserObject();
	for (int i = 1; i <= 5; i++)
	{
		WJLayer *checklayer = m_sceneJson->getSubLayer(WJUtils::stringAddInt("l", i, 3).c_str());
		const Size &size = checklayer->getContentSize();
		Rect rect = Rect(0, 0, size.width, size.height);
		if (rect.containsPoint(checklayer->convertToNodeSpace(sprite->getPositionWorld())))
		{
			item->setItemUserData((void*)i);
			return true;
		}
	}

	return false;
}

void P012_show::onScrollItemCollisonEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	if (m_changeState == changeState::changeBall)
		onScrollBallCollisonEndedEvent(node, touchEvent, scroll);
	else if (m_changeState == changeState::changDecor)
		onScrollDecorCollisonEndedEvent(node, touchEvent, scroll);
}

void P012_show::onScrollBallCollisonEndedEvent(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = dynamic_cast<ScrollItem*>(node);
	int index = (int)item->getItemUserData();
	WJSprite *basicSprite = m_sceneJson->getSubSprite(WJUtils::stringAddInt("b", index, 3).c_str());
	basicSprite->loadSpriteTexture(item->getItemFilePath().c_str());
	playBallLayerAni(m_sceneJson->getSubLayer(WJUtils::stringAddInt("", index, 3).c_str()));

	// 移除掉拖动的糖球棒
	WJSprite *sprite = item->getMovingSprite();
	WJSprite* newSprite = dynamic_cast<WJSprite*>(sprite->getUserObject());
	newSprite->followNode(nullptr);
	sprite->removeFromParentAndCleanup(true);
	sprite = nullptr;
	newSprite->removeFromParentAndCleanup(true);
	newSprite = nullptr;
}

void P012_show::onScrollDecorCollisonEndedEvent(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = dynamic_cast<ScrollItem*>(node);
	int index = (int)item->getItemUserData();

	WJLayer *layer = m_sceneJson->getSubLayer(WJUtils::stringAddInt("", index, 3).c_str());

	WJSprite *sprite = item->getMovingSprite();
	WJSprite* newSprite = dynamic_cast<WJSprite*>(sprite->getUserObject());
	newSprite->followNode(nullptr);
	sprite->removeFromParentAndCleanup(true);
	sprite = nullptr;
	
	Vec2 &point = layer->convertToNodeSpace(newSprite->getPositionWorld());
	newSprite->retain();
	newSprite->removeFromParentAndCleanup(false);
	newSprite->setPosition(point);
	newSprite->setMoveAble(true);
	newSprite->setScale(1.0f);
	newSprite->setOnTouchAble(CC_CALLBACK_2(P012_show::onDecorSpriteTouchAble, this));
	newSprite->setOnTouchEnded(CC_CALLBACK_2(P012_show::onDecorSpriteTouchEnded, this));
	newSprite->noClickMoveEffect();
	layer->addChild(newSprite, 100);
	newSprite->release();
	newSprite->setUserTag(index);
	newSprite->saveCurrentProperties();

	m_scrollDoubleLayer->addNodeIntoStack(newSprite);
	playBallLayerAni(layer);
}

void P012_show::onClickNextButton(Node *node, WJTouchEvent *event)
{
	PBase::onClickNextButton(node, event);
	showStarDialog(DiySaveType::P012_Sugar, m_sceneJson->getSubLayer("showpanel"));
}
