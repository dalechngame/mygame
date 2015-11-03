#include "Scissor.h"
#include "GestureSprite.h"

#define STAYTIME 3000 // ÇÐ»»Ðü¸¡Ì¬¼ä¸ô
#define SECOND_INTO_TIME 80 // Åö×²¼ä¸ô

#define NODE_DELAY_CALLFUNC 1314 // cutÇÐ»»Ðü¸¡tag

Scissor* Scissor::create(WJLayerJson *layer, GarlandLayer *garland)
{
	Scissor* deco = new (std::nothrow) Scissor();
	if (deco && deco->init(layer, garland))
	{
		deco->autorelease();
		return deco;
	}

	CC_SAFE_DELETE(deco);

	return NULL;
}

bool Scissor::init(WJLayerJson *layer, GarlandLayer *garland)
{
	if (!WJLayer::init())
	{
		return false;
	}

	m_garland = garland;
	m_json = layer;
	m_cutState = CutState::NORMAL;

	initCut(layer);
	//DRAWDEBUG(m_staticCut, 50, 50, -120, -70);

	return true;
}

void Scissor::onEnterTransitionDidFinish()
{
	WJLayer::onEnterTransitionDidFinish();

	setStaticCutVisible(true);
}

bool Scissor::onTouchAble(Node *node, WJTouchEvent *event)
{
	m_startTime = WJUtils::millisecond();
	scheduleUpdate();

	Rect srcRect = node->getBoundingBox();
	Rect destRect(srcRect.origin.x + 70, srcRect.origin.y, srcRect.size.width - 70, srcRect.size.height - 50);
	Vec2 targetPoint(event->x, event->y);

	if (destRect.containsPoint(targetPoint))
	{
		playAniWithCall(m_staticCut, false);

		m_open->setVisible(true);
		m_close->setVisible(false);
		return true;
	}

	return false;
}

bool Scissor::OnWillMoveTo(Node *node, WJTouchEvent *event)
{
	m_collidCurrent = WJUtils::millisecond();
	if (m_collidCurrent - m_collidStart < SECOND_INTO_TIME)
	{
		return true;
	}
	m_collidStart = m_collidCurrent;

	MarkLeaf *leaf = nullptr;
	if ((leaf = m_garland->isCutCollidedWithGarland(m_staticCut)))
	{
		m_cutState = CutState::CUT;

		// Í£Ö¹ËùÓÐµÄÐü¸¡Ð§¹û
		this->stopActionByTag(NODE_DELAY_CALLFUNC);
		unscheduleUpdate();

		changeDyna(true);
		m_staticCut->stopAllActions();

		m_cut->setTimeScale(0.8f);
		m_cut->playAnimation("aniCut", false);
		m_cut->setCompleteListener([=](int trackIndex, int){
			if (m_garland->m_leafVec.size() == 0)
			{
				Vec2 pos(m_garland->getGarland()->convertToNodeSpace(m_garland->getGarland()->getPositionWorld()));
				PUtils::playParticle("particles/magic001.plist", m_garland->getGarland(), pos, ZORDER_PROGRESS);

				m_garland->getGarland()->playAnimation("aniShake", true);
				m_garland->getGarland()->setTimeScale(0.5f);

				m_garland->getArrow()->setGuestureVisible(true);

			}
			PUtils::delayPlayFunc(this, CallFunc::create([=](){
				if (m_cut->isSkeletonAniCompleted(0) && !m_staticCut->isVisible() && !(m_cutState == CutState::SUSPAN))
				{
					changeDyna(false);

					m_staticCut->stopAllActions();
					m_staticCut->runAction(RepeatForever::create(
						Sequence::create(

						MoveBy::create(1.0f, Vec2(0, 15)),
						MoveBy::create(1.0f, Vec2(0, -15)),
						CallFunc::create([=](){m_cutState = CutState::SUSPAN; }),

						nullptr)));
				}

			}), 3.f)->setTag(NODE_DELAY_CALLFUNC);

		});

		PUtils::delayPlayFunc(this, CallFunc::create([=](){
			m_garland->getGarland()->playAnimation("aniShake", false);
			m_garland->getGarland()->addAnimation("aniShake", false);
			m_garland->getGarland()->setTimeScale(2.0f);

		}), 0.3f);
		if (leaf->weight == 2)
		{
			m_garland->getGarland()->slotFadeTo(leaf->leafName.c_str(), 1.0f, 175);
		}
		else if (leaf->weight == 1)
		{
			m_garland->getGarland()->slotFadeTo(leaf->leafName.c_str(), 1.0f, 125);
		}
		else if (leaf->weight == 0)
		{
			m_garland->getGarland()->slotFadeOut(leaf->leafName.c_str(), 1.0f);
		}

	}

	return true;
}

void Scissor::onTouchEnded(Node *node, WJTouchEvent *event)
{
	m_startTime = m_currTime = 0;
	unscheduleUpdate();

	m_staticCut->runAction(
		CallFunc::create([=](){

		m_cutState = CutState::NORMAL;
		m_staticCut->stopAllActions();
		m_staticCut->restoreSavedPosition(0.3f);

		changeDyna(false);
	}));
}

void Scissor::onClick(Node *node, WJTouchEvent *event)
{

}

void Scissor::playAniWithCall(Node *node, bool withCall /*= true*/)
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

void Scissor::initCut(WJLayerJson *layer)
{
	m_staticCut = layer->getSubLayer("scissorIcon");

	m_staticCut->setOnClick(CC_CALLBACK_2(Scissor::onClick, this));
	m_staticCut->setOnTouchAble(CC_CALLBACK_2(Scissor::onTouchAble, this));
	m_staticCut->setOnWillMoveTo(CC_CALLBACK_2(Scissor::OnWillMoveTo, this));
	m_staticCut->setOnTouchEnded(CC_CALLBACK_2(Scissor::onTouchEnded, this));

	m_staticCut->setScale(0.8f);
	m_staticCut->saveCurrentScale();
	m_staticCut->noClickMoveEffect();

	m_open = dynamic_cast<WJSprite *>(m_staticCut->getChildByName("scissor_open"));
	m_close = dynamic_cast<WJSprite *>(m_staticCut->getChildByName("scissor_close"));
	m_open->setVisible(false);

	m_cut = layer->getSubSkeleton("cut");
	m_cut->followNode(m_open);
	m_cut->setVisible(false);

}

void Scissor::update(float)
{
	if (m_currTime - m_startTime >= STAYTIME &&m_staticCut->getPosition() != m_staticCut->getSavedPosition() && !(m_cutState == CutState::SUSPAN))
	{
		if (m_cut->isSkeletonAniCompleted(0))
		{
			changeDyna(false);

			m_staticCut->stopAllActions();
			m_staticCut->runAction(RepeatForever::create(
				Sequence::create(

				MoveBy::create(1.0f, Vec2(0, 15)),
				MoveBy::create(1.0f, Vec2(0, -15)),
				CallFunc::create([=](){m_cutState = CutState::SUSPAN; }),
				nullptr)));
		}

		unscheduleUpdate();
	}
	m_currTime = WJUtils::millisecond();
}

void Scissor::setStaticCutVisible(bool visible, float delayTime)
{
	if (visible)
	{
		m_staticCut->runAction(Sequence::create(
			DelayTime::create(delayTime),
			EaseBackOut::create(MoveBy::create(0.5f, Vec2(0, 400))),
			CallFunc::create([=](){
			m_staticCut->saveCurrentPosition();
		}),
			nullptr
			));
	}
	else
	{
		m_staticCut->runAction(Sequence::create(
			DelayTime::create(delayTime),
			EaseBackOut::create(MoveBy::create(1.0f, Vec2(0, -400))),
			CallFunc::create([=](){
			m_staticCut->saveCurrentPosition();
		}),
			nullptr
			));
	}
}

bool Scissor::isCutFinished()
{
	return m_cut->isSkeletonAniCompleted(0);
}

void Scissor::changeDyna(bool change)
{
	if (change)
	{
		m_staticCut->setVisible(false);
		m_cut->setVisible(true);
	}
	else
	{
		m_staticCut->setVisible(true);
		m_cut->setVisible(false);

		m_close->setVisible(true);
		m_open->setVisible(false);
	}
}

