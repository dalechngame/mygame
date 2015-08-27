#include "LayerSetup.h"
#include "Scenestart.h"
#include "GameInfo.h"
#include "Mario.h"
#include "LayerGame.h"

bool LayerSetup::init()
{
	CCLayer::init();
	CCSprite *bg = CCSprite::create("Set_Music.png");
	bg->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	addChild(bg);
	bg->setScale(0.75f);

	setTouchMode(kCCTouchesOneByOne);
	setTouchEnabled(true);
	setTouchPriority(kCCMenuHandlerPriority << 1);

	CCMenu *menu = CCMenu::create();
	CCSprite *sp1 = CCSprite::create("music_on.png");
	CCSprite *sp2 = CCSprite::create("music_off.png");
	CCSprite *sp3 = CCSprite::create("sound_effect_on.png");
	CCSprite *sp4 = CCSprite::create("sound_effect_off.png");
	sp1->setScale(0.9f);
	sp2->setScale(0.9f);
	sp3->setScale(0.9f);
	sp4->setScale(0.9f);
	CCMenuItemSprite *_bgON = CCMenuItemSprite::create(sp1, sp1);
	CCMenuItemSprite *_bgOFF = CCMenuItemSprite::create(sp2, sp2);
	CCMenuItemSprite *_effectON = CCMenuItemSprite::create(sp3, sp3);
	CCMenuItemSprite *_effectOFF = CCMenuItemSprite::create(sp4, sp4);
	bool isBg = CCUserDefault::sharedUserDefault()->getBoolForKey("BGStatus", true);
	bool isEff = CCUserDefault::sharedUserDefault()->getBoolForKey("EffectStatus", true);
	isBg ? _bg = CCMenuItemToggle::createWithTarget(this, menu_selector(LayerSetup::setBG), _bgON, _bgOFF, NULL) :
		_bg = CCMenuItemToggle::createWithTarget(this, menu_selector(LayerSetup::setBG), _bgOFF, _bgON, NULL);
	isEff ? _effect = CCMenuItemToggle::createWithTarget(this, menu_selector(LayerSetup::setEffect), _effectON, _effectOFF, NULL) :
		_effect = CCMenuItemToggle::createWithTarget(this, menu_selector(LayerSetup::setEffect), _effectOFF, _effectON, NULL);

	CCScale9Sprite *bgButton = CCScale9Sprite::create("button.png");
	CCScale9Sprite *bgButtonLighted = CCScale9Sprite::create("buttonHighlighted.png");
	CCScale9Sprite *bgButton2 = CCScale9Sprite::create("button.png");
	CCScale9Sprite *bgButtonLighted2 = CCScale9Sprite::create("buttonHighlighted.png");
	CCLabelTTF * text1 = CCLabelTTF::create("back", "verdana", 20);
	CCLabelTTF * text2 = CCLabelTTF::create("home", "verdana", 20);
	_back = CCControlButton::create(text1, bgButton);
	_home = CCControlButton::create(text2, bgButton2);

	_back->setBackgroundSpriteForState(bgButtonLighted, CCControlStateHighlighted);
	_back->addTargetWithActionForControlEvents(this,
		cccontrol_selector(LayerSetup::backGame),
		CCControlEventTouchDown);

	_home->setBackgroundSpriteForState(bgButtonLighted2, CCControlStateHighlighted);
	_home->addTargetWithActionForControlEvents(this,
		cccontrol_selector(LayerSetup::backHome),
		CCControlEventTouchDown);

	_bg->setPosition(ccp(60, -20)); 
	_effect->setPosition(ccp(60, 40));
	_home->setPosition(ccp(winSize.width / 2 - 70, winSize.height / 2 - 70));
	_back->setPosition(ccp(winSize.width / 2 + 70, winSize.height / 2 - 70));

	menu->setTouchPriority(kCCMenuHandlerPriority << 2);
	_home->setTouchPriority(kCCMenuHandlerPriority << 2);
	_back->setTouchPriority(kCCMenuHandlerPriority << 2);



	menu->addChild(_bg);
	menu->addChild(_effect);
	addChild(menu);
	addChild(_home);
	addChild(_back);


	return true;
}

void LayerSetup::setBG(CCObject*obj)
{
	bool isBg = CCUserDefault::sharedUserDefault()->getBoolForKey("BGStatus", true);
	CCUserDefault::sharedUserDefault()->setBoolForKey("BGStatus", !isBg);
	isBg ? CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic() :
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("OnLand.wav");
}

void LayerSetup::setEffect(CCObject*obj)
{
	bool isEff = CCUserDefault::sharedUserDefault()->getBoolForKey("EffectStatus", true);
	CCUserDefault::sharedUserDefault()->setBoolForKey("EffectStatus", !isEff);
	isEff ? CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects() :
		CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}

void LayerSetup::backGame(CCObject*obj, CCControlEvent)
{
	CCCallFunc* pauseAction = CCCallFunc::create(this, callfunc_selector(LayerSetup::resumeGame));
	CCMoveBy* move = CCMoveBy::create(0.5f, ccp(winSize.width, 0));
	CCActionEase *rate = CCEaseExponentialOut::create(move);
	runAction(CCSequence::createWithTwoActions(rate, pauseAction));

}

void LayerSetup::backHome(CCObject*obj, CCControlEvent)
{

	CCCallFunc* pauseAction = CCCallFunc::create(this, callfunc_selector(LayerSetup::restartGame));
	CCMoveBy* move = CCMoveBy::create(0.5f, ccp(winSize.width, 0));
	CCActionEase *rate = CCEaseExponentialOut::create(move);
	runAction(CCSequence::createWithTwoActions(rate, pauseAction));
}

bool LayerSetup::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void LayerSetup::resumeGame()
{
	removeFromParent();
	CCArray *child = game->_map->getChildren();
	CCObject *obj;
	CCARRAY_FOREACH(child, obj)
	{
		Item * item = (Item *)obj;
		item->resumeSchedulerAndActions();
	}
	game->resumeSchedulerAndActions();
}

void LayerSetup::restartGame()
{
	
	this->removeFromParent();
	GameInfo::Utime = 300;
	GameInfo::UScore = 0;
	GameInfo::UCoin = 0;
	GameInfo::ULevel = 1;
	Mario::_life = 3;
	CCDirector *dic = CCDirector::sharedDirector();
	dic->replaceScene(CCTransitionCrossFade::create(0.5f, Common::scene(SceneStart::create())));

	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}
