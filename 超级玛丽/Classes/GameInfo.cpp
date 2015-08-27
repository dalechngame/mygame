#include "GameInfo.h"

int GameInfo::Utime = 300;
int GameInfo::UScore = 0;
int GameInfo::UCoin = 0;
int GameInfo::ULevel = 1;
CCLabelBMFont *GameInfo::monsterScore;
CCLabelBMFont * GameInfo::score;
CCLabelBMFont * GameInfo::coinNum;
CCLabelBMFont * GameInfo::level;
CCLabelBMFont * GameInfo::time;

bool GameInfo::init()
{
	CCLayer::init();

	//! coin 闪烁动画
	CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->animationByName("CoinBlink");
	CCAnimate* animate = CCAnimate::create(animation);

	scoreName = CCLabelTTF::create("Mario", "Courier", 15);
	levelName = CCLabelTTF::create("World", "Courier", 15);
	timeName = CCLabelTTF::create("Time", "Courier", 15);

	markX = CCLabelBMFont::create("X", "fonts/font.fnt");
	score = CCLabelBMFont::create(CCString::createWithFormat("%d", UScore)->getCString(), "fonts/font.fnt");
	coinNum = CCLabelBMFont::create(CCString::createWithFormat("%d", UCoin)->getCString(), "fonts/font.fnt");
	level = CCLabelBMFont::create(CCString::createWithFormat("%d", ULevel)->getCString(), "fonts/font.fnt");
	time = CCLabelBMFont::create(CCString::createWithFormat("%d", Utime)->getCString(), "fonts/font.fnt");

	
	
	blinkCoin = CCSprite::create();
	monsterScore = CCLabelBMFont::create("100", "fonts/font.fnt");

	monsterScore->setPosition(winSize.width / 2, winSize.height / 2);
	monsterScore->setScale(0.2f);
	monsterScore->setVisible(false);
	monsterScore->setZOrder(200);

	blinkCoin->runAction(CCRepeatForever::create(animate));

	score->setScale(0.3f);
	coinNum->setScale(0.3f);
	level->setScale(0.3f);
	time->setScale(0.3f);
	markX->setScale(0.2f);

	int width = winSize.width / 5;
	int height = winSize.height - 40;
	// 设置四个按钮的位置
	Common::moveNode(scoreName, ccp(width, height + 15));
	Common::moveNode(levelName, ccp(width * 3, height + 15));
	Common::moveNode(timeName, ccp(width * 4, height + 15));

	Common::moveNode(score, ccp(width, height));
	Common::moveNode(markX, ccp(width * 2, height - 2));
	Common::moveNode(blinkCoin, ccp(width * 2 - 10, height));
	Common::moveNode(coinNum, ccp(width * 2 + 12, height));
	Common::moveNode(level, ccp(width * 3, height));
	Common::moveNode(time, ccp(width * 4, height));

	addChild(score);
	addChild(coinNum);
	addChild(level);
	addChild(time);
	addChild(scoreName);
	addChild(levelName);
	addChild(timeName);
	addChild(blinkCoin);
	addChild(markX);
	addChild(monsterScore);

	//schedule(schedule_selector(GameInfo::scheduleTime), 1.0f);

	return true;
}

void GameInfo::scheduleTime(float dt)
{
	Utime--;
	setTime(CCString::createWithFormat("%d", Utime)->getCString());
}

void GameInfo::disaCall()
{
	monsterScore->setVisible(false);
}