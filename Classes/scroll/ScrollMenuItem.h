// Created by yuanzhongji on 2015/09

#ifndef __Scroll_Menu_Item_H__
#define __Scroll_Menu_Item_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "LBModalDialogLayer.h"
#include "ScrollItem.h"

typedef std::function<void(Node* node)> ScrollMenuItemCallback;

class ScrollMenuItem : public ScrollItem
{
protected:

	ScrollMenuItemCallback m_menuItemSelectCallBack;

	// 接受到选中的通知
	virtual void onSelectScrollItemNotification(Ref *ref) override;
	// 选中后的回调
	virtual void doEventSelectWhenDragOutFromScrollLayer(Node *node) override;

public:

	virtual void setMenuItemSelectCallBack(const ScrollMenuItemCallback & callback);

public:

	ScrollMenuItem();

	CREATE_SCROLLITEM_FUNC(ScrollMenuItem);

	virtual bool init(ScrollLayer *scrollLayer, const char * key, int index, ScrollDragDir drag, bool clickAble) override;

    virtual void onEnterTransitionDidFinish();

	virtual void onEnter();
	virtual void onExit();

};
#endif // __ScrollItem_H__
