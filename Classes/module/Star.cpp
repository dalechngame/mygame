// Created by wangwenjie on 2014/01

#include "Star.h"
#include "Common.h"
#include "GameManager.h"

#define STAR_FONT_FILE			"fonts/numbers.fnt"

USING_NS_CC;

Star* Star::create()
{
	Star *layer = new Star();
	if (layer && layer->initFromJsonFile("game/json/starnumbers.json"))
	{
		layer->autorelease();
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return NULL;
}

bool Star::initFromJsonFile( const char *jsonFileName )
{
	if (!WJLayerJson2x::initFromJsonFile(jsonFileName))
		return false;

	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->ignoreAnchorPointForPosition(false);

	// 爱心数量
	Label* pLabelInJson = this->getSubLabelByKey("num");
	pLabelInJson->setVisible(false);

	std::string starStr = WJUtils::stringAddInt("", getTotalStar());
	m_label = Label::createWithBMFont(STAR_FONT_FILE, starStr, TextHAlignment::CENTER);
	m_label->ignoreAnchorPointForPosition(false);
	m_label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_label->setPosition(pLabelInJson->getPosition() + Vec2(0.0f, 10.0f));
	this->addChild(m_label, 100);

	return true;
}

void Star::onEnter()
{
	WJLayerJson2x::onEnter();

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(Star::starChangedNotification),
		"heartChanged", NULL);
}

void Star::onExit()
{
	WJLayerJson2x::onExit();
	NotificationCenter::getInstance()->removeObserver(this, "heartChanged");
}

void Star::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}

void Star::starChangedNotification(Ref *object)
{
	m_label->setString(WJUtils::stringAddInt("", getTotalStar()).c_str());
}

void Star::addStar( int n )
{
	UserDefault::getInstance()->setIntegerForKey("star", getTotalStar() + n);
	UserDefault::getInstance()->flush();

	NotificationCenter::getInstance()->postNotification("heartChanged");
}

void Star::subStar( int n )
{
	int g = getTotalStar() - n;
	if (g < 0)
		g = 0;
	UserDefault::getInstance()->setIntegerForKey("star", g);
	UserDefault::getInstance()->flush();

	NotificationCenter::getInstance()->postNotification("heartChanged");
}

int Star::getTotalStar()
{
	return UserDefault::getInstance()->getIntegerForKey("star", 0);
}

void Star::playParticle()
{
	const Size &size = this->getContentSize();
	ParticleSystemQuad *particle = ParticleSystemQuad::create("particles/heartui.plist");
	particle->setPosition(Vec2(size.width / 2, size.height / 2)+ Vec2(0, 0));
	particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	this->addChild(particle, 1000);
}
