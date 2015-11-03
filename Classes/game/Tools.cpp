#include "Tools.h"

Tools* Tools::create(WJLayerJson *layer, Snowman *snowman)
{
	Tools* deco = new (std::nothrow) Tools();
	if (deco && deco->init(layer, snowman))
	{
		deco->autorelease();
		return deco;
	}

	CC_SAFE_DELETE(deco);

	return NULL;
}

bool Tools::init(WJLayerJson *layer, Snowman *snowman)
{
	if (!WJLayer::init())
	{
		return false;
	}

	m_snowman = snowman;
	m_json = layer;
	m_box = layer->getSubSprite("box");

	initShovel(layer);

	return true;
}

void Tools::onEnterTransitionDidFinish()
{
	WJLayer::onEnterTransitionDidFinish();

	setShovelVisible(true);
}

bool Tools::onTouchAble(Node *node, WJTouchEvent *event)
{
	isRunactionBack = true;

	return true;
}

bool Tools::OnWillMoveTo(Node *node, WJTouchEvent *event)
{
	if (m_snowman->isShovelCollidedWithDrift(this))
	{
		isRunactionBack = false;
		m_shovelState = ShovelState::SHOVELING;
		PUtils::delayPlayFunc(
			this, CallFunc::create([=](){

			changeDyna(true);
			m_shovel->setTimeScale(1.0f);
			m_shovel->playAnimation("aniSnow01", false);
			m_shovel->setCompleteListener([=](int, int){
				if (WJUtils::equals(m_shovel->getAnimationName(0), "aniSnow02"))
				{
					m_snowman->makeSnowman(this);
					m_staticShovel->runAction(
						CallFunc::create([=](){

						m_staticShovel->setPosition(m_shovel->getPosition());
						m_staticShovel->stopAllActions();
						if (m_snowman->getSnowman()->isVisible())
						{
							m_staticShovel->setEnabled(false);
							m_staticShovel->runAction(Sequence::create(
								MoveTo::create(0.3f, m_staticShovel->getSavedPosition()),
								CallFunc::create([=](){
								setShovelVisible(false);
							}),
								nullptr
								));
						}
						else
						{
							m_staticShovel->restoreSavedPosition(0.3f);
						}

						changeDyna(false);
						m_shovelState = ShovelState::NORMAL;
					}));
				}
				else if (WJUtils::equals(m_shovel->getAnimationName(0), "aniSnow01"))
				{
					
				}
			});
		}), 0.1f);

	}

	return true;
}

void Tools::onTouchEnded(Node *node, WJTouchEvent *event)
{
	if (m_shovelState == ShovelState::SHOVELING)
	{
		PUtils::delayPlayFunc(
			m_shovel, CallFunc::create([=](){
			m_shovel->setTimeScale(2.5f);
			m_shovel->playAnimation("aniSnow02", false);
		}), 0.f);
	}

	if (isRunactionBack)
	{
		m_staticShovel->runAction(
			CallFunc::create([=](){

			m_staticShovel->stopAllActions();
			m_staticShovel->restoreSavedPosition(0.3f);

			changeDyna(false);

			m_shovelState = ShovelState::NORMAL;
			isRunactionBack = true;
		}));
	}

}

void Tools::onClick(Node *node, WJTouchEvent *event)
{

}

void Tools::playAniWithCall(Node *node, bool withCall /*= true*/)
{
	WJBase *base = WJBase::convertToWJBase(node);

	if (!withCall)
	{
		node->stopAllActions();
		node->runAction(
			Sequence::create(

			ScaleTo::create(0.15f, 1.1f* base->getSavedScale()),
			ScaleTo::create(0.15f, 1.0f* base->getSavedScale()),
			ScaleTo::create(0.15f, 0.95f* base->getSavedScale()),
			ScaleTo::create(0.15f, 1.0f * base->getSavedScale()),

			nullptr));
	}
	else
	{

		node->stopAllActions();
		node->runAction(
			Sequence::create(

			ScaleTo::create(0.15f, 1.1f* base->getSavedScale()),
			ScaleTo::create(0.15f, 1.0f* base->getSavedScale()),
			ScaleTo::create(0.15f, 1.1f* base->getSavedScale()),
			ScaleTo::create(0.15f, 1.0f * base->getSavedScale()),

			CCDelayTime::create(0.3f),

			CallFunc::create([=](){
			node->stopAllActions();
			node->runAction(RepeatForever::create(
				Sequence::create(

				MoveBy::create(1.0f, Vec2(0, 15)),
				MoveBy::create(1.0f, Vec2(0, -15)),

				nullptr)));
		}),

			nullptr));
	}

}

void Tools::initShovel(WJLayerJson *layer)
{
	m_staticShovel = layer->getSubSprite("tool");

	m_staticShovel->setOnClick(CC_CALLBACK_2(Tools::onClick, this));
	m_staticShovel->setOnTouchAble(CC_CALLBACK_2(Tools::onTouchAble, this));
	m_staticShovel->setOnWillMoveTo(CC_CALLBACK_2(Tools::OnWillMoveTo, this));
	m_staticShovel->setOnTouchEnded(CC_CALLBACK_2(Tools::onTouchEnded, this));
	m_staticShovel->noClickMoveEffect();

	m_shovel = layer->getSubSkeleton("shovel");
	m_shovel->followNode(m_staticShovel);
	m_shovel->setVisible(false);

}

void Tools::setShovelVisible(bool visible, float delayTime)
{
	if (visible)
	{
		m_staticShovel->runAction(Sequence::create(
			DelayTime::create(delayTime),
			EaseBackOut::create(MoveBy::create(0.5f, Vec2(-250, 0))),
			CallFunc::create([=](){
			m_staticShovel->saveCurrentPosition();
		}),
			nullptr
			));
	}
	else
	{
		m_staticShovel->runAction(Sequence::create(
			DelayTime::create(delayTime),
			EaseBackOut::create(MoveBy::create(1.0f, Vec2(250, 0))),
			CallFunc::create([=](){
			m_staticShovel->saveCurrentPosition();
			m_snowman->removeDrift();
		}),
			nullptr
			));
	}
}

bool Tools::isShovelFinished()
{
	return m_shovel->isSkeletonAniCompleted(0);
}

void Tools::changeDyna(bool change)
{
	if (change)
	{
		m_staticShovel->setVisible(false);
		m_shovel->setVisible(true);
	}
	else
	{
		m_staticShovel->setVisible(true);
		m_shovel->setVisible(false);
	}
}

void Tools::setBoxVisible(bool visible, float delayTime /*= 1.2f*/)
{
	if (visible)
	{
		m_box->runAction(Sequence::create(
			DelayTime::create(delayTime),
			EaseBackOut::create(MoveBy::create(0.5f, Vec2(-300, 0))),
			CallFunc::create([=](){
			m_box->saveCurrentPosition();
		}),
			nullptr
			));
	}
	else
	{
		m_box->runAction(Sequence::create(
			DelayTime::create(delayTime),
			EaseBackOut::create(MoveBy::create(1.0f, Vec2(300, 0))),
			CallFunc::create([=](){
			m_box->saveCurrentPosition();
		}),
			nullptr
			));
	}
}

