// Created by wangwenjie on 2014/01

#ifndef __SCROLL_DOUBLE_LAYER_H__
#define __SCROLL_DOUBLE_LAYER_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "ConfigFoolHead.h"
#include "PBase.h"
#include "LockHelper.h"
#include "ScrollBase.h"
#include "ScrollLayer.h"
#include "ScrollItem.h"
#include "ScrollMenuItem.h"
#include "GameManager.h"

class ScrollDoubleLayer : public ScrollLayer
{
protected:
	 bool* m_scrollFirstInto;
	// 前一个选中的菜单的索引
	 CC_SYNTHESIZE(int, m_preSelectMenuIndex, PreSelectMenuIndex);
	 // menu选择菜单
	 ScrollTouchCallback m_clickMenuScrollEvent;
	 WJVScrollLayer *m_menuScrollLayer;
	 std::vector<ScrollMenuItem*> m_vectorMenuItem;

	// menu选择菜单相关
	virtual void initScrollLayerAgain(int index = 0);
	virtual void creatMenuScrollItem(cocos2d::Vector<WJScrollItem*> &items, int count, const char *key);
	virtual void initMenuClickScroll(const char * key, int count);
	
	virtual void _doEventMenuScrollItemClick(Node *node, WJTouchEvent *e, WJScrollLayer *scroll);
	virtual void playScrollLayerActionWhenClickMenuItem(ScrollMenuItem *item, WJTouchEvent *e, WJScrollLayer *scroll);
	virtual void resetSrollLayerWhenClickMenuItem(ScrollMenuItem *item, WJTouchEvent *e, WJScrollLayer *scroll);

	virtual void changeSelectMenuIndex(int index);
	virtual void setMenuItemSelected(ScrollMenuItem *item);

	// 点击选择坐标菜单按钮后的回调
	virtual void doEventClickMenuScrollCallBack(Node* node, WJTouchEvent* e, WJScrollLayer *scroll);
	// 点击了左边的一次菜单后的回调
	virtual void playClickMenuItemRunScrollAction(bool play = true);

	virtual void beginShowLayer() override;
	virtual void doEventScrollLayerShowEndCallBack(Node *node, WJTouchEvent *e, WJScrollLayer *scroll) override;
	
public:

	// 点击选择菜单按钮后的回调(touchEvent, scroll 在初始化时传递的都为null)
	// 如果需要动态替换item直接在回调里面处理
	void setClickMenuScrollItemEvent(const ScrollTouchCallback &callback);
	// 显示
	void show(float delay = 0.0f);
	// 重新加载子菜单
	void resetInitScrollLayer(std::string key, ScrollItem*item);
	// 设置当前1级菜单的选择index
	void setMenuItemSelectIndex(int index = 0);

public:

	ScrollDoubleLayer();
	~ScrollDoubleLayer();

	static ScrollDoubleLayer *create(const char * keyLeft, ScrollDragDir dragLeft, int countLeft, bool clickAbleLeft, const bool* firstIntoLeft);

	virtual bool init(const char * keyLeft, ScrollDragDir dragLeft, int countLeft, bool clickAbleLeft, const bool* firstIntoLeft);

	virtual void onEnterTransitionDidFinish() override;

	virtual void onEnter() override;
	virtual void onExit() override;

};
#endif // __SCROLL_LAYER_H__
