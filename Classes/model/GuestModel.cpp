// Created by wangwenjie on 2013/12

#include "GuestModel.h"
#include "Common.h"

USING_NS_CC;

#define GUEST_MODEL_NORMAL_ANI        "aniIdle"     // 播放的常态动画

GuestModel* GuestModel::create( int modelNo, bool playIdle)
{
    GuestModel *model = new GuestModel();
    model->m_modelNo = modelNo;
	model->m_playIdle = playIdle;
    model->autorelease();
    model->init();

    return model;
}

bool GuestModel::init()
{
	if (!ModelBase::init())
        return false;

	// 点击事件
	setModelClickEvent();
	// 常态动画
	setPlayNameWithChannel1(GUEST_MODEL_NORMAL_ANI);

	/* 设置动画的EventTrigger */
	m_skeleton->setEventListener(CC_CALLBACK_2(GuestModel::onSkeletonEvent, this, m_skeleton));

    return true;
}

void GuestModel::onSkeletonEvent(int stateIndex, spEvent* pEvent, WJSkeletonAnimation *skeleton)
{
	//if (!pEvent || !pEvent->stringValue)
	//	return;

	//if (m_foodCallback && strcmp(pEvent->data->name, "food") == 0)
	//{
	//	m_foodCallback(m_skeleton, m_foodSlotName, m_foodFileName, m_isFoodLoaded);
	//	m_isFoodLoaded = !m_isFoodLoaded;
	//	return;
	//}
}

void GuestModel::onEnter()
{
	ModelBase::onEnter();
}

void GuestModel::onExit()
{
	ModelBase::onExit();
}

void GuestModel::onEnterTransitionDidFinish()
{
	ModelBase::onEnterTransitionDidFinish();
	playGuestIdleAni();
}

GuestModel::GuestModel()
{

}

void GuestModel::playGuestIdleAni()
{
	if (m_playIdle)
	{
		m_skeleton->playAnimation(GUEST_MODEL_NORMAL_ANI, true, 1);
	}
}

void GuestModel::playParticleWithFace(const Vec2 &offset)
{
	Point middlePoint = Vec2(1363 / 2, 768 / 2);
	// 播放粒子特效
	ParticleSystemQuad *particle = ParticleSystemQuad::create("particles/love.plist");
	particle->setPosition(middlePoint + Vec2(0, 350) + offset);
	particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	this->addChild(particle, 2000);
}

void GuestModel::playAniWithNotPlayingOther( const char* animationName, bool loop, int stateIndex )
{
	std::string name = m_skeleton->getPlayingAnimationName(stateIndex);
	if (WJUtils::equals(name, ""))
	{
		ModelBase::playAnimation(animationName, loop, stateIndex);
	}
}

void GuestModel::setModelClickEvent()
{
	m_skeleton->setClickAble(true);
	m_skeleton->setOnClick(CC_CALLBACK_2(GuestModel::onClickGuestModel, this));
	m_skeleton->setTouchSwallowsTouches(false);
	m_skeleton->noClickMoveEffect();
	m_skeleton->setCustomizeTouchRect(Rect(-135, 70, 280, 500), true);

	//for test
	//LayerColor *colorLayerHead = LayerColor::create(Color4B(0, 255, 255, 80));
	//colorLayerHead->setContentSize(Size(280, 500));
	//colorLayerHead->setPosition(Vec2(-135, 70));
	//m_skeleton->addChild(colorLayerHead);
}

void GuestModel::onClickGuestModel(Node *node, WJTouchEvent *event)
{
	WJSkeletonAnimation *skeleton = (WJSkeletonAnimation*)node;
	// 当前已经指定了播放的动画和声音
	if (m_curAggregateStr.size() > 1)
	{
		startPlayClickStandbyAnimation();
	}
}

void GuestModel::onCompletedListener(int trackIndex, int loopCount)
{
	if (trackIndex == 1 && loopCount == 1)
	{
		std::string name = m_skeleton->getAnimationName(trackIndex);
		if (!WJUtils::equals("", m_playNameWithChannel1.c_str()) && 
			!WJUtils::equals(m_playNameWithChannel1.c_str(), name.c_str()))
		{
			playAnimation(m_playNameWithChannel1.c_str(), true, 1);
		}
	}
}
