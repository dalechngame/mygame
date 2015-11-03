// Created by yuanzhongji on 2015/09

#include "ScrollBase.h"
#include "Common.h"
#include "LBModalDialogLayer.h"
#include "ScrollItem.h"
#include "GameManager.h"

USING_NS_CC;

#define CALL_DOEVENT(_selector) if (_selector) { _selector(node, e, scroll); }
#define CALL_DOEVENT_BOOL(_selector) return !_selector || _selector(node, e, scroll);

ScrollBase::ScrollBase()
{
	m_scrollTouchAble = nullptr;
	m_scrollMovingCollision = nullptr;
	m_scrollCollisonEnded = nullptr;
	m_clickItem = nullptr;
	m_selectCallBack = nullptr;
	m_clickCancel = nullptr;
	m_failedCallBack = nullptr;
	m_scrollItemDragOutCallBack = nullptr;
	m_selectIndex = -1;
	m_stackMaxNum = 100;
	m_scrollTouchEndedCallBack = nullptr;
	m_scrollLayerShowEndCallBack = nullptr;
	m_stackIndex = 0;
	m_dealCancelIndex = 0;
	m_scrollDragOutInitNodeCallBack = nullptr;
	// 默认10个
	for (int i = 0; i <= 10; i++)
	{
		m_stackVector.push_back(m_stack);
		m_dealCancelVector.push_back(false);
	}
}

ScrollBase::~ScrollBase()
{
	m_stackVector.clear();
}

bool ScrollBase::doEventScrollItemTouchAble(Node* node, WJTouchEvent* e, WJScrollLayer *scroll)
{
	CALL_DOEVENT_BOOL(m_scrollTouchAble);
}

void ScrollBase::setScrollItemTouchAbleEvent( const ScrollTouchBoolCallback &callback )
{
	m_scrollTouchAble = callback;
}

void ScrollBase::setScrollItemMovingCollisionEvent(const ScrollTouchBoolCallback &callback)
{
	m_scrollMovingCollision = callback;
}

bool ScrollBase::doEventScrollItemMovingCollision(Node* node, WJTouchEvent* e, WJScrollLayer *scroll)
{
	// 超出限制了
	if (getPutIntoStackNodeNumber() >= m_stackMaxNum && m_dealCancelVector.at(m_dealCancelIndex))
		return false;

	CALL_DOEVENT_BOOL(m_scrollMovingCollision);
}


void ScrollBase::setScrollItemCollisonEndedEvent(const ScrollTouchCallback &callback)
{
	m_scrollCollisonEnded = callback;
}

void ScrollBase::doEventScrollItemCollisonEnded(Node *node, WJTouchEvent* e, WJScrollLayer *scroll)
{
	CALL_DOEVENT(m_scrollCollisonEnded);
}

void ScrollBase::setClickScrollItemEvent(const ScrollTouchCallback &callback)
{
	m_clickItem = callback;
}

void ScrollBase::doEventScrollItemClick(Node *node, WJTouchEvent *e, WJScrollLayer *scroll)
{
	// 超出限制了
	if (getPutIntoStackNodeNumber() >= m_stackMaxNum && m_dealCancelVector.at(m_dealCancelIndex))
		return;

	CALL_DOEVENT(m_clickItem);
}

void ScrollBase::setSelectScrollItemEvent(const ScrollTouchCallback &callback)
{
	m_selectCallBack = callback;
}

void ScrollBase::doEventSelectScrollItem(Node *node, WJTouchEvent *e, WJScrollLayer *scroll)
{
	doEventSelectWhenDragOutFromScrollLayer(node);
	// 当前选中状态
	CALL_DOEVENT(m_selectCallBack);
}

void ScrollBase::doEventSelectWhenDragOutFromScrollLayer(Node *node)
{
	ScrollItem *item = (ScrollItem*)node;
	m_selectIndex = item->getItemIndex();
	// 当前被选中了
	NotificationCenter::getInstance()->postNotification("selectItem", __Integer::create(m_selectIndex));
}

void ScrollBase::doEventClickCancelItem(Node *node, WJTouchEvent *e, WJScrollLayer *scroll)
{
	// 自动处理点击取消按钮
	if (m_dealCancelVector.at(m_dealCancelIndex))
		deleteNodeFromStackTop();
	else
		CALL_DOEVENT(m_clickCancel);
}

void ScrollBase::deleteNodeFromStackTop()
{
	if (m_stackVector.at(m_stackIndex).empty()) return;

	// 取消上一次放的装饰品
	WJSprite *sprite = m_stackVector.at(m_stackIndex).top();
	if (sprite)
	{
		m_stackVector.at(m_stackIndex).pop();
		sprite->setVisible(false);
		sprite->removeFromParentAndCleanup(true);
		sprite = NULL;
	}

	GameManager::getInstance()->removeUnusedDataAndTexture();
}

WJSprite* ScrollBase::getStackTopSprite()
{
	if (m_stackVector.at(m_stackIndex).empty()) 
		return nullptr;
	return m_stackVector.at(m_stackIndex).top();
}

void ScrollBase::deleteNodeFromInStack(WJSprite *sprite)
{
	// 将当前移除的装饰品移除出堆栈里面
	if (m_stackVector.at(m_stackIndex).empty()) return;

	stack<WJSprite*> _stack;
	while (!m_stackVector.at(m_stackIndex).empty())
	{
		WJSprite *topSprite = m_stackVector.at(m_stackIndex).top();
		if (topSprite != sprite)
		{
			_stack.push(topSprite);
		}
		m_stackVector.at(m_stackIndex).pop();
	}

	// 将当前的临时栈重新保存到撤销栈里面
	while (!_stack.empty())
	{
		WJSprite *_sprite = _stack.top();
		m_stackVector.at(m_stackIndex).push(_sprite);
		_stack.pop();
	}
}

void ScrollBase::setClickCancelItemEvent( const ScrollTouchCallback &callback )
{
	m_clickCancel = callback;
}

bool ScrollBase::addNodeIntoStack(WJSprite *sprite)
{
	if (getPutIntoStackNodeNumber() >= m_stackMaxNum)
		return false;
	//m_stack.push(sprite);
	m_stackVector.at(m_stackIndex).push(sprite);
	return true;
}

int ScrollBase::getItemSelectIndex()
{
	return m_selectIndex;
}


void ScrollBase::initItemSelectFromIndex( int index )
{
	ScrollItem *item = (ScrollItem*)m_vectorItem.at(index);
	item->setItemSelect(true);
	doEventSelectScrollItem(item, NULL, (WJScrollLayer*)item->getParent());
}

void ScrollBase::doEventFailedScrollItem(Node *node, WJTouchEvent *e, WJScrollLayer *scroll)
{
	CALL_DOEVENT(m_failedCallBack);
}

void ScrollBase::setFailedSelectItemEvent( const ScrollTouchCallback &callback )
{
	m_failedCallBack = callback;
}

void ScrollBase::doEventScrollItemTouchEnded(Node *node, WJTouchEvent *e, WJScrollLayer *scroll)
{
	CALL_DOEVENT(m_scrollTouchEndedCallBack);
}

void ScrollBase::setScrollItemTouchEndedEvent(const ScrollTouchCallback &callback)
{
	m_scrollTouchEndedCallBack = callback;
}

int ScrollBase::getPutIntoStackNodeNumber()
{
	return m_stackVector.at(m_stackIndex).size();
}

Node* ScrollBase::getItemFromIndex( int index )
{
	return m_vectorItem.at(index);
}

ssize_t ScrollBase::getItemChildCount()
{
	return m_vectorItem.size();
}

void ScrollBase::doEventScrollItemDragOutCallBack(Node* node, WJTouchEvent* e, WJScrollLayer *scroll)
{
	CALL_DOEVENT(m_scrollItemDragOutCallBack);
}

void ScrollBase::setScrollItemDragOutCallBack( const ScrollTouchCallback &callback )
{
	m_scrollItemDragOutCallBack = callback;
}

void ScrollBase::setScrollLayerShowEndCallBack(const ScrollTouchCallback &callback)
{
	m_scrollLayerShowEndCallBack = callback;
}

void ScrollBase::doEventScrollLayerShowEndCallBack(Node *node, WJTouchEvent *e, WJScrollLayer *scroll)
{
	CALL_DOEVENT(m_scrollLayerShowEndCallBack);
}

void ScrollBase::cleanWholeNodeFromStack()
{
	do 
	{
		if (m_stackVector.at(m_stackIndex).empty())
			break;
		deleteNodeFromStackTop();

	} while (true);
}

void ScrollBase::setAutoDealCancel(int index, ...)
{
	va_list ap;
	va_start(ap, index);
	int a = -1;
	do
	{
		a = va_arg(ap, int);
		if (a != -1)
			m_dealCancelVector.at(a) = true;
	} while (a != -1);
	va_end(ap);
}

void ScrollBase::setAutoDealCancel()
{
	setAutoDealCancel(AUTO_DEAL_CANCEL_START_INDEX, 0, AUTO_DEAL_CANCEL_END_INDEX);
}

Node* ScrollBase::doEventScrollDragOutInitNodeCallBack(Node *node, WJTouchEvent *e, WJScrollLayer *scroll)
{
	if (m_scrollDragOutInitNodeCallBack)
		return m_scrollDragOutInitNodeCallBack(node, e, scroll);
	
	return nullptr;
}

void ScrollBase::setScrollDragOutInitNodeCallBack(const ScrollInitDragOutNodeCallback &callback)
{
	m_scrollDragOutInitNodeCallBack = callback;
}

void ScrollBase::replaceScrollItem(Node *srcNode, Node *toNode)
{
	int index = m_vectorItem.getIndex(srcNode);
	if (index != -1)
		m_vectorItem.replace(index, toNode);
}
