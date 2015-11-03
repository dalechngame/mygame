// Created by wangwenjie on 2014/01

#ifndef __SCROLL_LAYER_H__
#define __SCROLL_LAYER_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "ConfigFoolHead.h"
#include "PBase.h"
#include "LockHelper.h"
#include "ScrollBase.h"

class ScrollLayer : public WJLayer, public ScrollBase
{
protected:
	CC_SYNTHESIZE(ScrollLayerStatus, m_layerStatus, LayerStatus);
	WJSprite *m_scrollBg;
	WJVScrollLayer* m_scrollLayer;
	WJSprite *m_switchSprite;
	bool m_firstInto;

	virtual void initBgSprite();
	virtual void initScrollLayer(const char * key, ScrollDragDir drag, int count, bool clickAble, bool firstInto);
	virtual void creatScrollItem(cocos2d::Vector<WJScrollItem*> &items, int count, const char *key, ScrollDragDir drag, bool clickAble);

	// 移动状态相关
	virtual void beginShowLayer();
	virtual void showEnded();
	virtual void endShowLayer(float delay = 0.5f);

	virtual void beginHideLayer();
	virtual void endHideLayer();

	virtual void onClickSwitchSprite(Node *node, WJTouchEvent *event);
	virtual void resetSwitchSpritePicture();

public:

	ScrollLayer();
	~ScrollLayer();

	static ScrollLayer *create(const char *key, ScrollDragDir drag, int count, bool clickAble, bool firstInto = false);
	virtual bool init(const char * key, ScrollDragDir drag, int count, bool clickAble, bool firstInto);
    virtual void onEnterTransitionDidFinish();

	virtual void onEnter();
	virtual void onExit();

	// 取得当前场景的json
	WJLayerJson* getSceneJson();

	// 重新加载滚动条
	void reInit(std::string key, ScrollDragDir drag, int count, bool clickAble, bool firstInto = false, float delayHide = 0.f,
		bool enable = true, float delayShow = 0.5f, float rightLength = 70.f);

	// 显示滚动条(默认从右边移动到屏幕里面)
	void show(float delay = 0.0f, float rightLength = 70);
	// 隐藏滚动条、可以在隐藏的时候设置当前是否可用
	void hide(float delay = 0.0f, bool enable = true);
	// 移动当前的滚动条到右边的屏幕外面
	void moveOutSideScene(float delay = 0.0f, bool enable = false);
	// 设置当前的节点被选中
	void initItemSelectFromIndex(int index);
	// 取得当前滚动条是否显示在屏幕里面
	bool getScrollIsShowed();
};
#endif // __SCROLL_LAYER_H__
