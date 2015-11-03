// Created by YUANZHONGJI on 2015/09


#include "NoviceFinger.h"


USING_NS_CC;

NoviceFinger* NoviceFinger::create()
{
	NoviceFinger *layer = new (std::nothrow) NoviceFinger();
	if (layer && layer->init())
	{
		layer->autorelease();
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return NULL;
}

bool NoviceFinger::init()
{
	if (!WJLayer::init())
		return false;

	// init
	this->setContentSize(Size(1363, 768));
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->ignoreAnchorPointForPosition(false);

	// 手指
	m_sprite = WJSprite::create("game/03_minigame/common/ui/finger_001.png");
	m_sprite->setPosition(Vec2::ZERO);
	m_sprite->setOpacity(0);
	m_sprite->saveCurrentProperties();
	this->addChild(m_sprite, 10);

	return true;
}

void NoviceFinger::onEnter()
{
	WJLayer::onEnter();
}

void NoviceFinger::onExit()
{
	WJLayer::onExit();
}

void NoviceFinger::onEnterTransitionDidFinish()
{
	WJLayer::onEnterTransitionDidFinish();
}

void NoviceFinger::startMoveFingerTips(Vec2 &fromPointWorld, Vec2 &toPointWorld, int repeatCount, float interval)
{
	//stopMoveFingerTips();
	m_sprite->stopAllActions();
	m_sprite->setOpacity(0);
	Sequence* seq = Sequence::create(
		Place::create(convertToNodeSpace(fromPointWorld)),
		CallFunc::create([=]() {
			m_sprite->setOpacity(255); 
		}),
		MoveTo::create(fromPointWorld.distance(toPointWorld) / FINGER_SPEED, convertToNodeSpace(toPointWorld)),
		FadeOut::create(0.1f),
		DelayTime::create(interval),
		nullptr);

	if (repeatCount > 0)
	{
		m_sprite->runAction(Repeat::create(seq, repeatCount));
	}
	else if (repeatCount == 0)
	{
		m_sprite->runAction(RepeatForever::create(seq));
	}
}

void NoviceFinger::startMoveFingerTipsForever(Vec2 &fromPointWorld, Vec2 &toPointWorld, float interval)
{
	startMoveFingerTips(fromPointWorld, toPointWorld, 0, interval);
}

void NoviceFinger::stopMoveFingerTips()
{
	this->stopAllActions();
	m_sprite->stopAllActions();
	m_sprite->setOpacity(0);
}

void NoviceFinger::delayStartMoveFingerTips(Vec2 &fromPointWorld, Vec2 &toPointWorld, int repeatCount, float interval, float delay)
{
	stopMoveFingerTips();
	this->runAction(Sequence::create(
		DelayTime::create(delay),
		CCCallFunc::create(CC_CALLBACK_0(NoviceFinger::startMoveFingerTips, this, 
	fromPointWorld, toPointWorld, repeatCount, interval)),
	nullptr));
}

void NoviceFinger::delayStartMoveFingerTipsForever(Vec2 &fromPointWorld, Vec2 &toPointWorld, float interval, float delay)
{
	stopMoveFingerTips();
	// 默认循环移动手指每次移动2次
	this->runAction(Sequence::create(
			DelayTime::create(delay),
			CCCallFunc::create(CC_CALLBACK_0(NoviceFinger::firstDelayMoveFingerForever, this, fromPointWorld,toPointWorld, 2, interval, delay)),
			nullptr));
}

void NoviceFinger::firstDelayMoveFingerForever(Vec2 &fromPointWorld, Vec2 &toPointWorld, int repeatCount, float interval, float delay)
{
	startMoveFingerTips(fromPointWorld, toPointWorld, repeatCount, interval);
	this->runAction(RepeatForever::create(
		Sequence::create(
		DelayTime::create(delay + (fromPointWorld.distance(toPointWorld) / FINGER_SPEED) * repeatCount),
		CCCallFunc::create(CC_CALLBACK_0(NoviceFinger::startMoveFingerTips, this, fromPointWorld, toPointWorld, 2, interval)),
		nullptr)));
}
