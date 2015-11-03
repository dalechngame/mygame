#include "Snowman.h"
#include "P003Snowman.h"
#include "Tools.h"

int Snowman::enterTimes;

Snowman* Snowman::create(WJLayerJson *layer, PBase *parent)
{
	Snowman* deco = new (std::nothrow) Snowman();
	if (deco && deco->init(layer, parent))
	{
		deco->autorelease();
		return deco;
	}

	CC_SAFE_DELETE(deco);

	return NULL;
}

bool Snowman::init(WJLayerJson *layer, PBase *parent)
{
	if (!WJLayer::init())
	{
		return false;
	}

	m_json = layer;
	m_parent = parent;
	m_drift = layer->getSubSprite("drift");
	enterTimes = 0;

	initSnowman(layer);
	initGlove();
	//DRAWDEBUG(m_snowman, 0, 0,-0, -0);

	return true;
}

void  Snowman::onEnterTransitionDidFinish()
{
	WJLayer::onEnterTransitionDidFinish();

}

ProgressStar  Snowman::getProgressState()
{
	return dynamic_cast<P003 *>(m_parent)->getProgressStarNum();
}

void Snowman::setProgressState(ProgressStar state)
{
	m_progressState = state;
	dynamic_cast<P003*>(m_parent)->setProgressStarNum(state);
}

void Snowman::initSnowman(WJLayerJson *layer)
{
	WJSprite *snowman = layer->getSubSprite("snowman");
	//snowman->setVisible(false);

	m_snowman = WJSkeletonAnimation::createWithFile("game/skeleton/p003/snowman/snowman");
	m_json->addChild(m_snowman, 100);

	m_snowman->setPosition(snowman->getPosition()-Vec2(0, 250));
	m_snowman->setVisible(false);
	m_snowman->noClickMoveEffect();
	m_snowman->setEnabled(false);
	m_snowman->setOnClick(CC_CALLBACK_2(Snowman::onClickSnowman, this));

	dealWithSnow();
}

void Snowman::makeSnowman(Tools *shovel)
{
	if (PUtils::isOneCollideWithOne(shovel->getShovel(), m_json->getSubSprite("snowman001")))
	{
		enterTimes++;
		std::string temp = WJUtils::stringAddInt("snowman", enterTimes, 3);
		Node *snow = m_json->getSubSprite(temp.c_str());
		if (snow)
		{
			snow->setOpacity(0);
			snow->setVisible(true);
			snow->runAction(FadeIn::create(0.5f));
			m_drift->runAction(ScaleBy::create(0.3f, m_drift->getScale()*0.95));
		}
		if (enterTimes == 5)
		{
			m_drift->runAction(FadeOut::create(0.5f));

			m_snowman->setOpacity(0);
			m_snowman->setVisible(true);
			m_snowman->runAction(Sequence::create(
				FadeIn::create(0.5f),
				CallFunc::create([=](){
				m_snowman->playAnimation("aniClick", false);
				m_snowman->addAnimation("aniIdle", true);

				PUtils::delayPlayFunc(this, CallFunc::create([=](){
					m_gues->setGuestureVisible(true);
				}), 1.0f);
				dealWithSnow(true);

			}),
				nullptr 
				));
		}
		return;
	}
}

bool Snowman::isShovelCollidedWithDrift(Tools *shovel)
{
	if (PUtils::isOneCollideWithOne(shovel->getStaticShovel(), m_drift, -70, -70) && !(shovel->getShovelState() == ShovelState::SHOVELING))
	{
		return true;
	}

	return false;
}

void Snowman::onClickSnowman(Node *node, WJTouchEvent *event)
{
	Rect srcRect = node->getBoundingBox();
	Rect destRect(srcRect.origin.x + 200, srcRect.origin.y, srcRect.size.width - 350, srcRect.size.height -200);
	Vec2 targetPoint(event->x, event->y);

	if (destRect.containsPoint(targetPoint))
	{
		m_snowman->playAnimation("aniClick", false);
		m_snowman->addAnimation("aniIdle", true);
	}

}

void Snowman::dealWithSnow(bool remove /*= false*/)
{
	int i = 1;
	do
	{
		std::string temp = WJUtils::stringAddInt("snowman", i, 3);
		Node *snow = m_json->getSubSprite(temp.c_str());

		if (snow)
		{
			if (remove)
			{
				snow->removeFromParent();
			}
			else
			{
				snow->setVisible(false);
			}

			i++;
		}
		else
		{
			break;
		}
	} while (1);
}

void Snowman::initGlove()
{
	m_gues = MulGestureSprite::create(this, m_json->getSubSprite("gloveL"), m_json->getSubSprite("gloveR"));
	m_json->addChild(m_gues, 100);
	m_gues->setPosition(m_json->getSubSprite("gloveL")->getPosition());
}

void Snowman::removeDrift()
{
	m_drift->removeFromParent();
}

float Snowman::getSnowmanBottomPos()
{
	return m_json->getSubSprite("snowman")->getBoundingBox().getMidX();
}
