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
	// ��������
	_bg = CCSprite::create(bgFile);
	// ������������
	_bgBar = CCSprite::create(bgBarFile);
	// ����ǰ������
	_bar = CCProgressTimer::create(CCSprite::create(foreFile));

	// �������εģ��������˶��Ľ�����
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