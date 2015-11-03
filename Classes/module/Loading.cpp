#include "Loading.h"
#include "LBToolbarLayer.h"

#define MIN_WAIT_TIME					500

#define LOADING_TYPE_NONE				1   // 没有Loading，直接场景切换
#define LOADING_TYPE_FADE				2   // 有Loading，使用Fade场景切换效果
#define LOADING_TYPE_3STEP				3   // 有Loading，分3步自定义实现场景切换效果，3步为：
                                            //      1. 创建Loading界面，动画方式覆盖到当前场景
                                            //      2. 替换当前场景为Loading场景，此时释放资源
                                            //      3. 创建下一个场景，并将Loading覆盖到新场景，动画消失Loading界面

#define LOADING_TYPE_3STEP_ONE_SCENE    1   // 0 or 1 是否只使用1个场景来实现Loading的3个步骤(推荐)
                                            //      使用1个场景来实现的优势是Loading界面只会创建1次，且动画更加连贯、灵活


#define FADE_TIME						0.8f
#define FADE_COLOR						Color3B::BLACK


#define TAG_LOADING                     36214
#define ZORDER_LOADING                  99999

int Loading::s_loadingType = LOADING_TYPE_FADE;
bool Loading::s_isLoading = false;

Loading* Loading::gotoScene( GameNumber gameNumber, void* userData)
{
	// switch loading type
	//if (gameNumber == GameNumber::P002)
	//	s_loadingType = LOADING_TYPE_3STEP;
	//else
	//	s_loadingType = LOADING_TYPE_FADE;

	s_loadingType = LOADING_TYPE_3STEP;

	// goto scene
	if (s_loadingType == LOADING_TYPE_NONE || s_loadingType == LOADING_TYPE_FADE)
	{
		return Loading::gotoPhase(gameNumber, userData, Phase::kLoad);       // kLoad
	}
	else if (s_loadingType == LOADING_TYPE_3STEP)
	{
		return Loading::gotoPhase(gameNumber, userData, Phase::kBeforeLoad); // kBeforeLoad
	}
	else
	{
		return nullptr;
	}
}

Loading* Loading::gotoPhase( GameNumber gameNumber, void* userData, Phase phase )
{
    if (s_isLoading && phase == Phase::kBeforeLoad)
        return nullptr;

    s_isLoading = true;
	LBToolbar::setBackKeyEnabled(false);

    Loading* loading = nullptr;
    
    if (s_loadingType == LOADING_TYPE_NONE)
    {
        Director::getInstance()->replaceScene(TransitionFade::create(FADE_TIME,
                                                                     createNextScene(gameNumber, userData),
                                                                     FADE_COLOR));
        s_isLoading = false;
		LBToolbar::setBackKeyEnabled(true);
    }
    else if (s_loadingType == LOADING_TYPE_FADE)
    {
        loading = new Loading();
        loading->autorelease();
        loading->init(gameNumber, userData, phase);
        
        Scene *scene = Scene::create();
        scene->addChild(loading);
        Director::getInstance()->replaceScene(TransitionFade::create(FADE_TIME, scene, FADE_COLOR));
    }
    else if (s_loadingType == LOADING_TYPE_3STEP)
    {
        if (phase == Phase::kBeforeLoad)
        {
            loading = new Loading();
            loading->autorelease();
            loading->init(gameNumber, userData, phase);
            
            Scene *runningScene = Director::getInstance()->getRunningScene();
            runningScene->addChild(loading, ZORDER_LOADING, TAG_LOADING);
        }
        else if (phase == Phase::kLoad)
        {
#if (LOADING_TYPE_3STEP_ONE_SCENE)
            Scene *runningScene = Director::getInstance()->getRunningScene();
            
            Vector<Node*>& children = runningScene->getChildren();
            Vector<Node*> deleteChild;
            for (auto child : children)
            {
                if (dynamic_cast<Camera*>(child))
                    continue;
                else if (child->getTag() == TAG_LOADING)
                    loading = (Loading*)child;
                else
                    deleteChild.pushBack(child);
            }
            
            for (auto child : deleteChild)
                child->removeFromParent();
            
            loading->m_phase = phase;
            loading->doLoading();       // goto Phase 2 (kLoad)
            
#else
            loading = new Loading();
            loading->autorelease();
            loading->init(gameNumber, userData, phase);
            
            Scene *scene = Scene::create();
            scene->addChild(loading);
            
            Director::getInstance()->replaceScene(scene);   // replace scene Phase 2 (kLoad)
#endif
        }
        else if (phase == Phase::kAfterLoad)
        {
#if (LOADING_TYPE_3STEP_ONE_SCENE)
            Scene *runningScene = Director::getInstance()->getRunningScene();
            loading = static_cast<Loading*>(runningScene->getChildByTag(TAG_LOADING));
            loading->m_phase = phase;
            loading->doLoading();       // goto Phase3 (kAfterLoad)
#else
            loading = new Loading();
            loading->autorelease();
            loading->init(gameNumber, userData, phase); // replace scene Phase3 (kAfterLoad)
#endif
        }
    }
    
	return loading;
}

bool Loading::init( GameNumber gameNumber, void* userData, Phase phase )
{
	if ( !WJLayer::init() )
	{
		return false;
	}

	m_gameNumber = gameNumber;
	m_userData = userData;
	m_phase = phase;
	m_startShowTime = 0;
	m_listenerStart = nullptr;
	m_listenerComplete = nullptr;

	const Size& winSize = Director::getInstance()->getWinSize();

	setClickAble(true);
	noClickMoveEffect();
	setTouchSwallowsTouches(true);
	setCustomizeTouchRect(Rect(0, 0, winSize.width, winSize.height), false);

	m_loadingJson = WJLayerJson::create("game/json/loading.json");
	m_loadingJson->ignoreAnchorPointForPosition(false);
	m_loadingJson->setAnchorPoint(Vec2(0.5f, 0));
	m_loadingJson->setPosition(Vec2(winSize.width / 2, 0));
	if (m_phase == Phase::kBeforeLoad)
		m_loadingJson->setPositionY(winSize.height);
    
    m_loadingJson->playAnimation("aniLoading");

	this->addChild(m_loadingJson);

	return true;
}

void Loading::onEnterTransitionDidFinish()
{
	WJLayer::onEnterTransitionDidFinish();

	if (getUserTag() != 1)//只show loading时不自动调用动画
		doLoading();
}

void Loading::doLoading(bool isDoNothing)
{
    const Size& winSize = Director::getInstance()->getWinSize();
    
    if (m_phase == Phase::kBeforeLoad)
    {
        m_loadingJson->runAction(Sequence::create(
                                                  EaseBackOut::create(MoveTo::create(0.5f, Vec2(m_loadingJson->getPositionX(), 0))),
												  CallFunc::create([&, isDoNothing]()
                                                                   {
																	   if (isDoNothing)
																	   {
																		   m_phase = Phase::kLoad;
																		   doLoading(true);
																	   }
																	   else
																	   {
																		   // begin load
																		   Loading::gotoPhase(m_gameNumber, m_userData, Phase::kLoad);
																	   }
                                                                   }),
                                                  NULL));
    }
    else if (m_phase == Phase::kLoad)
    {
        m_startShowTime = WJUtils::millisecond();
        runAction(Sequence::create(
                                   DelayTime::create(0.1f),	// delay
								   CallFunc::create([&, isDoNothing]()
                                                    {
														if (isDoNothing)
														{
															m_phase = Phase::kAfterLoad;
															doLoading(true);
														}
														else
														{
															releaseResources();

															// load next scene
															loadNextSceneResources();

															// load next scene sound
															loadNextSceneSound();
														}
                                                    }), 
                                   NULL));
    }
    else if (m_phase == Phase::kAfterLoad)
    {
        Vec2 pos = m_loadingJson->getPosition();
        m_loadingJson->runAction(Sequence::create(
                                                  EaseSineOut::create(MoveTo::create(0.5f, Vec2(m_loadingJson->getPositionX(), winSize.height))),
                                                  CallFunc::create(CC_CALLBACK_0(Loading::onLoadScene3StepCompleted, this)),
                                                  NULL));
    }
}

void Loading::onExit()
{
	WJLayer::onExit();

	if (m_listenerStart)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(m_listenerStart);
		Director::getInstance()->getEventDispatcher()->removeEventListener(m_listenerComplete);
	}
}

void Loading::releaseResources()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	CCLOG("------------- LOADING: win32/mac don't release unused resources ------------- ");
#else
	CCLOG("------------- LOADING: release unused resources. ------------- ");

	// release skeleton data cache
	WJSkeletonDataCache::getInstance()->removeUnusedSkeletonData();

	// release snapsnot
	WJUtils::releaseLastScreenShot();

	// release 3d cache
	// Sprite3DCache::getInstance()->removeAllSprite3DData();

	// last, release textures
	Director::getInstance()->purgeCachedData();  // getTextureCache()->removeUnusedTextures();

	// release sound
	// if (WJUtils::isIosLowPerformanceDevice())
	// 全部释放
	Common::sound.clearAll();
#endif
}


void Loading::loadNextSceneResources()
{
	PLoader *loader = PLoader::getLoader(m_gameNumber);
	CCAssert(loader, "not found loader.");

	SEL_AsyncLoadSceneResource asyncLoadResource = loader->getAsyncLoadSceneSelector();
	if (asyncLoadResource)
	{
		m_asyncLoadTaskCount = 0;

		m_listenerStart = Director::getInstance()->getEventDispatcher()->addCustomEventListener(NOTIF_ASYNC_PRELOAD_START, 
										[&](EventCustom* event)
										{
											m_asyncLoadTaskCount++;
										});

		m_listenerComplete = Director::getInstance()->getEventDispatcher()->addCustomEventListener(NOTIF_ASYNC_PRELOAD_COMPLETE, 
										[&](EventCustom* event)
										{
											if (--m_asyncLoadTaskCount == 0)
											{
												// all resource load completed
												createAndShowNextScene();
											}
										});

		asyncLoadResource();
		if (WJLayerJsonPreload::getInstance()->getTaskCount() <= 0)
		{
			// sync load
			createAndShowNextScene();
		}
		// else
		//	async load, do nothing, wait completed.
	}
	else
	{
		// sync load
		createAndShowNextScene();
	}
}

void Loading::loadNextSceneSound()
{
	// load common sound
	Common::loadCommonEffect();

	/*
	switch (m_gameNumber)
	{
	case GameNumber::P001:
		Common::sound.preload("P001");
		break;

	//case GameNumber::XXX:
	//	Common::sound.preload("XXX")
	//	break;

	default:
		break;
	}
	*/
}

void Loading::createAndShowNextScene()
{
	Node *nextSceneNode = createNextSceneNode(m_gameNumber, m_userData);
	nextSceneNode->retain();

	long long t = WJUtils::millisecond();
	if (t - m_startShowTime < MIN_WAIT_TIME)
	{
		// wait some time
		runAction(Sequence::createWithTwoActions(
						DelayTime::create((MIN_WAIT_TIME - (t - m_startShowTime)) / 1000.0f),
						CallFunc::create(CC_CALLBACK_0(Loading::showNextScene, this, nextSceneNode))
						));
	}
	else
	{
		showNextScene(nextSceneNode);
	}
}

Scene* Loading::createNextScene( GameNumber gameNumber, void* userData )
{
    Node* node = createNextSceneNode(gameNumber, userData);
    
    Scene* scene = Scene::create();
    scene->addChild(node);
    
    return scene;
}

Node* Loading::createNextSceneNode( GameNumber gameNumber, void* userData )
{
	PLoader *loader = PLoader::getLoader(gameNumber);
	CCAssert(loader, "not found loader.");

	// call create scene func.
	return loader->getCreateSceneSelector()(userData);
}

void Loading::showNextScene(Node *nextSceneNode)
{
	if (s_loadingType == LOADING_TYPE_FADE)
	{
		// set as false
		s_isLoading = false;
		LBToolbar::setBackKeyEnabled(true);

        Scene* nextScene = Scene::create();
        nextScene->addChild(nextSceneNode);
		Director::getInstance()->replaceScene(TransitionFade::create(FADE_TIME, nextScene, FADE_COLOR));
	}
	else if (s_loadingType == LOADING_TYPE_3STEP)
	{
#if (LOADING_TYPE_3STEP_ONE_SCENE)
        Scene *runningScene = Director::getInstance()->getRunningScene();
        runningScene->addChild(nextSceneNode, 0);

		Loading::gotoPhase(m_gameNumber, m_userData, Phase::kAfterLoad);
#else
        Loading *loading = Loading::gotoPhase(m_gameNumber, m_userData, Phase::kAfterLoad);
        
        Scene* nextScene = Scene::create();
        nextScene->addChild(nextSceneNode);
        nextScene->addChild(loading);
        
        Director::getInstance()->replaceScene(nextScene);   // replace
#endif
	}

	nextSceneNode->release();
}

void Loading::onLoadScene3StepCompleted()
{
	removeFromParent();

	// set as false
	s_isLoading = false;
	LBToolbar::setBackKeyEnabled(true);
}

void Loading::showLoading()
{
	Loading* loading = new Loading();
	loading->autorelease();
	loading->init();

	Scene *runningScene = Director::getInstance()->getRunningScene();
	runningScene->addChild(loading, ZORDER_LOADING, TAG_LOADING);

	loading->doLoading(true);
}

bool Loading::init()
{
	if (!WJLayer::init())
	{
		return false;
	}

	m_phase = Phase::kBeforeLoad;
	setUserTag(1);
	const Size& winSize = Director::getInstance()->getWinSize();

	setClickAble(true);
	noClickMoveEffect();
	setTouchSwallowsTouches(true);
	setCustomizeTouchRect(Rect(0, 0, winSize.width, winSize.height), false);

	m_loadingJson = WJLayerJson::create("game/json/loading.json");
	m_loadingJson->ignoreAnchorPointForPosition(false);
	m_loadingJson->setAnchorPoint(Vec2(0.5f, 0));
	m_loadingJson->setPosition(Vec2(winSize.width / 2, 0));
	m_loadingJson->setPositionY(winSize.height);
	m_loadingJson->playAnimation("aniLoading");

	this->addChild(m_loadingJson);

	return true;
}