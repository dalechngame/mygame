#include "LayerOver.h"
#include "LayerStart.h"


CCScene *LayerOver::scene(bool res)
{
	CCScene *scene = CCScene::create();
	LayerOver *layer = LayerOver::create(res);
	scene->addChild(layer);

	return scene;
}

LayerOver *LayerOver::create(bool res)
{
	LayerOver *pRet = new LayerOver();
	if (pRet && pRet->init(res))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool LayerOver::init(bool res)
{
	CCLayer::init();

	CCMenu *menu = CCMenu::create();
	CCMenuItem *back = CCMenuItemFont::create("Back", this, menu_selector(LayerOver::menuCallBack));

	if (res == true)
	{
		CCMenuItem *winItem = CCMenuItemFont::create("You Win");
		menu->addChild(winItem);
		menu->addChild(back);
	}
	else
	{
		CCMenuItem *failItem = CCMenuItemFont::create("You Lose");
		menu->addChild(failItem);
		menu->addChild(back);
	}

	addChild(menu);
	menu->alignItemsVerticallyWithPadding(30);

	return true;
}

void LayerOver::menuCallBack(CCObject *obj)
{
	CCScene  *scene = LayerStart::scene();
	CCDirector::sharedDirector()->replaceScene(scene);
}