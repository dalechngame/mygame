#include "GestureSprite.h"
#include "Garlandlayer.h"

USING_NS_CC;

#define CALL_DOEVENT(_selector) if (_selector) { _selector(dt); }
#define CALL_DOEVENT_BOOL(_selector) return !_selector || _selector(dt);

GestureSprite* GestureSprite::create(GarlandLayer *garland, Node *spr)
{
	GestureSprite* deco = new (std::nothrow) GestureSprite();
	if (deco && deco->init(garland, spr))
	{
		deco->autorelease();
		return deco;
	}

	CC_SAFE_DELETE(deco);

	return NULL;
}

bool GestureSprite::init(GarlandLayer *garland, Node *spr)
{
	if (!Sprite::init())
		return false;

	m_garland = garland;
	m_gues = dynamic_cast<Sprite *>(spr);
	xMove = 30;
	yMove = 80;

	m_listener = EventListenerTouchOneByOne::create();
	m_listener->onTouchBegan = CC_CALLBACK_2(GestureSprite::onTouchBegan, this);
	m_listener->onTouchMoved = CC_CALLBACK_2(GestureSprite::onTouchMoved, this);
	m_listener->onTouchEnded = CC_CALLBACK_2(GestureSprite::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);

	m_listener->setEnabled(false);
	m_gues->setVisible(false);

	return true;
}

void GestureSprite::funCallback(float dt)
{
	doEventTouchCallBack(dt);
}

bool GestureSprite::onTouchBegan(Touch *touch, Event *unused_event)
{
	prePos = touch->getLocation();
	scheduleOnce(schedule_selector(GestureSprite::funCallback), 0.3f);

	return true;
}

void GestureSprite::onTouchMoved(Touch *touch, Event *unused_event)
{
	curPos = touch->getLocation();
}

void GestureSprite::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto pos = touch->getLocation();
}

void GestureSprite::playAnimation(bool play)
{
	if (play)
	{
		m_gues->stopAllActions();
		PUtils::playNodeCircleTipForever(m_gues);
	}
	else
	{
		m_gues->stopAllActions();
	}
}

void GestureSprite::doEventTouchCallBack(float dt)
{
	auto sub = Vec2(prePos, curPos);

	if (sub.x > xMove && sub.y < -yMove)
	{
		m_garland->playGarlandFinished();
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

void GestureSprite::setGuestureVisible(bool visible)
{
	m_gues->setVisible(visible);
	m_listener->setEnabled(visible);

	playAnimation(visible);
}

