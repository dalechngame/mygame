#include "MulGestureSprite.h"
#include "Snowman.h"
#include "Rect.h"

USING_NS_CC;

#define CALL_DOEVENT(_selector) if (_selector) { _selector(dt); }
#define CALL_DOEVENT_BOOL(_selector) return !_selector || _selector(dt);

static int touchTimes;

MulGestureSprite* MulGestureSprite::create(Snowman *snowman, Node *left, Node *right)
{
	MulGestureSprite* deco = new (std::nothrow) MulGestureSprite();
	if (deco && deco->init(snowman, left, right))
	{
		deco->autorelease();
		return deco;
	}

	CC_SAFE_DELETE(deco);

	return NULL;
}

bool MulGestureSprite::init(Snowman *snowman, Node *left, Node *right)
{
	if (!Sprite::init())
		return false;

	touchTimes = 0;
	m_snowman = snowman;
	m_guesLeft = dynamic_cast<Sprite *>(left);
	m_guesRight = dynamic_cast<Sprite *>(right);

	m_leftPos.xMove = m_rightPos.xMove = 30;
	m_leftPos.xMove = m_rightPos.yMove = 50;
	Rect touchRectR(snowman->getSnowmanBottomPos(), 0, VisibleRect::getVisibleRect().size.width / 2, VisibleRect::getVisibleRect().size.height / 2);
	Rect touchRectL(0, 0, VisibleRect::getVisibleRect().size.width / 2, VisibleRect::getVisibleRect().size.height / 2);

	m_leftPos.touchRect =touchRectL;
	m_rightPos.touchRect = touchRectR;

	m_listener = EventListenerTouchAllAtOnce::create();
	m_listener->onTouchesBegan = CC_CALLBACK_2(MulGestureSprite::onTouchesBegan, this);
	m_listener->onTouchesMoved = CC_CALLBACK_2(MulGestureSprite::onTouchesMoved, this);
	m_listener->onTouchesEnded = CC_CALLBACK_2(MulGestureSprite::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);

	left->setVisible(false);
	right->setVisible(false);
	m_listener->setEnabled(false);

	return true;
}

void MulGestureSprite::funCallback(float dt)
{
	doEventTouchCallBack(dt);
}

void MulGestureSprite::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
	Node *parent = m_guesRight->getParent();
	for (auto &item : touches)
	{
		if (m_leftPos.touchRect.containsPoint(parent->convertToNodeSpace(item->getLocation())))
		{
			m_leftPos.startPos = item->getLocation();
		}
		else 	if (m_rightPos.touchRect.containsPoint(parent->convertToNodeSpace(item->getLocation())))
		{
			m_rightPos.startPos = item->getLocation();
		}
		/*if (PUtils::isOneCollideWithOne(m_guesLeft->getParent()->convertToNodeSpace(item->getLocation()), m_guesLeft))
		{
			m_leftPos.startPos = item->getLocation();
		}
		else if (PUtils::isOneCollideWithOne(m_guesRight->getParent()->convertToNodeSpace(item->getLocation()), m_guesRight))
		{
			m_rightPos.startPos = item->getLocation();
		}*/
	}
	scheduleOnce(schedule_selector(MulGestureSprite::funCallback), 0.3f);
}

void MulGestureSprite::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
	Node *parent = m_guesRight->getParent();
	for (auto &item : touches)
	{
		if (m_leftPos.touchRect.containsPoint(parent->convertToNodeSpace(item->getLocation())))
		{
			m_leftPos.curPos = item->getLocation();
		}
		else 	if (m_rightPos.touchRect.containsPoint(parent->convertToNodeSpace(item->getLocation())))
		{
			m_rightPos.curPos = item->getLocation();
		}
	}
	//for (auto &item : touches)
	//{
	//	if (PUtils::isOneCollideWithOne(m_guesLeft->getParent()->convertToNodeSpace(item->getLocation()), m_guesLeft))
	//	{
	//		m_leftPos.curPos = item->getLocation();
	//	}
	//	else if (PUtils::isOneCollideWithOne(m_guesRight->getParent()->convertToNodeSpace(item->getLocation()), m_guesRight))
	//	{
	//		m_rightPos.curPos = item->getLocation();
	//	}
	//}
}

void MulGestureSprite::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
	//auto pos = touch->getLocation();
}

void MulGestureSprite::playAnimation(bool play)
{
	if (play)
	{
		m_guesLeft->stopAllActions();
		m_guesRight->stopAllActions();
		PUtils::playNodeCircleTipForever(m_guesLeft);
		PUtils::playNodeCircleTipForever(m_guesRight);
	}
	else
	{
		m_guesRight->stopAllActions();
	}
}

void MulGestureSprite::doEventTouchCallBack(float dt)
{
	auto subR = Vec2(m_rightPos.startPos, m_rightPos.curPos);
	auto subL = Vec2(m_leftPos.startPos, m_leftPos.curPos);

	if (subR.x > m_rightPos.xMove && subR.y < -m_rightPos.yMove)
	{
		touchTimes++;
	}
	else if (subR.x <-m_rightPos.xMove && subR.y > m_rightPos.yMove)
	{
		touchTimes++;
	}

	if (subL.x < -m_rightPos.xMove && subL.y < -m_rightPos.yMove)
	{
		touchTimes++;
	}
	else if (subL.x >m_rightPos.xMove && subL.y > m_rightPos.yMove)
	{
		touchTimes++;
	}

	if (touchTimes >= 4)
	{
		touchTimes = 0;
		CCLOG("succeed");
		//1. setSnowmanEnable
		//2. movein box
	}

	CALL_DOEVENT(m_touchListener);

	////判断水平和垂直方向上哪个偏移量大
	////水平移动
	//if (fabs(sub.x) > fabs(sub.y))
	//{
	//	//右滑
	//	if (sub.x > moveDistance)
	//		CCLOG("right");
	//	else if (sub.x < -moveDistance)
	//			CCLOG("left");
	//}
	//else
	//{
	//	//上滑
	//	if (sub.y > moveDistance)
	//		CCLOG("up");
	//	else if (sub.y < -moveDistance)
	//			CCLOG("down");
	//}
}

void MulGestureSprite::setGuestureVisible(bool visible)
{
	m_guesLeft->setVisible(visible);
	m_guesRight->setVisible(visible);

	m_listener->setEnabled(visible);

	playAnimation(visible);
}

void MulGestureSprite::runAnimationRM(Node *node)
{
	node->runAction(Spawn::create(RotateBy::create(0.5, 10)));
}

