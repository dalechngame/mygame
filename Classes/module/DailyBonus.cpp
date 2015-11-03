// Created by wangwenjie on 2014/01

#include "DailyBonus.h"
#include "Common.h"
#include "LBModalDialogLayer.h"
#include "star.h"
#include "GameManager.h"

USING_NS_CC;

DailyBonus * DailyBonus::create( int days )
{
	DailyBonus *layer = new DailyBonus();
	if (layer && layer->init(days))
	{
		layer->autorelease();
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return NULL;
}

bool DailyBonus::init(int days)
{
	if (!TipDialog::init("game/json/dailybonus.json"))
		return false;

    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->ignoreAnchorPointForPosition(false);
    Size winSize = Director::getInstance()->getWinSize();
    this->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

    m_days = days;

	setNodeColorAndUsedAble();

    return true;
}

bool DailyBonus::showItIfNeed(Node *owner, int zOrder)
{
    int days = checkDays();
    if (days <= 0)
        return false;

    DailyBonus *dailyBonus = DailyBonus::create(days);
	owner->addChild(dailyBonus, zOrder);
	dailyBonus->show(LBModalDialogAnimationType::ScaleEaseBack);
    
    return true;
}

int DailyBonus::checkDays()
{
    std::string strLastDay = UserDefault::getInstance()->getStringForKey("lastDay", "");
    std::string strToday = WJUtils::getSystemYMD();
    int days = UserDefault::getInstance()->getIntegerForKey("days", 1);

	// 只能送7次
	if (days >= 7)
		return 0;

    do
    {
        if (strLastDay.length() == 0)
        {
            days = 1;  // default 1
            break;
        }

        long lastDay = atol(strLastDay.c_str());
        long today = atol(strToday.c_str());
        if (today <= lastDay)
        {
            // error or today
            return 0;
        }

		// 间隔一天或者间隔几天
        if (today - lastDay >= 1)
        {
            days++;
            break;
        }

    } while (false);

    // write to file
	UserDefault::getInstance()->setIntegerForKey("days", days);
	UserDefault::getInstance()->setStringForKey("lastDay", strToday);
	UserDefault::getInstance()->flush();

    return days;
}

int DailyBonus::getDays()
{
    return UserDefault::getInstance()->getIntegerForKey("days");
}

void DailyBonus::onEnterTransitionDidFinish()
{
    TipDialog::onEnterTransitionDidFinish();

	this->scheduleOnce(CC_SCHEDULE_SELECTOR(DailyBonus::onPlayGiftAniTimer), 6.0f);
}

void DailyBonus::setNodeColorAndUsedAble()
{
	if (m_days <= 0 || m_days > 7)
		return;

	WJSprite *spriteDay;
	WJSprite *spriteGift;
	for (int i = 1; i <= 7; i++)
	{
		spriteDay = m_layerJson->getSubSprite(WJUtils::stringAddInt("day", i, 3).c_str());
		spriteGift = m_layerJson->getSubSprite(WJUtils::stringAddInt("day", i, 3).append("_gift").c_str());

		if (m_days == i)
		{
			spriteDay->setScale(spriteDay->getScale() * 1.5f);
			spriteGift->setScale(spriteGift->getScale() * 1.5f);
			spriteDay->setOnClick(CC_CALLBACK_2(DailyBonus::onClickDaySprite, this));
		}
		else
		{
			spriteDay->setGrayEnabled(true);
			spriteGift->setGrayEnabled(true);
		}
	}
}

void DailyBonus::onClickDaySprite(Node *node, WJTouchEvent *event)
{
	if (m_completed)
		return;

	m_completed = true;
	WJSprite *sprite = (WJSprite*)node;
	sprite->setEnabled(false);
	// 关闭弹窗
	onClickClose(m_layerJson->getSubSprite("no"), event);
	// 礼物
	Scene *scene = Director::getInstance()->getRunningScene();
	const Size &size = Director::getInstance()->getWinSize();
	WJSprite *spriteGift = m_layerJson->getSubSprite(WJUtils::stringAddInt("day", m_days, 3).append("_gift").c_str());
	Vec2 &point = spriteGift->getPositionWorld();
	spriteGift->retain();
	spriteGift->removeFromParentAndCleanup(true);
	spriteGift->setPosition(scene->convertToNodeSpace(point));
	scene->addChild(spriteGift, 1000);
	spriteGift->release();

	// ？？ 礼物飞到公主的房间
	spriteGift->runAction(Sequence::create(
		MoveTo::create(0.5f, Vec2(size.width / 2, size.height / 2)),
		DelayTime::create(3.0f),
		Spawn::create(
			MoveTo::create(1.0f, Vec2(800, 600)),
			ScaleTo::create(1.0f, 0.1f),
			RotateBy::create(1.0f, 720),
			FadeOut::create(1.0f),
		nullptr),
		RemoveSelf::create(),
		nullptr));
}

void DailyBonus::onPlayGiftAniTimer(float time)
{
	if (m_completed)
		return;
	
	onClickDaySprite(m_layerJson->getSubSprite(WJUtils::stringAddInt("day", m_days, 3).c_str()), nullptr);
}

DailyBonus::DailyBonus()
{
	m_completed = false;
}
