// Created by wangwenjie on 2014/01

#include "ScrollDoubleLayer.h"
#include "ScrollMenuItem.h"

USING_NS_CC;

#define CALL_DOEVENT(_selector) if (_selector) { _selector(node, e, scroll); }

ScrollDoubleLayer * ScrollDoubleLayer::create(const char * keyLeft, ScrollDragDir dragLeft, int countLeft, bool clickAbleLeft, const bool* firstIntoLeft )
{
	ScrollDoubleLayer *layer = new (std::nothrow) ScrollDoubleLayer();
	if (layer && layer->init(keyLeft, dragLeft, countLeft, clickAbleLeft, firstIntoLeft))
	{
		layer->autorelease();
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return nullptr;
}

bool ScrollDoubleLayer::init(const char * keyLeft, ScrollDragDir dragLeft, int countLeft, bool clickAbleLeft, const bool* firstIntoLeft )
{
	m_scrollFirstInto = (bool*)firstIntoLeft;

	ScrollDoubleTemplate object = ScrollDoubleTemplateTem(WJUtils::stringAddInt(keyLeft, 1, 3).c_str());

	// 默认初始化一级菜单的第一个对应的滚动条
	if (!ScrollLayer::init(object.itemKey.c_str(), ScrollDragDir::DragLeft, object.count, object.clickAble, *(m_scrollFirstInto + 2)))
		return false;

	initMenuClickScroll(keyLeft, countLeft);

	return true;
}

void ScrollDoubleLayer::initScrollLayerAgain(int index)
{
	_doEventMenuScrollItemClick(m_vectorMenuItem.at(index), nullptr, nullptr);
}

void ScrollDoubleLayer::onEnterTransitionDidFinish()
{
	ScrollLayer::onEnterTransitionDidFinish();
}

void ScrollDoubleLayer::onEnter()
{
	ScrollLayer::onEnter();
}

ScrollDoubleLayer::ScrollDoubleLayer()
{
	m_menuScrollLayer = nullptr;
	m_clickMenuScrollEvent = nullptr;
	m_preSelectMenuIndex = -1;
}

ScrollDoubleLayer::~ScrollDoubleLayer()
{

}

void ScrollDoubleLayer::onExit()
{
	ScrollLayer::onExit();
}

void ScrollDoubleLayer::initMenuClickScroll(const char * key, int count)
{
	if (m_menuScrollLayer)
	{
		m_menuScrollLayer->removeFromParentAndCleanup(true);
		m_menuScrollLayer = NULL;
	}

	const Size &size = this->getContentSize();
	cocos2d::Vector<WJScrollItem*> items;
	items.clear();
	creatMenuScrollItem(items, count, key);

	m_menuScrollLayer = WJVScrollLayer::create();
	m_menuScrollLayer->pageScrollMode = false;
	m_menuScrollLayer->initWithItems(items, Rect(122, 70, 110, 375), NULL, false,
		NULL, Color3B::WHITE);
	m_menuScrollLayer->setPosition(Vec2(173, 100));
	this->addChild(m_menuScrollLayer, 50);

	// bg
	WJSprite *leftbg = WJSprite::create("game/03_minigame/common/ui/frame004.png");
	leftbg->setAnchorPoint(Vec2(0.0f, 0.0f));
	leftbg->ignoreAnchorPointForPosition(false);
	leftbg->setPosition(Vec2(105, 37));
	this->addChild(leftbg, 10);

	// 第一次进入左边的滚动layer滚动
	if (*(m_scrollFirstInto + 1))
	{
		m_menuScrollLayer->setEnabled(false);
		m_menuScrollLayer->setPositionY(m_menuScrollLayer->getMaxPosition());
	}

	// 隐藏开关
	m_switchSprite->setVisible(false);
}

void ScrollDoubleLayer::creatMenuScrollItem(cocos2d::Vector<WJScrollItem*> &items, int count, const char *key)
{
	m_vectorMenuItem.clear();
	for (int i = 0; i < count; i++)
	{
		ScrollMenuItem *scrollItem = ScrollMenuItem::create(this, key, i, ScrollDragDir::StopDrag, true);
		scrollItem->setClickScrollItemEvent(CC_CALLBACK_3(ScrollDoubleLayer::_doEventMenuScrollItemClick, this));
		// 默认选中了第一个
		if (i == 0) scrollItem->setItemSelect(true);

		std::string itemKey = key;
		WJUtils::stringAddInt(itemKey, i + 1, 3);
		ScrollItemTemplate object = ScrollItemTemplateTem(itemKey);
		WJScrollItem* item = WJScrollItem::create(scrollItem, 0, scrollItem->getContentSize().height + object.itemsInterval);
		items.pushBack(item);

		// 当前1级菜单的数组
		m_vectorMenuItem.push_back(scrollItem);
	}
}

void ScrollDoubleLayer::_doEventMenuScrollItemClick(Node *node, WJTouchEvent *e, WJScrollLayer *scroll)
{
	auto item = (ScrollMenuItem*)node;
	if (item->getItemIndex() == m_preSelectMenuIndex)
		return;

	// 选中状态的改变
	setMenuItemSelected(item);

	playScrollLayerActionWhenClickMenuItem(item, e, scroll);
}

void ScrollDoubleLayer::playScrollLayerActionWhenClickMenuItem(ScrollMenuItem *item, WJTouchEvent *e, WJScrollLayer *scroll)
{
	m_scrollBg->stopAllActions();
	m_scrollBg->runAction(Sequence::create(
		ScaleTo::create(0.3f, 0.01f),
		CCCallFunc::create(CC_CALLBACK_0(ScrollDoubleLayer::resetSrollLayerWhenClickMenuItem, this, item, e, scroll)),
		DelayTime::create(0.15f),
		ScaleTo::create(0.3f, 1.0f),
		nullptr));
}

void ScrollDoubleLayer::resetSrollLayerWhenClickMenuItem(ScrollMenuItem *item, WJTouchEvent *e, WJScrollLayer *scroll)
{
	// 重新创建新的二级layer菜单
	changeSelectMenuIndex(item->getItemIndex());
	resetInitScrollLayer(item->getScrollItemKey(), item);

	playClickMenuItemRunScrollAction(e != nullptr);
	doEventClickMenuScrollCallBack(item, e, scroll);
}

void ScrollDoubleLayer::doEventClickMenuScrollCallBack(Node* node, WJTouchEvent* e, WJScrollLayer *scroll)
{
	CALL_DOEVENT(m_clickMenuScrollEvent);
}

void ScrollDoubleLayer::setClickMenuScrollItemEvent(const ScrollTouchCallback &callback)
{
	m_clickMenuScrollEvent = callback;
}

void ScrollDoubleLayer::playClickMenuItemRunScrollAction(bool play)
{
	if (play) 
		endShowLayer(1.0f);
}

void ScrollDoubleLayer::doEventScrollLayerShowEndCallBack(Node *node, WJTouchEvent *e, WJScrollLayer *scroll)
{
	bool isFirstIntoLeft = *(m_scrollFirstInto + 1);
	if (isFirstIntoLeft && m_menuScrollLayer)
	{
		*(m_scrollFirstInto + 1) = false;
		m_menuScrollLayer->stopAllActions();
		m_menuScrollLayer->runAction(Sequence::create(
			DelayTime::create(0.5f),
			EaseSineOut::create(MoveTo::create(0.5f, Vec2(m_menuScrollLayer->getPositionX(), m_menuScrollLayer->getMinPosition()))),
			CCCallFunc::create([=]() {
				m_menuScrollLayer->setEnabled(true);
			}),
		NULL));
	}
}

void ScrollDoubleLayer::beginShowLayer()
{
	ScrollLayer::beginShowLayer();

	if (*(m_scrollFirstInto + 1) && m_menuScrollLayer)
		m_menuScrollLayer->setPositionY(m_menuScrollLayer->getMaxPosition());
}

void ScrollDoubleLayer::show(float delay /*= 0.0f*/)
{
	this->setMenuItemSelectIndex(0);
	ScrollLayer::show(delay, 170);
}

void ScrollDoubleLayer::resetInitScrollLayer(std::string key, ScrollItem*item)
{
	ScrollDoubleTemplate object = ScrollDoubleTemplateTem(key);
	initScrollLayer(object.itemKey.c_str(), ScrollDragDir::DragLeft, object.count, object.clickAble, *(m_scrollFirstInto + item->getItemIndex() + 2));
	*(m_scrollFirstInto + item->getItemIndex() + 2) = false;
}


void ScrollDoubleLayer::setMenuItemSelectIndex(int index)
{
	// menu layer 选中状态的改变
	setMenuItemSelected(m_vectorMenuItem.at(index));
	// 重新创建子菜单
	initScrollLayerAgain(index);
}

void ScrollDoubleLayer::changeSelectMenuIndex(int index)
{
	m_preSelectMenuIndex = index;
	setDealCancelIndex(index);
}

void ScrollDoubleLayer::setMenuItemSelected(ScrollMenuItem *item)
{
	ScrollMenuItem* _item;
	for (int i = 0; i < m_vectorMenuItem.size(); i++)
	{
		_item = m_vectorMenuItem.at(i);
		_item->setItemSelect(false);
	}
	item->setItemSelect(true);
}