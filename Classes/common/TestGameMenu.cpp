// Created by wangwenjie on 2015/01

#include "TestGameMenu.h"
#include "Common.h"
#include "LBModalDialogLayer.h"
#include "Loading.h"

USING_NS_CC;

bool TestGameMenu::init()
{
	if (!Layer::init())
		return false;

	const Size& winSize = Director::getInstance()->getWinSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// menu
	int count = (int)GameNumber::P020;

	Vector<MenuItem*> menuItems;
	const int c = 4;
	const float w = 250, h = 60;
	const float x = (winSize.width - c * w) / 2 + w / 2;
	const float y = winSize.height - (winSize.height - ceil(count / (float)c) * h) / 2;

	// title
	std::string title = "Test Menu";
	//title.append("  ").append(__DATE__).append("  ").append(__TIME__);
    Label *labelTitle = Label::createWithSystemFont(title, MenuItemFont::getFontName(), 28);
	labelTitle->setPosition(winSize.width / 2, y + 100);
	this->addChild(labelTitle);

	// menu items
	for (int i = (int)GameNumber::P001, n = 0; i <= count; i++, n++)
	{
		MenuItemFont* item = MenuItemFont::create(Common::getGameName((GameNumber)i).c_str(),
												  CC_CALLBACK_1(TestGameMenu::onMenuItemClick, this, (GameNumber)i));
		item->setPosition(x + (n % c) * w, y - (n / c) * h);
		menuItems.pushBack(item);
	}

	// back
	MenuItemFont *itemBack = MenuItemFont::create("Back",
												  CC_CALLBACK_1(TestGameMenu::onMenuItemBackClick, this));
	itemBack->setPosition(winSize.width / 2, menuItems.at(menuItems.size() - 1)->getPositionY() - 100);
	menuItems.pushBack(itemBack);

	Menu *menu = Menu::createWithArray(menuItems);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void TestGameMenu::onMenuItemClick( Ref* node, GameNumber gameNumber )
{
	PLoader *loader = PLoader::getLoader(gameNumber);
	if (!loader)
		return;

	Loading::gotoScene(gameNumber);
}

void TestGameMenu::onMenuItemBackClick( Ref* node )
{
	static_cast<LBModalDialog*>(this->getParent())->close();
}

void TestGameMenu::show()
{
	TestGameMenu *layer = new TestGameMenu();
	layer->autorelease();
	layer->init();

	LBModalDialog *dialog = LBModalDialog::create(layer, true);

	Scene *scene = Director::getInstance()->getRunningScene();
	scene->addChild(dialog);

	dialog->show();
}
