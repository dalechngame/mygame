#include "LayerGameOver.h"  			
#include "AppMacros.h"		
#include "LayerGameMain.h"
#include "Plane.h"

CCScene * LayerGameOver::scene()
{
	CCScene * scene = CCScene::create();
	LayerGameOver * layer = LayerGameOver::create();
	scene->addChild(layer);
	return scene;
}

bool LayerGameOver::init()
{
	CCLayer::init();

	CCSprite * background = CCSprite::createWithSpriteFrameName("gameover.png");
	background->setAnchorPoint(ccp(0, 0));
	background->setPosition(ccp(0, 0));


	const char *scoreStr = CCString::createWithFormat("%d", LayerGameMain::score)->getCString();

	CCLabelBMFont *finalScore = CCLabelBMFont::create(scoreStr, "font/font.fnt");
	finalScore->setColor(ccc3(143, 146, 147));
	//finalScore->setAnchorPoint(ccp(0.5, 1));
	finalScore->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 50));

	CCSprite * nor = CCSprite::createWithSpriteFrameName("btn_finish.png");
	CCSprite * press = CCSprite::createWithSpriteFrameName("btn_finish.png");

	//! 重新开始按钮
	CCMenuItemSprite *overItem = CCMenuItemSprite::create(nor, press, this, menu_selector(LayerGameOver::restartGameMCallBack));
	//! 放在中间偏下
	overItem->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 50));
	CCMenu *pauseMenu = CCMenu::create(overItem, NULL);
	pauseMenu->setPosition(CCPointZero);


	addChild(background);
	addChild(finalScore);
	addChild(pauseMenu);

	return true;
}


void LayerGameOver::restartGameMCallBack(CCObject* obj)
{
	LayerGameMain::score = 0;
	Plane::_splane = NULL;
	CCScene *pscene = LayerGameMain::scene();
	CCDirector::sharedDirector()->replaceScene(pscene);
}

