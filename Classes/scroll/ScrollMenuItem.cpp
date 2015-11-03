// Created by yuanzhongji on 2015/09

#include "ScrollMenuItem.h"

bool ScrollMenuItem::init(ScrollLayer *scrollLayer, const char *  key, int index, ScrollDragDir drag, bool clickAble)
{
	if (!ScrollItem::init(scrollLayer, key, index, drag, clickAble))
	{
		return false;
	}

	m_bgSprite->loadSpriteTexture("game/03_minigame/common/ui/frame006.png");

	return true;
}

void ScrollMenuItem::onEnterTransitionDidFinish()
{
	ScrollItem::onEnterTransitionDidFinish();
}

void ScrollMenuItem::onEnter()
{
	ScrollItem::onEnter();
}

void ScrollMenuItem::onExit()
{
	ScrollItem::onExit();
}

void ScrollMenuItem::onSelectScrollItemNotification(Ref *ref)
{
	// nothing 不接收2级item发出的选中通知
}

void ScrollMenuItem::doEventSelectWhenDragOutFromScrollLayer(Node *node)
{

}

void ScrollMenuItem::setMenuItemSelectCallBack(const ScrollMenuItemCallback & callback)
{
	m_menuItemSelectCallBack = callback;
}

ScrollMenuItem::ScrollMenuItem()
{
	m_menuItemSelectCallBack = nullptr;
}
