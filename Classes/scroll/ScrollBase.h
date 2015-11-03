// Created by yuanzhongji on 2015/09

#ifndef __SCROLL_BASE_H__
#define __SCROLL_BASE_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "ConfigFoolHead.h"
#include "PBase.h"
#include "LockHelper.h"
#include <string>
#include "vector"

using namespace cocos2d;

enum class DragoutType
{
	WJSPRITE = 0,
	WJSKELETON
};

enum class ScrollDragDir
{
	DragLeft = 0,
	DragRight,
	DragUp,
	DragDown,
	StopDrag
};

enum class ScrollLayerStatus
{
	normalStatus = 0,
	showingStatus,
	showedStatus,
	hidingStatus,
	hidedStatus,
};


typedef std::function<bool (Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)> ScrollTouchBoolCallback;
typedef std::function<void (Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)> ScrollTouchCallback;
typedef std::function<Node* (Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)> ScrollInitDragOutNodeCallback;

#define CC_CALLBACK_5(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4,std::placeholders::_5, ##__VA_ARGS__)

class ScrollBase
{
protected:

	// 回调函数
	ScrollTouchBoolCallback m_scrollTouchAble;
	ScrollTouchBoolCallback m_scrollMovingCollision;
	ScrollTouchCallback m_scrollItemDragOutCallBack;
	ScrollTouchCallback m_scrollCollisonEnded;
	ScrollTouchCallback m_clickItem;
	ScrollTouchCallback m_selectCallBack;
	ScrollTouchCallback m_clickCancel;
	ScrollTouchCallback m_failedCallBack;
	ScrollTouchCallback m_scrollTouchEndedCallBack;
	ScrollTouchCallback m_scrollLayerShowEndCallBack;
	ScrollInitDragOutNodeCallback m_scrollDragOutInitNodeCallBack;

	// 当前取消按钮需要操作的堆栈
	stack<WJSprite*> m_stack;
	std::vector<stack<WJSprite*>> m_stackVector;
	//  是否自动处理(单个scrolllayer)
	std::vector<bool> m_dealCancelVector;
	// 当前layer的所有子项
	Vector<Node*> m_vectorItem;

	// 滚动控件事件相关
	virtual bool doEventScrollItemTouchAble(Node* node, WJTouchEvent* e, WJScrollLayer *scroll);
	virtual bool doEventScrollItemMovingCollision(Node* node, WJTouchEvent* e, WJScrollLayer *scroll);
	virtual void doEventScrollItemDragOutCallBack(Node* node, WJTouchEvent* e, WJScrollLayer *scroll);
	virtual void doEventScrollItemCollisonEnded(Node *node, WJTouchEvent* e, WJScrollLayer *scroll);
	virtual void doEventScrollItemClick(Node *node, WJTouchEvent *e, WJScrollLayer *scroll);
	virtual void doEventSelectScrollItem(Node *node, WJTouchEvent *e, WJScrollLayer *scroll);
	virtual void doEventClickCancelItem(Node *node, WJTouchEvent *e, WJScrollLayer *scroll);
	virtual void doEventFailedScrollItem(Node *node, WJTouchEvent *e, WJScrollLayer *scroll);
	virtual void doEventScrollItemTouchEnded(Node *node, WJTouchEvent *e, WJScrollLayer *scroll);
	virtual void doEventSelectWhenDragOutFromScrollLayer(Node *node);
	virtual void doEventScrollLayerShowEndCallBack(Node *node, WJTouchEvent *e, WJScrollLayer *scroll);
	virtual Node* doEventScrollDragOutInitNodeCallBack(Node *node, WJTouchEvent *e, WJScrollLayer *scroll);

public:

	// 是否处理点击取消按钮
	// 默认的处理行为为移除当前保存的堆栈的头部元素
	// 当前被选中的子项索引(从0开始)
	CC_SYNTHESIZE(int, m_selectIndex, SelectIndex);
	// 保存堆栈最大的值
	CC_SYNTHESIZE(int, m_stackMaxNum, StackMaxNum);
	// 当前处理的堆栈的index
	CC_SYNTHESIZE(int, m_stackIndex, StackIndex);
	// 当前取消按钮是否自动处理index
	CC_SYNTHESIZE(int, m_dealCancelIndex, DealCancelIndex);

	// 触摸开始的处理
	void setScrollItemTouchAbleEvent(const ScrollTouchBoolCallback &callback);
	// 滚动的精灵在移动过程中的碰撞的检测，当返回为true时表示碰撞到了物体, 当前精灵需要的信息
	// 统一保存到setUserData()里面，touchend里面使用UserData里面的变量
	void setScrollItemMovingCollisionEvent(const ScrollTouchBoolCallback &callback);
	// 当前的滚动控件检测到了碰撞后的事件处理
	void setScrollItemCollisonEndedEvent(const ScrollTouchCallback &callback);
	// 子项被拖出来之后的回调(每次拖出来执行一次)
	void setScrollItemDragOutCallBack( const ScrollTouchCallback &callback );
	// 当前的滚动子项的点击事件处理
	void setClickScrollItemEvent(const ScrollTouchCallback &callback);
	// 当前的子项被选中后的事件处理
	void setSelectScrollItemEvent(const ScrollTouchCallback &callback);
	// 点击了当前的取消按钮
	void setClickCancelItemEvent(const ScrollTouchCallback &callback);
	// 碰撞失败的处理(只是子项可以移动的)
	void setFailedSelectItemEvent(const ScrollTouchCallback &callback);
	// 触摸结束
	void setScrollItemTouchEndedEvent(const ScrollTouchCallback &callback);
	// layer显示完全之后的回调
	void setScrollLayerShowEndCallBack(const ScrollTouchCallback &callback);
	// item被拖拽出来创建新node的回调,返回null时默认创建sprite
	void setScrollDragOutInitNodeCallBack(const ScrollInitDragOutNodeCallback &callback);

	// 设置当前是否被选中了
	void setItemSelect(bool select);

	// cancel 处理相关
	bool addNodeIntoStack(WJSprite *sprite);
	void deleteNodeFromStackTop();
	void deleteNodeFromInStack(WJSprite *sprite);
	int getPutIntoStackNodeNumber();
	void cleanWholeNodeFromStack();
	//取得最后一个元素，当取消的元素有绑定到骨骼时使用。
	WJSprite* getStackTopSprite();

	// 取得当前被选中的子项的索引
	int getItemSelectIndex();
	// 取得当前的子项
	Node* getItemFromIndex(int index);
	// 取得当前的子项总数
	ssize_t getItemChildCount();
	// 替換item
	void replaceScrollItem(Node *srcNode, Node *toNode);
	// ScrollDoubleLayer是否自动处理cancel事件(start, index1,..indexn, end); endIndex = -1;
	void setAutoDealCancel(int index, ...);
	// 单个scrollLayer
	void setAutoDealCancel();
	//-----------------------------------------------------
public:

	ScrollBase();
	~ScrollBase();

	// 设置当前的节点被选中
	void initItemSelectFromIndex(int index);
};

#endif // __SCROLL_LAYER_H__
