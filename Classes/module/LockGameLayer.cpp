// Created by YUANZHONGJI on 2015/09
#include "LockGameLayer.h"
#include "Store.h"

USING_NS_CC;

LockGameLayer* LockGameLayer::create(GameNumber number)
{
	LockGameLayer *layer = new (std::nothrow) LockGameLayer();
	if (layer && layer->init(number))
	{
		layer->autorelease();
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return NULL;
}

bool LockGameLayer::init(GameNumber number)
{
	if (!WJLayer::init())
		return false;

	m_gameNumber = number;
	// init
	this->setContentSize(Size(1363, 768));
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setPosition(Director::getInstance()->getWinSize() / 2);
	this->ignoreAnchorPointForPosition(false);

	return true;
}

void LockGameLayer::onEnter()
{
	WJLayer::onEnter();
}

void LockGameLayer::onExit()
{
	WJLayer::onExit();
}

void LockGameLayer::onEnterTransitionDidFinish()
{
	WJLayer::onEnterTransitionDidFinish();
}

bool LockGameLayer::isGameLocked()
{
	return false;
}

void LockGameLayer::disableTouchOnScreen(float duration)
{
	const Size &size = this->getContentSize();
	if (!m_coverLayer)
	{
		LayerColor* pLayerColor = LayerColor::create(Color4B(255, 255, 255, 0), size.width, size.height);

		m_coverLayer = WJLayer::create();
		m_coverLayer->addChild(pLayerColor);
		m_coverLayer->setContentSize(size);
		m_coverLayer->ignoreAnchorPointForPosition(false);
		m_coverLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_coverLayer->setPosition(size / 2);
		this->addChild(m_coverLayer, 10);

		m_coverLayer->noClickMoveEffect();
		m_coverLayer->setMoveAble(false);
		m_coverLayer->setClickAble(true);
		m_coverLayer->setTouchSwallowsTouches(true);
		m_coverLayer->setOnClick(CC_CALLBACK_2(LockGameLayer::onCoverLayerClick, this));

		m_coverLayer->setVisible(false);
	}

	if (duration > 0.0f)
	{
		m_coverLayer->runAction(Sequence::create(Show::create(),
			DelayTime::create(duration),
			Hide::create(),
			nullptr));
	}
	else
	{
		m_coverLayer->setVisible(true);
	}
}

void LockGameLayer::onCoverLayerClick(Node *node, WJTouchEvent *event)
{
	// nothing
}

void LockGameLayer::show()
{
	if (isGameLocked())
	{
		disableTouchOnScreen(2.0f);
		this->runAction(Sequence::create(DelayTime::create(1.5f),
			CallFunc::create(CC_CALLBACK_0(LockGameLayer::showLockLayerInGame, this)),
			nullptr));
	}
	
}


void LockGameLayer::showLockLayerInGame()
{
	const Size &size = this->getContentSize();

	m_lockLayer = WJLayer::create();
	m_lockLayer->setContentSize(size);
	m_lockLayer->ignoreAnchorPointForPosition(false);
	m_lockLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_lockLayer->setPosition(size / 2);
	m_lockLayer->setTag(4403);
	this->addChild(m_lockLayer, 20);

	LayerColor* pColorLayer = LayerColor::create(Color4B(0, 0, 0, 127), size.width, size.height);
	m_lockLayer->addChild(pColorLayer);

	WJSprite* pLockSprite = WJSprite::create("game/01_common/lockBig.png");
	pLockSprite->ignoreAnchorPointForPosition(false);
	pLockSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pLockSprite->setPosition(size / 2);
	pLockSprite->setScale(0.2f);
	m_lockLayer->addChild(pLockSprite, 20);

	m_lockLayer->noClickMoveEffect();
	m_lockLayer->setClickAble(true);
	m_lockLayer->setTouchSwallowsTouches(true);
	m_lockLayer->setCustomizeTouchRect(Rect(0.0f, 0.0f, size.width, size.height), true);
	m_lockLayer->setOnClick(CC_CALLBACK_2(LockGameLayer::onBigLockClicked, this));

	m_lockLayer->setEnabled(false);

	//ParticleSystemQuad *pParticle = ParticleSystemQuad::create("particles/suo.plist");
	//pParticle->setPosition(pLockSprite->getPosition());
	//pParticle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	//m_lockLayer->addChild(pParticle, 10);

	pLockSprite->runAction(Sequence::create(
		Spawn::createWithTwoActions(
		ScaleTo::create(1.0f, 1.0f),
		RotateBy::create(1.0f, 1080.0f)),
		CCCallFunc::create([=]() {
			PUtils::playBubbleEffectWithCurrentScale(pLockSprite);
	}),
	nullptr));


	m_lockLayer->runAction(Sequence::create(DelayTime::create(1.05f),
		CallFunc::create(CC_CALLBACK_0(WJLayer::setEnabled, m_lockLayer, true, false)),
		DelayTime::create(1.0f),
		CallFunc::create([=]() {
		if (m_lockLayer->getTag() == 4403)
			onBigLockClicked(m_lockLayer, nullptr);
		}),
		nullptr))->setTag(9821);
}

void LockGameLayer::onBigLockClicked(Node* pNode, WJTouchEvent* e)
{
	WJLayer *layer = (WJLayer*)pNode;
	layer->stopActionByTag(9821);

	if (WJUtils::canClick("bigLockClick", 500))
	{
		CCLOG("--------click big lock--------");
		//if (m_gameNumber == GameNumber::P001)
		Store::showStoreMini(IAP_HAIR_PACK_KEY);

		pNode->setTag(4404);
	}
}

LockGameLayer::LockGameLayer()
{
	m_coverLayer = nullptr;
	m_lockLayer = nullptr;
}
