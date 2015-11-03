// Created by yuanzhongji on 2015/09

#ifndef __ScrollItem_H__
#define __ScrollItem_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "LBModalDialogLayer.h"
#include "ScrollLayer.h"
#include "ScrollBase.h"
#include "PConfig.h"

#define CREATE_SCROLLITEM_FUNC(__TYPE__)					\
	static __TYPE__* create(ScrollLayer *scrollLayer, const char * key, int index, ScrollDragDir drag, bool clickAble)	\
{													\
	__TYPE__ *item = new (std::nothrow) __TYPE__(); \
	if (item && item->init(scrollLayer, key, index, drag, clickAble))		\
	{									\
		item->autorelease();		\
		return item;		\
	}	\
	CC_SAFE_DELETE(item);		\
	return NULL;		\
}		\

class ScrollItem : public WJLayer , public ScrollBase
{
protected:
	// 背景
	WJSprite *m_bgSprite;
	// 滚动layer
	CC_SYNTHESIZE(ScrollLayer*, m_pScrollLayer, PScrollLayer);
	// 滚动子项移动的方向
	CC_SYNTHESIZE(ScrollDragDir, m_dragDir, DragDir);
	// 当前正在移动的精灵
	WJSprite *m_movingSprite;
	// 是否可以移动
	CC_SYNTHESIZE(bool, m_itemMoveAble, ItemMoveAble);
	// 是否可以点击
	CC_SYNTHESIZE(bool, m_itemClickAble, ItemClickAble);
	// icon 精灵
	WJSprite *m_itemSprite;
	// 拖出来的item类型
	CC_SYNTHESIZE(DragoutType ,m_type,Type);
	// 移动的过程中是否碰撞到了符合条件的
	CC_SYNTHESIZE(bool, m_movingCollide, MovingCollide);
	// 是否是取消按钮
	CC_SYNTHESIZE(bool, m_cancelItem, CalcelItem);
	// 拖出来后的放大的大小
	CC_SYNTHESIZE(float, m_itemScaleTo, ItemScaleTo);
	// 拖出来后的旋转
	CC_SYNTHESIZE(float, m_itemRotateTo, ItemRotateTo);
	// 背景拖出来后的放大的大小
	CC_SYNTHESIZE(float, m_itemBgScale, ItemBgScale);
	// 原始的key
	CC_SYNTHESIZE(std::string, m_primievalKey, PrimievalKey);
	// icon 图片
	CC_SYNTHESIZE(std::string, m_itemIconFile, ItemIconFile);
	// icon的大小
	CC_SYNTHESIZE(float, m_itemIconScale, ItemIconScale);
	// 当前的索引(从0开始)
	CC_SYNTHESIZE(int, m_itemIndex, ItemIndex);
	// 当前粒子效果的个数
	CC_SYNTHESIZE(int, m_particleNum, ParticleNum);
	// 当被拖出滚动控件的时候的偏移量
	CC_SYNTHESIZE(Vec2, m_offsetWhenDragOut, OffsetWhenDragOut);
	// 子项对应的配置文件的key
	CC_SYNTHESIZE(std::string, m_scrollItemKey, ScrollItemKey);
	// item对应大图的路径
	CC_SYNTHESIZE(std::string, m_itemFilePath, ItemFilePath);
	// item自定义保存数据
	CC_SYNTHESIZE(void*, m_itemUserData, ItemUserData);

	// 通知相关
	EventListenerCustom* m_storeEventListener;
	EventListenerCustom* m_getfreeEventListener;

	// 加锁
	virtual void initLockedScrollItem();

	// 拖拽出來相關
	virtual bool isScrollDragOut(WJTouchEvent *event, ScrollDragDir drag, float minDistance = 20.0f, float maxDistance = 20.0f);
	virtual Vec2 movePointWhenDragOut(ScrollDragDir drag);
	virtual void creatNewSpriteOnScrollSide(WJSprite *oldsprite, WJScrollLayer *scroll, ScrollDragDir drag, float scaleTo);
	virtual float playItemBounceEffect(Node* pNode, float time = 0.15f);

	// 事件处理
	virtual bool _onScrollItemTouchAble( Node *node, WJTouchEvent *event);
	virtual bool _onScrollItemWillMoveTo(Node *node, WJTouchEvent *event);
	virtual void _onScrollItemTouchEnded( Node *node, WJTouchEvent *event);
	virtual void _onClickCancelItem(Node *node, WJTouchEvent *event);

	// 当前的项被选中的通知
	virtual void onSelectScrollItemNotification(Ref *ref);

	// 点击了星星购买后的回调
	virtual void onClickLockItemCallBack(Node *node, WJTouchEvent *event, WJScrollLayer *scroll);
	virtual void onMovingLockItemCallBack(Node *node, WJTouchEvent *event, WJScrollLayer *scroll);

	// 通知相关
	virtual void onStoreIapPurchased();
	virtual void onGetFreeUnlocked(const char* getFreeLockKey, int coins);

	virtual Node* _doEventWhenDragOutItemFromScrollLayer(WJSprite *sprite, WJTouchEvent *event, WJScrollLayer *scroll);

	// 在判断加锁前进行可用性的判断
	virtual bool checkItemUsedAbleBeforeCheckLocked(Node *node, WJTouchEvent *event, WJScrollLayer *scroll);
	// item在移动之前判断是否可以移动
	virtual bool checkItemMovingAbleWhenMoving(Node *node, WJTouchEvent *event);

public:
	//clone a WJSprite or WJSkeletonAnimation
	virtual Node* getItem(WJSprite *spr);
	// 设置当前是否被选中了
	void setItemSelect(bool select);
	// 取得当前对应的item的大图的图片路径
	std::string getItemFilePath();
	// 取得当前正在移动的精灵
	WJSprite* getMovingSprite();
	// 取得当前的icon精灵
	WJSprite* getItemIconSprite();
	// 添加粒子效果
	void addParticleIntoItem(int sum);
	// 移除掉粒子效果
	void removeParticle();
	
	// 克隆事件到scrollitem
	void cloneEventTo(ScrollItem *toItem);

public:
	
	ScrollItem();

	CREATE_SCROLLITEM_FUNC(ScrollItem);

	virtual bool init(ScrollLayer *scrollLayer, const char * key, int index, ScrollDragDir drag, bool clickAble);

    virtual void onEnterTransitionDidFinish();

	virtual void onEnter();
	virtual void onExit();

};
#endif // __ScrollItem_H__
