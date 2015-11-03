// Created by wangwenjie on 2013/04

#include "Splash.h"
#include "LBLibraryBase.h"
#include "WJUtils.h"

#include                "P001Welcome.h"
#define  FIRST_SCENE     P001

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	#define WAIT_TIME 0
#else
	#define WAIT_TIME 3000
#endif

Scene* Splash::createScene()
{
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();
	
	// 'layer' is an autorelease object
	Splash *layer = Splash::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Splash::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	m_startTime = WJUtils::millisecond();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    Size size = Director::getInstance()->getWinSize();
    Size frame = Director::getInstance()->getOpenGLView()->getFrameSize();
    
    float scale = 1.0f / Director::getInstance()->getOpenGLView()->getScaleX();
    float rotate = 0;
    bool isLandscape = frame.width > frame.height;
    
    const char* fileName;
    if (frame.width == 480 || frame.height == 480)  // iphone
    {
        fileName = "Default.png";
        rotate = isLandscape ? 90 : 0;
    }
    else if (frame.width == 960 || frame.height == 960)  // iphone-hd
    {
        fileName = "Default@2x.png";
        rotate = isLandscape ? 90 : 0;
    }
    else if (frame.width == 1136 || frame.height == 1136)  // iphone5
    {
        fileName = "Default-568h@2x.png";
        rotate = isLandscape ? 90 : 0;
    }
    else if (frame.width == 2048 || frame.height == 2048)  // ipad-hd
    {
        fileName = isLandscape ? "Default-Landscape@2x.png" : "Default-Portrait@2x.png";
    }
    else  // ipad
    {
        fileName = isLandscape ? "Default-Landscape.png" : "Default-Portrait.png";
    }
    
    Sprite *sprite = Sprite::create(fileName);
    sprite->setScale(scale);
    sprite->setRotation(rotate);
    sprite->setPosition(Vec2(size.width / 2, size.height / 2));
    
    this->addChild(sprite);

#endif
    
	return true;
}

void Splash::onGotoNextScene(float t)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, m_nextScene, Color3B::WHITE));
#else
	Director::getInstance()->replaceScene(m_nextScene);

	WJUtils::callaction_void(ACTION_VOID_GAME_INIT_COMPLETE);
#endif

	m_nextScene->release();
}

void Splash::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	onCreateNextScene(0);
#else
	this->scheduleOnce(schedule_selector(Splash::onCreateNextScene), 0);
#endif
}

void Splash::onCreateNextScene( float delay )
{
	// start load scene
	m_nextScene = FIRST_SCENE::createScene();
	m_nextScene->retain();

	long long t = WAIT_TIME - (WJUtils::millisecond() - m_startTime);
	if (t > 0)
	{
		// wait and replace scene
		this->scheduleOnce(schedule_selector(Splash::onGotoNextScene), t / 1000.0f);
	}
	else
	{
		onGotoNextScene(0);
	}
}


