// Created by wangwenjie on 2013/04

#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include <string>

#include "Common.h"
#include "ConfigLanguageSetting.h"

#include "Splash.h"

using namespace CocosDenshion;
USING_NS_CC;

AppDelegate::AppDelegate() : Application()
{
}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

	GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
	// initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
	if(!glview)
	{
		glview = GLViewImpl::create("Libii Game");

		// ------------------ Test For Desktop -----------
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
		// ----- ios -----
		//glview->setFrameSize(2048, 1536);		// ipad-hd
		//glview->setFrameSize(1024, 768);		// ipad1,2
		//glview->setFrameSize(512, 384);		// ipad1,2  half

		//glview->setFrameSize(960, 640);		// iphone4,4s

		glview->setFrameSize(1136, 640);      // iphone5
		//glview->setFrameSize(568, 320);       // iphone5 half

		// ----- amazon -----
		//glview->setFrameSize(502, 300);       // fire1 half

		//glview->setFrameSize(512, 300);       // fire2 half

		//glview->setFrameSize(1280, 800);      // fire-hd 7

		// ----- google play
		//glview->setFrameSize(1205, 800);      // nexus7

		//glview->setFrameSize(800, 480);       // 800 * 480

		//glview->setFrameSize(640, 360);       // i9300 half

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		RECT winRect;
		GetWindowRect(glview->getWin32Window(), &winRect);
		int winWidth = winRect.right - winRect.left;
		int winHeight = winRect.bottom - winRect.top;

		RECT winWorkArea;
		SystemParametersInfo(SPI_GETWORKAREA, NULL, &winWorkArea, NULL);
		int winScreenWidth = winWorkArea.right;     // GetSystemMetrics(SM_CXSCREEN);
		int winScreenHeight = winWorkArea.bottom;   // GetSystemMetrics(SM_CYSCREEN);

		MoveWindow(glview->getWin32Window(), (winScreenWidth - winWidth) / 2, (winScreenHeight - winHeight) / 2, winWidth, winHeight, false);
	#endif
#endif

		director->setOpenGLView(glview);
	}
    
    // ---------------- set default projection 2d/3d ---------
    director->setProjection(Director::Projection::_2D);

	// --------------------- Multi-Resolution ----------------
	glview->setDesignResolutionSize(1363, 768, ResolutionPolicy::FIXED_HEIGHT);

	// fullscreen width scale
	Size visibleSize = director->getVisibleSize();
	Size designSize = glview->getDesignResolutionSize();
	TO_SCALE_WIDTH = visibleSize.width / designSize.width;
	TO_SCALE_HEIGHT = visibleSize.height / designSize.height;

	// ---------------------- Language -----------------------
#if (APP_LANG_EN == 1)
	ConfigLanguageSetting::instance().setCurLanguage(LanguageType::ENGLISH);

#elif (APP_LANG_CN == 1)
	ConfigLanguageSetting::instance().setCurLanguage(LanguageType::CHINESE);

#elif (APP_LANG_AUTO == 1)
	// auto language
#else
	// auto language
#endif

	LanguageType lang = ConfigLanguageSetting::instance().getCurLanguage();
	if (lang == LanguageType::CHINESE)
	{
	    FileUtils::getInstance()->addSearchPath("zh-cn/", true);
	}
	// -----------------------------------------------------------

	// ------------------ For China Store ------------------------
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && CHINA_STORE == 1)
	WJUtils::setIapDisabled(true);
#endif
	// -----------------------------------------------------------

	// turn on display FPS
#if COCOS2D_DEBUG
	director->setDisplayStats(true);
#else
	director->setDisplayStats(false);
#endif

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);

	// create a scene. it's an autorelease object
	Scene* scene = Splash::createScene();

	// run
	director->runWithScene(scene);

	// ------------------- init ------------------------
	WJUtils::start(PACKAGE_NAME);
	WJUtils::randomize();

	m_timer.sessionStart();
	Common::loadCommonEffect();

	if (DEBUG_NO_IAP)
		WJUtils::setIapDisabled(true);

	if (DEBUG_NO_ADS)
		WJUtils::setAdsDisabled(true);

	// -------------------------------------------------


#if APP_TARGET == APP_TARGET_IOS
	LOGD("-- APP TARGET iOS --");
#elif APP_TARGET == APP_TARGET_KINDLE_FIRE
	LOGD("-- APP TARGET Amazon --");
#elif APP_TARGET == APP_TARGET_GOOGLE
	LOGD("-- APP TARGET Google --");
#elif APP_TARGET == APP_TARGET_WP8
	LOGD("-- APP TARGET WP8 --");
#else
	LOGD("-- APP TARGET Unknown --");
#endif


	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();
	
    // -------------------- enter background -----------
	WJUtils::pauseSound3x();
	m_pauseTime = WJUtils::millisecond();    
    // -------------------------------------------------
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
	
    // -------------------- enter foreground -----------
	WJUtils::resumeSound();

    bool bSessionResume = true;
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	long long mtime = WJUtils::millisecond();
	if (mtime - m_pauseTime < SESSION_TIME)
	{
        bSessionResume  = false;
		LOGD_L("session resume less than:", SESSION_TIME);
	}
#endif

    if (bSessionResume)
    {
        // show chartboost on app resume
		WJUtils::showChartboostOnAppResume();

        m_timer.sessionResume();
    }

	WJUtils::onAppResume(bSessionResume);

    // -------------------------------------------------
}

void AppDelegateTimer::sessionStart()
{
	Director::getInstance()->getScheduler()->schedule(schedule_selector(AppDelegateTimer::onSessionStartTimer), this, 0, 0, 2.0f, false);
}

void AppDelegateTimer::onSessionStartTimer(float t)
{
    // update moregame
    WJUtils::updateMoreGameData(URL_MOREGAME_UPDATE, true, THIS_APP_ID);
    
    // check rating
	WJUtils::checkRatingOnStartup(NULL, RATE_MESSAGE, RATE_URL, RATE_NEW_VERSION_RATE_AGAIN);
    
    // prepare FullScreen Interstitial AD
    WJUtils::prepareAdsFullScreen(GOOGLE_ADS_ID_INTERSTITIAL);

    // show Chartboost on app startup
	WJUtils::showChartboostOnAppStartup(true);
}

void AppDelegateTimer::sessionResume()
{
    Director::getInstance()->getScheduler()->schedule(schedule_selector(AppDelegateTimer::onSessionResumeTimer), this, 0, 0, 2.0f, false);
}

void AppDelegateTimer::onSessionResumeTimer( float t )
{
    // update moregame
    WJUtils::updateMoreGameData(URL_MOREGAME_UPDATE, true, THIS_APP_ID);

    // check rating
    WJUtils::checkRatingOnStartup(NULL, RATE_MESSAGE, RATE_URL, RATE_NEW_VERSION_RATE_AGAIN);
}
