#include "SceneStart.h"
#include "sceneMenu.h"

bool SceneStart::init()
{
	CCLayer::init();

	CCSprite* bg = CCSprite::create("background.png");
	addChild(bg);
	bg->setPosition(ccp(winSize.width / 2, winSize.height / 2));

	CCMenu* menu = CCMenu::create();
	addChild(menu);

	CCMenuItemImage* start = CCMenuItemImage::create("startgame_normal.png", "startgame_select.png", this, menu_selector(SceneStart::Start));
	menu->addChild(start);

	CCMenuItemImage* setup = CCMenuItemImage::create("Setting_n.png", "setting_s.png", this, menu_selector(SceneStart::Setup));
	menu->addChild(setup);

	CCMenuItemImage* quit = CCMenuItemImage::create("quitgame_normal.png", "quitgame_select.png", this, menu_selector(SceneStart::Quit));
	menu->addChild(quit);

	CCMenuItemImage* about = CCMenuItemImage::create("about_normal.png", "about_select.png", this, menu_selector(SceneStart::About));
	menu->addChild(about);

	// 设置四个按钮的位置
	Common::moveNode(setup, ccp(0, -40));
	Common::moveNode(quit, ccp(0, -80));
	Common::moveNode(about, ccp(150, -120));



	return true;
}

void SceneStart::Start(CCObject*)
{
	CCDirector::sharedDirector()->replaceScene(Common::scene(SceneMenu::create()));
}

void SceneStart::Setup(CCObject*)
{}
void SceneStart::Quit(CCObject*)
{}
void SceneStart::About(CCObject*)
{}