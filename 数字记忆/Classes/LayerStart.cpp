#include "LayerStart.h"

#include "LayerGame.h"



CCScene *LayerStart::scene()
{
	CCScene * scene = CCScene::create();
	LayerStart * layer = LayerStart::create();
	scene->addChild(layer);
	return scene;
}
bool LayerStart::init()
{
	CCLayer::init();

		//采用工厂模式统一分发触摸事件,定时器事�?运动事件
	CCMenuItem * easyItem = CCMenuItemFont::create("Easy", this,
		menu_selector(LayerStart::easy));
	CCMenuItem * hardItem = CCMenuItemFont::create("Hard", this, 
		menu_selector(LayerStart::hard));
	CCMenuItem * veryHardItem = CCMenuItemFont::create("VeryHard", 
		this, menu_selector(LayerStart::veryHard));
	CCMenuItem * quitItem = CCMenuItemFont::create("Quit", this,
		menu_selector(LayerStart::quit));

	CCMenu * menu = CCMenu::create(easyItem, hardItem, 
		veryHardItem, quitItem,NULL);

	menu->alignItemsVerticallyWithPadding(30);
	addChild(menu);
	return true;
}

void LayerStart::easy(CCObject* obj)
{
	CCScene * scene = LayerGame::scene(EASY);
	CCDirector::sharedDirector()->replaceScene(scene);
}
void LayerStart::hard(CCObject* obj)
{
	CCScene * scene = LayerGame::scene(HARD);
	CCDirector::sharedDirector()->replaceScene(scene);
}
void LayerStart::veryHard(CCObject* obj)
{
	CCScene * scene = LayerGame::scene(VERYHARD);
	CCDirector::sharedDirector()->replaceScene(scene);
}
void LayerStart::quit(CCObject* obj)
{
	exit(1);
}