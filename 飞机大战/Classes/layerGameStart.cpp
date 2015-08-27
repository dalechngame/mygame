#include "layerGameStart.h"  			
#include "SimpleAudioEngine.h"
#include "AppMacros.h"
#include "LayerGameMain.h"
using namespace CocosDenshion;

#define loadMusic() SimpleAudioEngine::sharedEngine()

CCScene * layerGameStart::scene() 		
{ 						
	CCScene * scene = CCScene::create(); 	
	layerGameStart * layer = layerGameStart::create(); 
	scene->addChild(layer); 		
	return scene; 				
} 						

bool layerGameStart::init() 			
{ 						
	CCLayer::init(); 			
	
	addPreLoadMusic();
	addStartGamePicture();

	return true;				
} 						

void layerGameStart::addStartGamePicture()
{	
	//加载全局图片信息
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/shoot_background.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/shoot.plist");

	//加载当前gamestart页面所需要的图片,动画

	CCSprite * background = CCSprite::createWithSpriteFrameName("background.png");
	background->setAnchorPoint(ccp(0, 0));
	background->setPosition(ccp(0, 0));
	addChild(background);

	CCSprite *copyright = CCSprite::createWithSpriteFrameName("shoot_copyright.png");
	//! 屏幕适配的原因重新设置锚点
	copyright->setAnchorPoint(ccp(0.5, 0));
	copyright->setPosition(ccp(winSize.width / 2, winSize.height / 2));

/*CCLog("copyright:\n\n\nposition x = %f, position y = %f", copyright->getPositionX(), copyright->getPositionY());
CCLog("contentsize witdth = %f, contentsize height = %f", copyright->getContentSize().width, copyright->getContentSize().height);

CCLog("boundingbox x = %f, y = %f", copyright->boundingBox().origin.x, copyright->boundingBox().origin.y);

CCLog("boundingbox width = %f, height = %f", copyright->boundingBox().size.width, copyright->boundingBox().size.height);

CCLog("anchor x = %f, y = %f", copyright->getAnchorPoint().x, copyright->getAnchorPoint().y);

CCLog("isIgnore %d", copyright->isIgnoreAnchorPointForPosition());*/
	addChild(copyright);


	CCSprite *loading = CCSprite::createWithSpriteFrameName("game_loading1.png");
	loading->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 40));
	addChild(loading);

	CCAnimation *animation = CCAnimation::create();
	animation->setDelayPerUnit(0.2f);
	char nameBuf[100];
	for (int i = 0; i < 4; i++)
	{
		memset(nameBuf, 0, sizeof(nameBuf));
		sprintf(nameBuf, "game_loading%d.png", i + 1);
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(nameBuf));

	}
	CCAnimate *animate = CCAnimate::create(animation);
	CCRepeat *repeat = CCRepeat::create(animate, 3);
	CCCallFunc *loadingDone = CCCallFunc::create(this, callfunc_selector(layerGameStart::toMainGameCallback));
	CCSequence *seqence = CCSequence::create(repeat, loadingDone, NULL);

	loading->runAction(seqence);
}

void layerGameStart::addPreLoadMusic()
{
	loadMusic()->preloadBackgroundMusic("sound/game_music.wav");
	loadMusic()->preloadEffect("sound/achievement.wav");
	loadMusic()->preloadEffect("sound/big_spaceship_flying.wav");
	loadMusic()->preloadEffect("sound/bullet.wav");
	loadMusic()->preloadEffect("sound/button.wav");
	loadMusic()->preloadEffect("sound/enemy1_down.wav");
	loadMusic()->preloadEffect("sound/enemy2_down.wav");
	loadMusic()->preloadEffect("sound/enemy3_down.wav");
	loadMusic()->preloadEffect("sound/game_over.wav");
	loadMusic()->preloadEffect("sound/get_bomb.wav");
	loadMusic()->preloadEffect("sound/get_double_laser.wav");
	loadMusic()->preloadEffect("sound/out_porp.wav");
	loadMusic()->preloadEffect("sound/use_bomb.wav");
}

void layerGameStart ::toMainGameCallback()
{
	CCScene *scene = LayerGameMain::scene();
	CCDirector::sharedDirector()->replaceScene(scene);
}