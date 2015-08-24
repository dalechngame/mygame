#include "LayerGame.h"
#include "LayerStart.h"
#include "LayerMenu.h"
#include "LayerSetup.h"
#include "MenuCtrl.h"
#include "Item.h"
#include "GameInfo.h"
#include "ResPath.h"
#include "Bullet.h"
#include "LayerLoadRes.h"
#include "Mario.h"

int LayerGame::_idx;
CCLabelBMFont *LayerGame::monsterScore;

extern const char* picfiles[];
extern const char* effectMusicfile[];
extern const char* bgMusicFile[];

LayerGame* LayerGame::create(int idx)
{
	LayerGame*ret = new LayerGame;
	ret->init(idx);
	ret->autorelease();
	return ret;
}

bool LayerGame::init(int idx)
{
	CCLayer::init();

	_idx = idx;
	audioEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();

	// 加载资源
	loadResource();

	return true;
}

void LayerGame::addGameInfo()
{
	GameInfo * gameInfo = GameInfo::create();

	coin = CCSprite::create();
	monsterScore = CCLabelBMFont::create("100", "fonts/font.fnt");

	coin->setPosition(ccp(winSize.width / 3, winSize.height / 3));
	monsterScore->setPosition(winSize.width / 2, winSize.height / 2);
	monsterScore->setScale(0.2f);
	monsterScore->setVisible(false);
	monsterScore->setZOrder(200);

	_map->addChild(coin);
	_map->addChild(monsterScore);
	addChild(gameInfo);
}

void LayerGame::loadResource()
{
	_bar = LayerLoadRes::create("bg.png", "sliderTrack.png", "sliderProgress.png");
	_bar->setPosition(winSize.width / 2, winSize.height / 2);
	addChild(_bar);

	// 加载声音文件
	int musicfilecount = sizeof(effectMusicfile) / sizeof(*effectMusicfile);
	for (int i = 0; i < musicfilecount; ++i)
	{
		audioEngine->preloadEffect(effectMusicfile[i]);
		audioEngine->preloadBackgroundMusic(bgMusicFile[i]);
	}

	// 加载图片，只将图片加载到纹理缓存
	_loadFileCount = 0;
	_totalfile = sizeof(picfiles) / sizeof(*picfiles);
	for (int i = 0; i < _totalfile; ++i)
	{
		CCTextureCache::sharedTextureCache()->addImageAsync(picfiles[i], this, callfuncO_selector(LayerGame::loadImageFuncCall));
	}

}

void LayerGame::loadImageFuncCall(CCObject*)
{
	_loadFileCount++;
	_bar->setPercentage((_loadFileCount)*100.0f / _totalfile);

	if (_loadFileCount == _totalfile)
	{
		_bar->removeFromParent();
		startGame();
	}
}

void LayerGame::moveLeftCallBack(CCObject*)
{
	_menuShow->setTexture(this->_textureDirLeft);
	_marioDir = Common::LEFT;
}

void LayerGame::moveRightCallBack(CCObject*)
{
	_menuShow->setTexture(this->_textureDirRight);
	_marioDir = Common::RIGHT;
}

void LayerGame::moveMarioUpd(float dt)
{
	if (_mario->_bflagRunAction || _mario->_bAutoRun)
	{
		return;
	}
#ifdef WIN32
	short key;

	key = GetKeyState('F');
	if (key < 0) _marioDir = Common::RIGHT;

	key = GetKeyState('D');
	if (key < 0) _marioDir = Common::LEFT;

	key = GetKeyState('J');
	if (key < 0)_mario->jump();

	key = GetKeyState('K');
	if (key < 0)_mario->fire();

#endif

	if (_marioDir == Common::LEFT)
	{
		//! 用来改变状态
		_marioDir = Common::NONE;
		_mario->moveLeft(dt);
	}
	else if (_marioDir == Common::RIGHT)
	{
		_marioDir = Common::NONE;
		_mario->moveRight(dt);
	}
	else
	{
		_menuShow->setTexture(this->_textureDirNone);
		_mario->stop();
	}

	_mario->moveUp(dt);
	_mario->moveDown(dt);
}

void LayerGame::checkMarioTouchPole(float)
{
	CCRect rcMario = _mario->boundingBox();
	CCPoint pt = ccp(rcMario.getMaxX() - 8, rcMario.getMaxY());
	CCPoint ptInMap = Common::Point2Tile(_map, pt);
	CCTMXLayer* layer = _map->layerNamed("flagpole");

	if (ptInMap.y >= _map->getMapSize().height) return;
	if (ptInMap.y < 0) return;

	// 碰到旗杆了
	if (layer->tileGIDAt(ptInMap) != 0)
	{
		_mario->autoRunDown();
		unschedule(schedule_selector(LayerGame::checkMarioTouchPole));
	}
}

void LayerGame::checkMarioDie(float)
{
	// 检查Mario的脑袋是不是出了地图下外
	CCRect rc = _mario->boundingBox();
	if (rc.getMaxY() < 0)
	{
		_mario->die(true);
	}
}

void LayerGame::startGame()
{
	bool isBg = CCUserDefault::sharedUserDefault()->getBoolForKey("BGStatus", true);
	bool isEff = CCUserDefault::sharedUserDefault()->getBoolForKey("EffectStatus", true);
	isEff ? audioEngine->resumeAllEffects() :
		audioEngine->pauseAllEffects();
	isBg ?// 循环播放
		audioEngine->playBackgroundMusic("OnLand.wav", true) :
	audioEngine->stopBackgroundMusic();
	

	// 创建ItemReward
	Item::_bridge = NULL;
	Item::_itemReward = CCArray::create();
	Item::_pbulletArray = CCArray::create();
	Item::_itemReward->retain();
	Item::_pbulletArray->retain();

	addMap();
	addRes();
	addGameInfo();
	addCtrl();
	addstopMenu();
	addLeaderAndMon();
	openSchedule();

}

void LayerGame::GameOver()
{
	Mario::_life -= 1;
	if (Mario::_life == 0)
	{
		GameInfo::Utime = 300;
		GameInfo::UScore = 0;
		GameInfo::UCoin = 0;
		GameInfo::ULevel = 1;
		Mario::_life = 3;

		CCDirector::sharedDirector()->replaceScene(Common::scene(LayerStart::create()));
	}
	else
	{

		CCDirector::sharedDirector()->replaceScene(Common::scene(LayerGame::create(_idx)));
	}
}

void LayerGame::addRes()
{

	// 加载蘑菇怪资源
	{
		CCAnimation* animation = Common::CreateAnimation("Mushroom0.png", 0, 1, 16, 0.4f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "MushroomMoving");
		CCSpriteFrame* dead1 = Common::getSpriteFrame("Mushroom0.png", 2, 16);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(dead1, "MushroomDead1");
		CCSpriteFrame* dead2 = Common::getSpriteFrame("Mushroom0.png", 3, 16);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(dead2, "MushroomDead2");
	}

	// 加载乌龟动画资源
	{
	CCAnimation* animation1 = Common::CreateAnimation("tortoise0.png", 2, 3, 18, 0.4f);
	CCAnimationCache::sharedAnimationCache()->addAnimation(animation1, "TortoiseMoveLeft");
	CCAnimation* animation2 = Common::CreateAnimation("tortoise0.png", 4, 5, 18, 0.4f);
	CCAnimationCache::sharedAnimationCache()->addAnimation(animation2, "TortoiseMoveRight");
	CCAnimation* animation3 = Common::CreateAnimation("tortoise0.png", 8, 9, 18, 0.4f);
	CCAnimationCache::sharedAnimationCache()->addAnimation(animation3, "TortoiseDead");

	CCAnimation* animation4 = Common::CreateAnimation("tortoise0.png", 0, 1, 18, 0.4f);
	CCAnimationCache::sharedAnimationCache()->addAnimation(animation4, "TortoiseFlyLeft");
	CCAnimation* animation5 = Common::CreateAnimation("tortoise0.png", 6, 7, 18, 0.4f);
	CCAnimationCache::sharedAnimationCache()->addAnimation(animation5, "TortoiseFlyRight");
}
	// 小mario
	{
		CCAnimationCache::sharedAnimationCache()->addAnimation(
			Common::CreateAnimation("smallWalkLeft.png", 0, 9, 14, 0.05f), "smallleftwalk");
		CCAnimationCache::sharedAnimationCache()->addAnimation(
			Common::CreateAnimation("smallWalkRight.png", 0, 9, 14, 0.05f), "smallrightwalk");
		CCAnimationCache::sharedAnimationCache()->addAnimation(
			Common::CreateAnimation("small_die.png", 0, 6, 16, 0.05f), "smalldie");

		CCSpriteFrame *frame = Common::getSpriteFrame("smallWalkLeft.png", 10, 14);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(frame, "smallleftjump");
		frame = Common::getSpriteFrame("smallWalkRight.png", 10, 14);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(frame, "smallrightjump");
	}
	// 大mario
	{
		CCAnimationCache::sharedAnimationCache()->addAnimation(Common::CreateAnimation("walkLeft.png", 0, 9, 18, 0.05f), "bigleftwalk");
		CCAnimationCache::sharedAnimationCache()->addAnimation(Common::CreateAnimation("walkRight.png", 0, 9, 18, 0.05f), "bigrightwalk");
		CCAnimationCache::sharedAnimationCache()->addAnimation(Common::CreateAnimation("normal_die.png", 0, 6, 16, 0.05f), "bigdie");

		CCSpriteFrame *frame = Common::getSpriteFrame("walkLeft.png", 10, 18);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(frame, "bigleftjump");
		frame = Common::getSpriteFrame("walkRight.png", 10, 18);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(frame, "bigrightjump");
	}
	// fire mario
	{
		CCAnimationCache::sharedAnimationCache()->addAnimation(Common::CreateAnimation("WalkLeft_fire.png", 0, 9, 18, 0.05f), "fireleftwalk");
		CCAnimationCache::sharedAnimationCache()->addAnimation(Common::CreateAnimation("WalkRight_fire.png", 0, 9, 18, 0.05f), "firerightwalk");
		CCAnimationCache::sharedAnimationCache()->addAnimation(Common::CreateAnimation("fire_die.png", 0, 6, 16, 0.05f), "firedie");
		CCAnimationCache::sharedAnimationCache()->addAnimation(Common::CreateAnimation("fireActionL.png", 0, 5, 25, 0.05f), "FireActionL");
		CCAnimationCache::sharedAnimationCache()->addAnimation(Common::CreateAnimation("fireActionR.png", 0, 5, 25, 0.05f, true), "FireActionR");

		CCSpriteFrame *frame = Common::getSpriteFrame("WalkLeft_fire.png", 10, 18);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(frame, "fireleftjump");
		frame = Common::getSpriteFrame("WalkRight_fire.png", 10, 18);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(frame, "firerightjump");
	}
	// Flower资源
	{
		CCAnimation* animation = Common::CreateAnimation("flower0.png", 0, 1, 16, 0.4f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "FlowerShow");
	}

	// Reward
	{
		CCSpriteFrame* frame = Common::getSpriteFrame("rewardMushroomSet.png", 0, 16);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(frame, "MushroomReward");
	}
	{
		CCSpriteFrame* frame = Common::getSpriteFrame("rewardMushroomSet.png", 1, 16);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(frame, "MushroomAddLife");
	}
	{
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("Tools.png");
		CCArray* arr = CCArray::create();
		for (int i = 0; i <= 3; ++i)
		{
			CCSpriteFrame* frame = CCSpriteFrame::createWithTexture(texture,
				CCRectMake(i * 17, 0, 17, texture->getContentSize().height / 3));
			arr->addObject(frame);
		}
		CCAnimationCache::sharedAnimationCache()->addAnimation(CCAnimation::createWithSpriteFrames(arr, 0.1f), "RewardFlower");
	}

	// flyfish动画
	{
		CCAnimation* animation1 = Common::CreateAnimation("flyFishLeft.png", 0, 5, 16, 0.05f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation1, "FlyFishLeft");
		CCAnimation* animation2 = Common::CreateAnimation("flyFishRight.png", 0, 5, 16, 0.05f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation2, "FlyFishRight");
	}

	// boss动画
	{
		CCAnimation* animation1 = Common::CreateAnimation("boss.png", 0, 3, 32, 0.05f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation1, "BossWalkLeft");
		CCAnimation* animation2 = Common::CreateAnimation("boss.png", 4, 7, 32, 0.05f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation2, "BossWalkRight");
	}

	// coin 动画
	{
		CCAnimation* animation1 = Common::CreateAnimation("coinani.png", 0, 3, 8, 0.1f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation1, "CoinBlink");
	}
	// 子弹动画
	{
		CCAnimation* animation1 = Common::CreateAnimation("fireRight.png", 10, 10, 11, 0.1f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation1, "FireBomb");
		CCAnimation* animation2 = Common::CreateAnimation("fireRight.png", 0, 8, 10, 0.1f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation2, "FireRight");
	}

}

void LayerGame::addMap()
{
	_map = CCTMXTiledMap::create(Common::format(_idx + 1, "MarioMap", ".tmx"));
	addChild(_map);
	Common::moveNode(_map, ccp(0, winSize.height - _map->getContentSize().height));
}

void LayerGame::addCtrl()
{
	//! 控制界面背景
	CCSprite* sprite = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("controlUI.png"));
	addChild(sprite);
	sprite->setPosition(ccp(0, 0));
	sprite->setAnchorPoint(ccp(0, 0));

	// 定制菜单
	MenuCtrl* menu = MenuCtrl::create();
	addChild(menu);

	//! 控制按键
	_textureDirNone = CCTextureCache::sharedTextureCache()->addImage("backKeyImage.png");
	_textureDirLeft = CCTextureCache::sharedTextureCache()->addImage("backKeyLeft.png");
	_textureDirRight = CCTextureCache::sharedTextureCache()->addImage("backKeyRight.png");

	_menuShow = CCSprite::createWithTexture(_textureDirNone);
	addChild(_menuShow);
	CCPoint ptmenuShowPos = ccp(70, 50);
	_menuShow->setPosition(ptmenuShowPos);

	//! 2个空的精灵
	CCSprite* left1 = CCSprite::create();
	CCSprite* left2 = CCSprite::create();
	left1->setContentSize(CCSizeMake(_textureDirNone->getContentSize().width / 2, _textureDirNone->getContentSize().height));
	left2->setContentSize(CCSizeMake(_textureDirNone->getContentSize().width / 2, _textureDirNone->getContentSize().height));
	CCMenuItemSprite* left = CCMenuItemSprite::create(left1, left2, this, menu_selector(LayerGame::moveLeftCallBack));
	menu->addChild(left);


	CCSprite* right1 = CCSprite::create();
	CCSprite* right2 = CCSprite::create();
	right1->setContentSize(CCSizeMake(_textureDirNone->getContentSize().width / 2, _textureDirNone->getContentSize().height));
	right2->setContentSize(CCSizeMake(_textureDirNone->getContentSize().width / 2, _textureDirNone->getContentSize().height));
	CCMenuItemSprite* right = CCMenuItemSprite::create(right1, right2, this, menu_selector(LayerGame::moveRightCallBack));
	menu->addChild(right);

	left->setPosition(ccp(ptmenuShowPos.x - winSize.width / 2, ptmenuShowPos.y - winSize.height / 2));
	right->setPosition(ccp(ptmenuShowPos.x - winSize.width / 2, ptmenuShowPos.y - winSize.height / 2));

	Common::moveNode(left, ccp(-_menuShow->getContentSize().width / 4, 0));
	Common::moveNode(right, ccp(_menuShow->getContentSize().width / 4, 0));

	_marioDir = Common::NONE;

	// 跳跃，发射子弹，菜单
	addJumpAndFireMenu();

}

void LayerGame::addJumpAndFireMenu()
{
	// jump menu
	{
		CCMenu* menu = CCMenu::create();
		addChild(menu);

		CCTexture2D* textureAB_Normal = CCTextureCache::sharedTextureCache()->addImage("AB_normal.png");
		CCTexture2D* textureAB_Select = CCTextureCache::sharedTextureCache()->addImage("AB_select.png");
		CCSprite* jumpNormal = CCSprite::createWithTexture(textureAB_Normal);
		CCSprite* jumpSelect = CCSprite::createWithTexture(textureAB_Select);
		CCMenuItemSprite* jump = CCMenuItemSprite::create(jumpNormal, jumpSelect, this, menu_selector(LayerGame::jumpCallBack));
		menu->addChild(jump);
		Common::moveNode(jump, ccp(192, -125));
	}
	// fire menu
	{
	CCMenu* menu = CCMenu::create();
	addChild(menu);

	CCTexture2D* textureAB_Normal = CCTextureCache::sharedTextureCache()->addImage("AB_normal.png");
	CCTexture2D* textureAB_Select = CCTextureCache::sharedTextureCache()->addImage("AB_select.png");
	CCSprite* jumpNormal = CCSprite::createWithTexture(textureAB_Normal);
	CCSprite* jumpSelect = CCSprite::createWithTexture(textureAB_Select);
	CCMenuItemSprite* jump = CCMenuItemSprite::create(jumpNormal, jumpSelect, this, menu_selector(LayerGame::fireCallBack));
	menu->addChild(jump);
	Common::moveNode(jump, ccp(112, -125));
}
}

void LayerGame::fireCallBack(CCObject*)
{
	_mario->fire();

	/*CC_SAFE_RELEASE(Item:: _itemReward);
	GameInfo::Utime = 300;
	LayerGame::_idx++;
	GameInfo::ULevel = LayerGame::_idx + 1;
	Mario::_life = 3;

	CCDirector::sharedDirector()->replaceScene(Common::scene(LayerGame::create(LayerGame::_idx)));*/
}

void LayerGame::jumpCallBack(CCObject*)
{
	_mario->jump();
}

void LayerGame::addLeaderAndMon()
{
	// 加载地图对象元素
	_mario = Mario::create();

	CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
	CCArray* objs = objGroup->getObjects();
	CCObject* obj;
	CCARRAY_FOREACH(objs, obj)
	{
		CCDictionary* dict = (CCDictionary*)obj;
		const CCString* x = dict->valueForKey("x");
		const CCString* y = dict->valueForKey("y");
		const CCString* type = dict->valueForKey("type");
		if (type->m_sString == "BirthPoint")
		{
			_mario->setPosition(ccp(x->intValue(), y->intValue() - _map->getTileSize().height));
			_mario->setAnchorPoint(ccp(0, 0));
			_map->addChild(_mario);
		}
		else
		{
			Item* item = Item::create(dict);

			if (item)
			{
				item->_mario = _mario;
				//	_items->addObject(item);
				_map->addChild(item);
			}
		}
	}

	openAntiAlias();
}

void LayerGame::openAntiAlias()
{
	CCDirector::sharedDirector()->setProjection(kCCDirectorProjection2D);
	CCArray *pChildrenArray = _map->getChildren();
	CCSpriteBatchNode *child = NULL;
	CCObject *pObject = NULL;
	CCARRAY_FOREACH(pChildrenArray, pObject){
		child = (CCSpriteBatchNode *)pObject;
		if (!child)
			break;
		child->getTexture()->setAntiAliasTexParameters();
	}
}

void LayerGame::openSchedule()
{
	schedule(schedule_selector(LayerGame::checkMarioDie));
	schedule(schedule_selector(LayerGame::checkMarioTouchPole));
	schedule(schedule_selector(LayerGame::moveMarioUpd));
	schedule(schedule_selector(GameInfo::scheduleTime), 1.0f);
}

void LayerGame::disaCall()
{
	{ monsterScore->setVisible(false); }
}

void LayerGame::addstopMenu()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu);

	CCTexture2D* textureAB_Normal = CCTextureCache::sharedTextureCache()->addImage("M_n.png");
	CCTexture2D* textureAB_Select = CCTextureCache::sharedTextureCache()->addImage("M_s.png");
	CCSprite* jumpNormal = CCSprite::createWithTexture(textureAB_Normal);
	CCSprite* jumpSelect = CCSprite::createWithTexture(textureAB_Select);
	CCMenuItemSprite* jump = CCMenuItemSprite::create(jumpNormal, jumpSelect, this, menu_selector(LayerGame::stopCallBack));
	menu->addChild(jump);
	Common::moveNode(jump, ccp(-5, -125));
}

void LayerGame::stopCallBack(CCObject *)
{
	LayerSetup* setup = LayerSetup::create();
	setup->game = this;
	Common::moveNode(setup, ccp(winSize.width, 0));
	CCCallFunc* pauseAction = CCCallFunc::create(this, callfunc_selector(LayerGame::setPauseFuncCall));
	CCMoveTo* move = CCMoveTo::create(0.5f, ccp(0, 0));
	CCActionEase *rate = CCEaseExponentialOut::create(move);
	setup->runAction(CCSequence::createWithTwoActions(rate, pauseAction));
	addChild(setup);

}

void LayerGame::setPauseFuncCall()
{
	CCArray *child = _map->getChildren();
	CCObject *obj;
	CCARRAY_FOREACH(child, obj)
	{
		Item * item = (Item *)obj;
		item->pauseSchedulerAndActions();
	}
	pauseSchedulerAndActions();
}

