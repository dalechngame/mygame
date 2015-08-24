#include "LayerLoadRes.h"

LayerLoadRes* LayerLoadRes::create(const char* bgFile, const char *bgBarFile,  const char* foreFile)
{
	LayerLoadRes* ret = new LayerLoadRes;
	ret->init(bgFile, bgBarFile, foreFile);
	ret->autorelease();
	return ret;
}

bool LayerLoadRes::init(const char* bgFile, const char *bgBarFile, const char* foreFile)
{
	CCLayer::init();
	// 创建背景
	_bg = CCSprite::create(bgFile);
	// 创建背景精灵
	_bgBar = CCSprite::create(bgBarFile);
	// 创建前景精灵
	_bar = CCProgressTimer::create(CCSprite::create(foreFile));

	// 设置条形的，从左到右运动的进度条
	_bar->setType(kCCProgressTimerTypeBar);
	_bar->setMidpoint(ccp(0, 0));
	_bar->setBarChangeRate(ccp(1, 0));
	_bar->setPercentage(0);
	_bg->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	
	addChild(_bg, BG);
	addChild(_bgBar, BGBAR);
	addChild(_bar, BAR);

	return true;
}