// Created by wangwenjie on 2014/01

#include "Common.h"
#include "ProgressBar.h"

USING_NS_CC;

ProgressBar* ProgressBar::create(const char * jsonFile)
{
	ProgressBar *layer = new ProgressBar();
	if (layer && layer->initFromJsonFile(jsonFile))
	{
		layer->autorelease();
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return NULL;
}

bool ProgressBar::initFromJsonFile(const char * jsonFile)
{
    if (!WJLayerJson2x::initFromJsonFile(jsonFile))
        return false;

	const Size &size = Director::getInstance()->getWinSize();
	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->ignoreAnchorPointForPosition(false);
	this->setPosition(Vec2(size.width / 2, size.height / 2));

    return true;
}

void ProgressBar::onEnterTransitionDidFinish()
{
	WJLayerJson2x::onEnterTransitionDidFinish();
}

ProgressBar::ProgressBar()
{
	m_progressStar = ProgressStar::noStar;
}

void ProgressBar::resetProgressStar()
{
	// 没有星星
	if (m_progressStar == ProgressStar::noStar)
	{
		loadStarSpriteTexture("star1", false);
		loadStarSpriteTexture("star2", false);
		loadStarSpriteTexture("star3", false);
	}
	// 获得一颗星星
	else if (m_progressStar == ProgressStar::oneStar)
	{
		loadStarSpriteTexture("star1", true);
		loadStarSpriteTexture("star2", false);
		loadStarSpriteTexture("star3", false);
	}
	// 获得二颗星星
	else if (m_progressStar == ProgressStar::twoStar)
	{
		loadStarSpriteTexture("star1", true);
		loadStarSpriteTexture("star2", true);
		loadStarSpriteTexture("star3", false);
	}
	// 获得三颗星星
	else if (m_progressStar == ProgressStar::threeStar)
	{
		loadStarSpriteTexture("star1", true);
		loadStarSpriteTexture("star2", true);
		loadStarSpriteTexture("star3", true);
	}
}

void ProgressBar::loadStarSpriteTexture(const char * key, bool light)
{
	WJSprite *sprite = this->getSubSpriteByKey(key);
	sprite->setVisible(light);
}

ProgressStar ProgressBar::getStarNum()
{
	return m_progressStar;
}

void ProgressBar::setProgressStarNum( ProgressStar star, bool playSound)
{
	ProgressStar lastStar = getStarNum();
	// 当前已经满足条件、或者当前的星星的数量已经大于要设置的星星数量
	if (star <= lastStar)
		return;

	m_progressStar = star;
	resetProgressStar();
	creatParticleFromStar(lastStar, star, playSound);
}

void ProgressBar::creatParticleFromStar(ProgressStar last, ProgressStar now, bool playSound)
{
	if (now == ProgressStar::oneStar)
	{
		playSoundWithStar("Common:0073", playSound);
		playSoundWithStar("Common:0047_princess", playSound);
		creatParticleFromKey("star1");
	}
	else if (now == ProgressStar::twoStar)
	{
		playSoundWithStar("Common:0073", playSound);
		playSoundWithStar("Common:0048_princess", playSound);
		creatParticleFromKey("star2");
	}
	else if (now == ProgressStar::threeStar)
	{
		playSoundWithStar("Common:0073", playSound);
		playSoundWithStar("Common:0049_princess", playSound);
		creatParticleFromKey("star3");
	}
}

void ProgressBar::playSoundWithStar(const char * key, bool playSound)
{
	if (playSound)
	{
		Common::sound.play(key);
	}
}

void ProgressBar::creatParticleFromKey(const char *key)
{
	Common::sound.play("Common:0039");
	WJSprite *sprite = this->getSubSpriteByKey(key);
	ParticleSystemQuad *particle = ParticleSystemQuad::create("particles/starRank001.plist");
	particle->setPosition(sprite->getPosition());
	sprite->getParent()->addChild(particle, 1000);
}
