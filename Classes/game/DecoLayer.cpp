#include "DecoLayer.h"
#include "P002Catcher.h"
#include "ArrowPathBezier.h"
#include "ParabolaMove.h"
#include "Rect.h"

#define PLAY_CIRCLE_ANI_TAG              6655               // 提示动画

#define CALL_DOEVENT(_selector) if (_selector) { _selector(node, e); }
#define CALL_DOEVENT_BOOL(_selector) return !_selector || _selector(node, e);

DecoLayer::DecoLayer()
{
	m_originPic = nullptr;
	m_circle = nullptr;
	m_layer = nullptr;

	m_drop.clear();
}

DecoLayer::~DecoLayer()
{
	for each (Mark * mark in m_drop)
	{
		CC_SAFE_DELETE(mark);
	}
}

DecoLayer* DecoLayer::create(WJLayer *layer, int index, bool usingSke)
{
	DecoLayer* deco = new (std::nothrow) DecoLayer();
	if (deco && deco->init(layer, index, usingSke))
	{
		deco->autorelease();
		return deco;
	}

	CC_SAFE_DELETE(deco);

	return NULL;
}

bool DecoLayer::init(WJLayer *layer, int index, bool usingSke)
{
	if (!WJLayer::init())
	{
		return false;
	}
	m_layer = layer;
	m_index = index;
	m_usingSke = usingSke;
	m_colorIndex = 0;
	m_originColor = 0;

	m_layer->noClickMoveEffect();
	m_layer->setOnClick(CC_CALLBACK_2(DecoLayer::onClickCircle, this));

	initDeco(layer);
	initCircleTips();

	return true;
}

void  DecoLayer::onEnterTransitionDidFinish()
{
	WJLayer::onEnterTransitionDidFinish();

	WJSkeletonAnimation *circle = dynamic_cast<WJSkeletonAnimation*>(m_circle);

	circle->setScale(0.01);
	circle->runAction(Sequence::create(
		DelayTime::create(0.2f),
		EaseBackOut::create(ScaleTo::create(0.3f, 1.2f)),
		EaseBackOut::create(ScaleTo::create(0.3f, 1.0f)),
		DelayTime::create(0.15f),
		CallFunc::create([=](){

		circle->playAnimation("aniClick", false);
		circle->addAnimation("aniIdle", true);
		circle->addAnimation("aniClick", false, 0, circle->getAnimation("aniIdle")->duration *2.0f);
		circle->setCompleteListener([=](int trackIndex, int loopCount)
		{

			if (WJUtils::equals(circle->getAnimationName(trackIndex), "aniClick"))
			{
				playSkeAni(0, false);
				playDropAni();
			}
			else if (WJUtils::equals(circle->getAnimationName(trackIndex), "aniIdle") &&
				!(dynamic_cast<P002*>(this->getParent()->getParent())->getCircleState() == CircleState::ORIGIN ||
				dynamic_cast<P002*>(this->getParent()->getParent())->getCircleState() == CircleState::COLOR))
			{
				circle->stopAnimation(0);
				circle->addAnimation("aniIdle", true, 0);
			}

		});

	}), nullptr));


}

void DecoLayer::initDeco(WJLayer *layer)
{
	m_originPic = dynamic_cast<WJSpriteImageView*>
		(layer->getChildByName(WJUtils::stringAddInt("image_", m_index + 1, 1)));

	if (m_usingSke)
	{
		m_circle = WJSkeletonAnimation::createWithFile(WJUtils::stringAddInt("game/skeleton/p002/drop/drop", m_index + 1, 2).c_str());

		m_layer->addChild(m_circle, 100);
		m_circle->setPosition(m_originPic->getPosition());

		m_originPic->removeFromParent();

	}

	int i = 1;
	do
	{
		WJSkeletonAnimation *circle = dynamic_cast<WJSkeletonAnimation *>(m_circle);
		circle->update(0);

		std::string temp = WJUtils::stringAddInt("", i, 3);
		spBone *bone = circle->findBone(temp);

		if (bone)
		{
			Mark *mark = new Mark;
			mark->name = temp;
			mark->bonePos = circle->getBoneWorldPosition(temp.c_str());

			m_drop.push_back(mark);
			m_randomInt.push_back(i - 1);

			i++;
		}
		else
		{
			break;
		}
	} while (1);

}

void DecoLayer::seeThis(bool canSee)
{
	m_layer->setVisible(canSee);
}

void DecoLayer::changeCircle(int index, Node *newSpr)
{
	WJSkeletonAnimation *circle = dynamic_cast<WJSkeletonAnimation*>(newSpr);

	PUtils::addWJChildToAnotherParent(circle, m_layer);
	scheduleUpdate();
	circle->setPosition(m_circle->getPosition());

	m_circle->removeFromParent();
	m_circle = circle;

	// 播放粒子
	PUtils::playParticle("particles/dream.plist", circle, circle->convertToNodeSpace(circle->getPositionWorld()), ZORDER_PROGRESS);

	// 播放drop的动画
	playDropAni();

	circle->playAnimation("aniClick", false);
	circle->setCompleteListener([=](int trackIndex, int loopCount)
	{
		switch (trackIndex)
		{
		case 0:
			if (WJUtils::equals(circle->getAnimationName(trackIndex), "aniClick"))
			{
				playSkeAni(1, false);
			}
			else if (WJUtils::equals(circle->getAnimationName(trackIndex), "aniIdle") &&
				!(dynamic_cast<P002*>(this->getParent()->getParent())->getCircleState() == CircleState::ORIGIN ||
				dynamic_cast<P002*>(this->getParent()->getParent())->getCircleState() == CircleState::COLOR))
			{
				circle->stopAllAnimation();
				circle->addAnimation("aniIdle", true, 1);
			}

			break;
		case 1:
			if (WJUtils::equals(circle->getAnimationName(trackIndex), "aniClick"))
			{
				playSkeAni(1, false);
			}
			else 	if (WJUtils::equals(circle->getAnimationName(trackIndex), "aniIdle") &&
				!(dynamic_cast<P002*>(this->getParent()->getParent())->getCircleState() == CircleState::ORIGIN ||
				dynamic_cast<P002*>(this->getParent()->getParent())->getCircleState() == CircleState::COLOR))
			{
				circle->stopAllAnimation();
				circle->addAnimation("aniIdle", true, 1);
			}

			break;

		default:
			break;
		}


	});


}

Mark* DecoLayer::isCollisioned(Node *destSpr)
{
	for each (Mark *mark in m_drop)
	{
		if (PUtils::isOneCollideWithOne(mark->bonePos, destSpr, 0, 0, 0, 10.f))
		{
			return mark;
		}
	}

	return nullptr;
}

void DecoLayer::changeDrop(Node *newSpr, Mark*mark, int index)
{
	WJSkeletonAnimation *drop = dynamic_cast<WJSkeletonAnimation*>(newSpr);

	drop->playAnimation("aniClick", false, 0);
	drop->setCompleteListener([=](int trackIndex, int loopCount)
	{
		if (WJUtils::equals(drop->getAnimationName(trackIndex), "aniClick"))
		{
			drop->playAnimation("aniIdle", true);
		}

	});

	PUtils::addWJChildToAnotherParent(drop, m_layer);

	if (mark->drop)
	{
		mark->drop->removeFromParent();
		dynamic_cast<WJSkeletonAnimation*>(m_circle)->removeBindNode(mark->drop);
	}

	dynamic_cast<WJSkeletonAnimation*>(m_circle)->bindNodeToBone(drop, mark->name, Vec2::ZERO);

	mark->drop = drop;
	mark->index = index;
	mark->isPlaced = true;
	stopplayCircleAni();
	startplayCircleAni(1.5f);
}

void DecoLayer::playScaleAniWithCall(Node *spr, bool withCall)
{
	WJBase *base = WJBase::convertToWJBase(spr);

	if (!withCall)
	{
		if (base)
		{
			spr->runAction(
				Sequence::create(
				ScaleTo::create(0.15f, 1.01f * base->getSavedScale()),
				ScaleTo::create(0.15f, 0.99f * base->getSavedScale()),
				ScaleTo::create(0.15f, 1.01f * base->getSavedScale()),
				ScaleTo::create(0.15f, 1.00f * base->getSavedScale()),
				nullptr));
		}
	}
	else
	{
		if (base)
		{
			WJSkeletonAnimation *circle = dynamic_cast<WJSkeletonAnimation*>(m_circle);
			spr->runAction(
				Sequence::create(
				ScaleTo::create(0.15f, 1.01f * base->getSavedScale()),
				ScaleTo::create(0.15f, 0.99f * base->getSavedScale()),
				ScaleTo::create(0.15f, 1.01f * base->getSavedScale()),
				ScaleTo::create(0.15f, 0.99f * base->getSavedScale()),
				CallFunc::create([=](){PUtils::playParticle("particles/dream.plist", circle, circle->convertToNodeSpace(circle->getPositionWorld()), ZORDER_PROGRESS); }),
				nullptr));
		}
	}
}

void DecoLayer::initCircleTips()
{
	for each (Mark *mark in m_drop)
	{
		mark->tip1 = WJSprite::create("game/03_minigame/p008/cicle001.png");
		mark->tip1->setPosition(m_layer->convertToNodeSpace(mark->bonePos));
		mark->tip1->setOpacity(200);
		mark->tip1->setVisible(false);
		mark->tip1->saveCurrentProperties();
		m_layer->addChild(mark->tip1, 900);

		mark->tip2 = WJSprite::create("game/03_minigame/p008/cicle001.png");
		mark->tip2->setPosition(m_layer->convertToNodeSpace(mark->bonePos));
		mark->tip2->setOpacity(200);
		mark->tip2->setVisible(false);
		mark->tip2->saveCurrentProperties();
		m_layer->addChild(mark->tip2, 901);
	}

}

void DecoLayer::startplayCircleAni(float delay)
{
	float time = 0.9f;

	for each (Mark *mark in m_drop)
	{
		if (!mark->isPlaced)
		{
			this->runAction(RepeatForever::create(
				Sequence::create(
				DelayTime::create(delay),
				CCCallFunc::create([=]() {
				runCircleScaleAction(mark->tip1);
			}),
				DelayTime::create(time),
				CCCallFunc::create([=]() {
				runCircleScaleAction(mark->tip2);
			}),
				DelayTime::create(time),
				CCCallFunc::create([=]() {
				runCircleScaleAction(mark->tip1);
			}),
				DelayTime::create(time),
				CCCallFunc::create([=]() {
				runCircleScaleAction(mark->tip2);
			}),
				DelayTime::create(time),
				CCCallFunc::create([=]() {
				runCircleScaleAction(mark->tip1);
			}),
				DelayTime::create(time),
				CCCallFunc::create([=]() {
				runCircleScaleAction(mark->tip2);
			}),
				DelayTime::create(3.0f),
				nullptr)))->setTag(PLAY_CIRCLE_ANI_TAG);
		}

	}
}

void DecoLayer::stopplayCircleAni()
{
	for each (Mark *mark in m_drop)
	{
		this->stopActionByTag(PLAY_CIRCLE_ANI_TAG);
		mark->tip1->stopAllActions();
		mark->tip2->stopAllActions();
		mark->tip1->setVisible(false);
		mark->tip2->setVisible(false);
	}

}

void DecoLayer::runCircleScaleAction(WJSprite *sprite)
{
	sprite->stopAllActions();
	sprite->setOpacity(0);
	sprite->setVisible(true);
	sprite->restoreSavedScale();

	sprite->runAction(Sequence::create(
		DelayTime::create(0.1f),
		ScaleTo::create(1.2f, 0.01f * sprite->getSavedScale()),
		nullptr));
	sprite->runAction(Sequence::create(
		DelayTime::create(0.1f),
		FadeTo::create(0.3f, 0.7f * sprite->getSavedOpacity()),
		FadeTo::create(0.9f, 0.1f * sprite->getSavedOpacity()),
		nullptr));
}

void DecoLayer::onClickCircle(Node* node, WJTouchEvent* touchEvent)
{
	doEventClickCircleCallBack(this, touchEvent);
}

void DecoLayer::doEventClickCircleCallBack(Node *node, WJTouchEvent *e)
{
	CALL_DOEVENT(m_clickCircleCallBack);

	dynamic_cast<WJSkeletonAnimation *>(m_circle)->playAnimation("aniClick", false, 1);
	playDropAni();
}

void DecoLayer::playSkeAni(int trackIndex, bool playParticle)
{
	WJSkeletonAnimation *circle = dynamic_cast<WJSkeletonAnimation*>(m_circle);

	if (playParticle)
	{
		PUtils::playParticle("particles/dream.plist", circle, circle->convertToNodeSpace(circle->getPositionWorld()), ZORDER_PROGRESS);
	}

	circle->addAnimation("aniIdle", true, trackIndex);
	circle->addAnimation("aniClick", false, trackIndex, circle->getAnimation("aniIdle")->duration *2.0f);


}

void DecoLayer::playDropAni()
{
	for (auto it = m_drop.begin(); it != m_drop.end(); it++)
	{
		Mark *mark = *it;
		if (mark->isPlaced)
		{
			WJSkeletonAnimation *drop = dynamic_cast<WJSkeletonAnimation*>(mark->drop);

			drop->playAnimation("aniClick", false, 0);
			float speed = random(0.7f, 1.3f);
			drop->setTimeScale(speed);
			drop->setCompleteListener([=](int trackIndex, int loopCount)
			{
				if (WJUtils::equals(drop->getAnimationName(trackIndex), "aniClick"))
				{
					drop->playAnimation("aniIdle", true);
				}

			});
		}
	}

}

Mark * DecoLayer::getRandomMark(int index)
{
	int i = 0;
	for (auto it = m_drop.begin(); it != m_drop.end(); it++, i++)
	{
		Mark *mark = *it;
		if (mark->isPlaced)
		{
			PUtils::removeChiledOfVector(m_randomInt, i);
		}
	}
	if (m_randomInt.size() == 0)
	{
		for (int i = 0; i < m_drop.size(); i++)
		{
			m_randomInt.push_back(i);
		}
	}

	Mark *child = m_drop.at(m_randomInt.at(WJUtils::randomInt(m_randomInt.size())));
	child->isPlaced = true;

	return child;
}


void DecoLayer::changeDeco(int index, Node* newSpr)
{
	WJSprite *deco = dynamic_cast<WJSprite*>(newSpr);

	Vec2 pos(m_circle->convertToNodeSpace(deco->getPositionWorld()));
	PUtils::addWJChildToAnotherParent(deco, m_circle);
	deco->setPosition(pos);
	deco->setMoveAble(true);
	deco->setOnTouchAble(CC_CALLBACK_2(DecoLayer::onTouchDeco, this));
	
	// 播放粒子
	PUtils::playParticle("particles/magic.plist", deco, deco->convertToNodeSpace(deco->getPositionWorld()), ZORDER_PROGRESS);
}

bool DecoLayer::onTouchDeco(Node *node, WJTouchEvent *touchEvent)
{
	m_posCalc.currentDeco = node;
	m_parabolaCalc.start = WJUtils::millisecond();
	m_posCalc.startPos = node->getPosition();

	schedule(schedule_selector(DecoLayer::calc));

	return true;
}

void DecoLayer::calc(float)
{
	m_parabolaCalc.current = WJUtils::millisecond();
	m_posCalc.currentPos = m_posCalc.currentDeco->getPosition();
	float dis = m_posCalc.currentPos.getDistance(m_posCalc.startPos);

	Vec2 shootVector = m_posCalc.currentPos - m_posCalc.startPos;		//方向向量
	shootVector.normalize();
	float radians = atan2(shootVector.y, shootVector.x);				//算出旋转的弧度
	float degrees = CC_RADIANS_TO_DEGREES(radians);					 //将弧度转换成角度

	float duration = 0;
	if (!PUtils::isOneCollideWithOne(m_circle, m_posCalc.currentDeco))
	{
		Vec2 startPos(dynamic_cast<WJBase *>(m_posCalc.currentDeco)->getPositionWorld());
		ccQuadBezierConfig bezier1;
		if ((degrees <-10 && degrees >= -180))
		{
			/*m_posCalc->currentDeco->runAction(Spawn::create(

				));*/
		}
		else
		{
			if ((degrees <= 90 && degrees >= -10))
			{
				Vec2 ctrlPos(startPos.getMidpoint(LeftBottom));
				bezier1 = {
					m_posCalc.currentDeco->convertToNodeSpace(Vec2(random(Bottom.getMidpoint(RightBottom).x, RightBottom.x), 0)),
					ctrlPos,
				};
			}
			else if ((degrees >= 90 && degrees <= 180))
			{
				Vec2 ctrlPos(startPos.getMidpoint(LeftBottom) - Vec2(100, 100));
				bezier1 = {
					m_posCalc.currentDeco->convertToNodeSpace(Vec2(WJUtils::randomInt(Bottom.getMidpoint(LeftBottom).x), 0)),
					ctrlPos,
				};
			}

			auto action = ArrowPathBezier::create(0.8, bezier1);
			m_posCalc.currentDeco->runAction(Sequence::create(
				action,
				FadeOut::create(0.5f),
				nullptr
				));
		}
	

		unschedule(schedule_selector(DecoLayer::calc));
	}

	CCLOG("%f %f %d", dis, degrees ,m_parabolaCalc.current-m_parabolaCalc.start);
}

/*
if (m_randomInt.size() == 0 || m_randomInt.size() ==m_drop.size())
{
for (int i = 0; i < m_drop.size(); i++)
{
bool isThesame = false;
for each(Mark *mark in m_drop)
{
if (mark->index == index)
{
isThesame = true;
break;
}
}
if (isThesame)
{
break;
}
else
{
m_randomInt.push_back(i);
}
}
}
*/




