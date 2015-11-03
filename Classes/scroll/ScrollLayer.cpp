// Created by wangwenjie on 2014/01

#include "ScrollLayer.h"
#include "Common.h"
#include "LBModalDialogLayer.h"
#include "ScrollItem.h"
#include "ScrollItemTemplate.h"

USING_NS_CC;

#define SCROLL_LAYER_MOVE_OUT_SIDE_TAG                5596      // 滚动的控件移动到屏幕外面去了tag

ScrollLayer * ScrollLayer::create(const char *key, ScrollDragDir drag, int count, bool clickAble, bool firstInto)
{
	ScrollLayer *layer = new (std::nothrow) ScrollLayer();
    layer->init(key, drag, count, clickAble, firstInto);
    layer->autorelease();

    return layer;
}

bool ScrollLayer::init(const char * key, ScrollDragDir drag, int count, bool clickAble, bool firstInto)
{ 
    if (!WJLayer::init())
        return false;

	initBgSprite();
	initScrollLayer(key, drag, count, clickAble, firstInto);

    return true;
}

void ScrollLayer::initBgSprite()
{
	m_scrollBg = WJSprite::create("game/03_minigame/common/ui/frame003.png");
	const Size &size = m_scrollBg->getContentSize();
	m_scrollBg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(m_scrollBg, 0);

	// 控制显示和隐藏的
	m_switchSprite = WJSprite::create("game/03_minigame/common/ui/down.png");
	m_switchSprite->setPosition(Vec2(-5, 420));
	m_switchSprite->noClickMoveEffect();
	m_switchSprite->setClickAble(true);
	m_switchSprite->setOnClick(CC_CALLBACK_2(ScrollLayer::onClickSwitchSprite, this));
	this->addChild(m_switchSprite, 200);

	this->setContentSize(size);
	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->ignoreAnchorPointForPosition(false);
	this->setPosition(Vec2(size.width + 200, size.height / 2 - 80));
}

void ScrollLayer::initScrollLayer(const char * key, ScrollDragDir drag, int count, bool clickAble, bool firstInto)
{
	if (m_scrollLayer)
	{
		m_scrollLayer->removeFromParentAndCleanup(true);
		m_scrollLayer = NULL;
	}

	m_firstInto = firstInto;
	const Size &size = this->getContentSize();
	cocos2d::Vector<WJScrollItem*> items;
	items.clear();
	creatScrollItem(items, count, key, drag, clickAble);

	m_scrollLayer = WJVScrollLayer::create();
	m_scrollLayer->pageScrollMode = false;
	m_scrollLayer->initWithItems(items, Rect(0, 35, size.width, size.height - 70), NULL, false,
		NULL, Color3B::WHITE);
	m_scrollLayer->setPosition(Vec2(size.width / 2, size.height / 2));
	m_scrollBg->addChild(m_scrollLayer, 50);

	// 第一次进入
	if (m_firstInto)
	{
		m_scrollLayer->setEnabled(false);
		m_scrollLayer->setPositionY(m_scrollLayer->getMaxPosition());
	}

	// 重新设置背景的锚点
	WJUtils::setNodeAnchorPoint(m_scrollBg, Vec2(1.0f, 0.5f));
	m_scrollBg->saveCurrentProperties();
}

void ScrollLayer::creatScrollItem(cocos2d::Vector<WJScrollItem*> &items, int count, const char *key, ScrollDragDir drag, bool clickAble)
{
	m_vectorItem.clear();
	for (int i = 0; i < count; i++)
	{
		ScrollItem *scrollItem = ScrollItem::create(this, key, i, drag, clickAble);
		scrollItem->setScrollItemTouchAbleEvent(CC_CALLBACK_3(ScrollLayer::doEventScrollItemTouchAble, this));
		scrollItem->setScrollItemMovingCollisionEvent(CC_CALLBACK_3(ScrollLayer::doEventScrollItemMovingCollision, this));
		scrollItem->setScrollItemDragOutCallBack(CC_CALLBACK_3(ScrollLayer::doEventScrollItemDragOutCallBack, this));
		scrollItem->setScrollItemCollisonEndedEvent(CC_CALLBACK_3(ScrollLayer::doEventScrollItemCollisonEnded, this));
		scrollItem->setFailedSelectItemEvent(CC_CALLBACK_3(ScrollLayer::doEventFailedScrollItem, this));
		scrollItem->setClickScrollItemEvent(CC_CALLBACK_3(ScrollLayer::doEventScrollItemClick, this));
		scrollItem->setSelectScrollItemEvent(CC_CALLBACK_3(ScrollLayer::doEventSelectScrollItem, this));
		scrollItem->setClickCancelItemEvent(CC_CALLBACK_3(ScrollLayer::doEventClickCancelItem, this));
		scrollItem->setScrollItemTouchEndedEvent(CC_CALLBACK_3(ScrollLayer::doEventScrollItemTouchEnded, this));
		scrollItem->setScrollDragOutInitNodeCallBack(CC_CALLBACK_3(ScrollLayer::doEventScrollDragOutInitNodeCallBack, this));

		m_vectorItem.pushBack(scrollItem);
		std::string itemKey = key;
		WJUtils::stringAddInt(itemKey, i + 1, 3);
		ScrollItemTemplate object = ScrollItemTemplateTem(itemKey);
		WJScrollItem* item = WJScrollItem::create(scrollItem, 0, scrollItem->getContentSize().height + object.itemsInterval);
		items.pushBack(item);
		
		// 小设备的添加物件拖出来后的偏移量
		if (WJUtils::isSmallScreen())
			scrollItem->setOffsetWhenDragOut(Vec2(object.offsetX, object.offsetY));
	}
}


ScrollLayer::ScrollLayer() : WJLayer(), ScrollBase()
{
	m_scrollLayer = NULL;
	m_layerStatus = ScrollLayerStatus::hidedStatus;
	m_switchSprite = nullptr;
}

ScrollLayer::~ScrollLayer()
{

}

void ScrollLayer::onEnterTransitionDidFinish()
{
	WJLayer::onEnterTransitionDidFinish();
}

void ScrollLayer::onEnter()
{
	WJLayer::onEnter();
}

void ScrollLayer::onExit()
{
	WJLayer::onExit();
}

WJLayerJson* ScrollLayer::getSceneJson()
{
	return (WJLayerJson*)this->getParent();
}

void ScrollLayer::show(float delay, float rightLength)
{
	if (m_layerStatus == ScrollLayerStatus::hidedStatus)
	{
		resetSwitchSpritePicture();
		const Size &size = Director::getInstance()->getWinSize();
		this->setPosition(this->getParent()->convertToNodeSpace(Vec2(size.width + 300, size.height / 2)));
		this->saveCurrentPosition();

		if (m_firstInto)
			m_scrollLayer->setPositionY(m_scrollLayer->getMaxPosition());

		this->stopAllActions();
		this->runAction(Sequence::create(
			DelayTime::create(delay),
			CCCallFunc::create(CC_CALLBACK_0(ScrollLayer::beginShowLayer, this)),
			EaseBackOut::create(
			MoveTo::create(0.5f, this->getParent()->convertToNodeSpace(Vec2(size.width - rightLength, size.height / 2)))),
			CCCallFunc::create(CC_CALLBACK_0(ScrollLayer::showEnded, this)),
			NULL));
	}
}

void ScrollLayer::beginShowLayer()
{
	m_layerStatus = ScrollLayerStatus::showingStatus;
	// 播放进来的声音
	if (WJUtils::canClick("playScrollShowSound", 500))
		Common::sound.play("Common:0002");
}

void ScrollLayer::showEnded()
{
	endShowLayer(0.5f);
}

void ScrollLayer::endShowLayer(float delay)
{
	m_layerStatus = ScrollLayerStatus::showedStatus;
	if (m_firstInto && m_scrollLayer)
	{
		m_scrollLayer->stopAllActions();
		m_scrollLayer->runAction(Sequence::create(
			DelayTime::create(delay),
			EaseSineOut::create(MoveTo::create(0.5f, Vec2(m_scrollLayer->getPositionX(), m_scrollLayer->getMinPosition()))),
			CCCallFunc::create([=](){
				m_scrollLayer->setEnabled(true);
			}),
		NULL));
		m_firstInto = false;
	}
	doEventScrollLayerShowEndCallBack(this, nullptr, nullptr);
}

void ScrollLayer::hide(float delay, bool enable)
{
	if (m_layerStatus == ScrollLayerStatus::showedStatus)
	{
		this->setEnabled(enable);
		resetSwitchSpritePicture();
		const Size &size = Director::getInstance()->getWinSize();
		this->stopAllActions();
		this->runAction(Sequence::create(
			DelayTime::create(delay),
			CCCallFunc::create(CC_CALLBACK_0(ScrollLayer::beginHideLayer, this)),
			EaseBackIn::create(
				MoveTo::create(0.5f, this->getParent()->convertToNodeSpace(Vec2(size.width + 50, size.height / 2)))
			),
			CCCallFunc::create(CC_CALLBACK_0(ScrollLayer::endHideLayer, this)),
			NULL));
	}
}

void ScrollLayer::beginHideLayer()
{
	m_layerStatus = ScrollLayerStatus::hidingStatus;
}

void ScrollLayer::endHideLayer()
{
	m_layerStatus = ScrollLayerStatus::hidedStatus;
}

void ScrollLayer::onClickSwitchSprite(Node *node, WJTouchEvent *event)
{
	if (m_layerStatus == ScrollLayerStatus::hidedStatus)
		this->show();
	else if (m_layerStatus == ScrollLayerStatus::showedStatus)
	{
		Common::sound.play("Common:button");
		this->hide();
	}
}

void ScrollLayer::resetSwitchSpritePicture()
{
	if (m_layerStatus == ScrollLayerStatus::hidedStatus)
		m_switchSprite->loadSpriteTexture("game/03_minigame/common/ui/up.png");
	else if (m_layerStatus == ScrollLayerStatus::showedStatus)
		m_switchSprite->loadSpriteTexture("game/03_minigame/common/ui/down.png");
}

void ScrollLayer::moveOutSideScene( float delay, bool enable )
{
	// 当前不管什么状态，只要移动到屏幕外面都会去强制执行、并且一旦执行就不会停止
	//if (m_layerStatus == ScrollLayerStatus::showedStatus)
	if (!this->getActionByTag(SCROLL_LAYER_MOVE_OUT_SIDE_TAG))
	{
		this->setEnabled(false);
		const Size &size = Director::getInstance()->getWinSize();
		this->stopAllActions();
		this->runAction(Sequence::create(
			DelayTime::create(delay),
			CCCallFunc::create(CC_CALLBACK_0(ScrollLayer::beginHideLayer, this)),
			EaseBackIn::create(
			MoveTo::create(0.5f, this->getParent()->convertToNodeSpace(Vec2(size.width + 150, size.height / 2)))
			),
			CCCallFunc::create(CC_CALLBACK_0(ScrollLayer::endHideLayer, this)),
			CCCallFunc::create([=]() {
				this->setEnabled(enable);
			}),
			NULL))->setTag(SCROLL_LAYER_MOVE_OUT_SIDE_TAG);
	}
}

void ScrollLayer::initItemSelectFromIndex( int index )
{
	ScrollItem *item = (ScrollItem*)m_vectorItem.at(index);
	item->setItemSelect(true);
	doEventSelectScrollItem(item, NULL, (WJScrollLayer*)item->getParent());
}

bool ScrollLayer::getScrollIsShowed()
{
	return 	m_layerStatus == ScrollLayerStatus::showedStatus;
}

void ScrollLayer::reInit(std::string key, ScrollDragDir drag, int count, bool clickAble, bool firstInto /*= false*/,
	float delayHide /*= 0.f*/, bool enable /*= true*/, float delayShow, float rightLength)
{

	if (m_layerStatus == ScrollLayerStatus::showedStatus)
	{
		this->setEnabled(enable);
		resetSwitchSpritePicture();
		const Size &size = Director::getInstance()->getWinSize();

		this->stopAllActions();
		this->runAction(Sequence::create(
			DelayTime::create(delayHide),
			CCCallFunc::create(CC_CALLBACK_0(ScrollLayer::beginHideLayer, this)),
			EaseBackIn::create(
			MoveTo::create(0.5f, this->getParent()->convertToNodeSpace(Vec2(size.width + 50, size.height / 2)))
			),
			CCCallFunc::create([=]()
		{
			endHideLayer();
			initScrollLayer(key.c_str(), drag, count, clickAble, firstInto);
			show(delayShow, rightLength);
		}),
			NULL));
	}

}
