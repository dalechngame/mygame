#include "PBase.h"
#include "Common.h"
#include "TestGameMenu.h"
#include "GameManager.h"

PBase::PBase() : WJLayer(), 
	m_userData(nullptr),
	m_sceneJson(nullptr),
	m_popupMenu(nullptr),
	m_snapShot(nullptr),
	m_adsLayer(nullptr),
	m_storeEventListener(nullptr),
	m_getfreeEventListener(nullptr),
	m_noviceFinger(nullptr),
	m_lockGameLayer(nullptr),
	m_photoButton(nullptr),
	m_nextButton(nullptr),
	m_star(nullptr),
	m_progress(nullptr),
	m_homeButton(nullptr)
{

}

PBase::~PBase()
{

}

bool PBase::init()
{
	if ( !WJLayer::init() )
	{
		return false;
	}

#if (COCOS2D_DEBUG)
	CCLOG("Init Scene: %s...", getGameName().c_str());
#endif

	m_winSize = Director::getInstance()->getWinSize();
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_origin = Director::getInstance()->getVisibleOrigin();
    m_visibleRect = Rect(m_origin.x, m_origin.y, m_visibleSize.width, m_visibleSize.height);

	// top layer
	m_topLayer = WJLayer::create();
	m_topLayer->saveCurrentPosition();
	this->addChild(m_topLayer, ZORDER_TOP_LAYER);

	// main
	if (getGameNumber() == GameNumber::P014_Main)
	{
		// android back key
		this->addChild(LBToolbar::createBackKey([&]()
		{
			// quit app
			WJUtils::callaction_void(ACTION_VOID_CONFIRM_QUIT);
		}));
	}

	initPopupMenu();
	initSnapshot();
	initAdsBanner();
	initLockGameLayer();
	initFingerLayer();
	initPhotoButton();
	initNextButton();
	initStar();
	initHomeButton();

	return true;
}

void PBase::initPhotoButton(const Vec2 &point)
{
	if (getGameNumber() != GameNumber::P014_Main)
	{
		// 拍照
		m_photoButton = WJButton::create("button/camera.png");
		m_photoButton->setClickAble(true);
		m_photoButton->setOnClick(CC_CALLBACK_2(PBase::onClickPhotoButton, this));
		m_photoButton->setPosition(point == Vec2::ZERO ? Vec2(m_winSize.width - 155, m_winSize.height - HOME_AND_PHOTOS_TOP_LENGTH) : point);
		m_photoButton->saveCurrentPosition();
		m_photoButton->setClickAniType(WJButton::ClickAniType::Scale);
		m_photoButton->setNormalAniType(WJButton::NormalAniType::QBreathing);
		m_topLayer->addChild(m_photoButton, ZORDER_PHOTO_BUTTON);
		m_photoButton->setPositionX(m_winSize.width - 60);
	}
}

void PBase::initNextButton()
{
	if (getGameNumber() != GameNumber::P014_Main)
	{
		m_nextButton = WJButton::create("button/next.png");
		m_nextButton->setClickAble(true);
		m_nextButton->setOnClick(CC_CALLBACK_2(PBase::onClickNextButton, this));
		m_nextButton->setPosition(Vec2(m_winSize.width - 60, m_winSize.height - HOME_AND_PHOTOS_TOP_LENGTH));
		m_nextButton->setVisible(false);
		m_nextButton->setUserTag(NEXT_BUTTON_HIDE_TAG);
		m_nextButton->setNormalAniType(WJButton::NormalAniType::Breathing);
		m_nextButton->saveCurrentProperties();
		m_topLayer->addChild(m_nextButton, ZORDER_NEXT_BUTTON);
	}
}

void PBase::initStar(const Vec2 &point)
{
	// 星星
	m_star = Star::create();
	m_star->setPosition(point == Vec2::ZERO ? Vec2(200, m_winSize.height - 90) : point);
	m_topLayer->addChild(m_star, 100);
}

void PBase::initProgressBar(const Vec2 &pointWorld, float scale, const char * jsonFile)
{
	if (getGameNumber() != GameNumber::P014_Main && getGameNumber() != GameNumber::P013)
	{
		m_progress = ProgressBar::create(jsonFile);
		 m_progress->setPosition(m_sceneJson->convertToNodeSpace( pointWorld == Vec2::ZERO ? 
		 		Vec2(40, m_winSize.height / 2) : pointWorld));
		m_progress->setScale(m_progress->getScale() * scale);
		m_sceneJson->addChild(m_progress, ZORDER_PROGRESS);
	}
}

void PBase::initHomeButton()
{
	if (getGameNumber() != GameNumber::P014_Main)
	{
		m_homeButton = WJButton::create("button/home.png");
		m_homeButton->setClickAble(true);
		m_homeButton->setOnClick(CC_CALLBACK_2(PBase::onClickHomeButton, this));
		m_homeButton->setPosition(Vec2(60, m_winSize.height - HOME_AND_PHOTOS_TOP_LENGTH));
		m_homeButton->setClickAniType(WJButton::ClickAniType::Scale);
		m_homeButton->setNormalAniType(WJButton::NormalAniType::QBreathing);
		m_topLayer->addChild(m_homeButton, 100);

		// android back key
		this->addChild(LBToolbar::createBackKey([&]()
		{
			// return home
			if (m_homeButton->isEnabled() && m_homeButton->isClickAble())
			{
				onClickHomeButton(m_homeButton, nullptr);
			}
		}));
	}
}

std::string PBase::getGameName()
{
	return Common::getGameName(getGameNumber());
}

void PBase::initLockGameLayer()
{
	m_lockGameLayer = LockGameLayer::create(getGameNumber());
	this->addChild(m_lockGameLayer, ZORDER_LOCK_LAYER);
}

void PBase::initFingerLayer()
{
	// 提示
	m_noviceFinger = NoviceFinger::create();
	m_noviceFinger->setPosition(SCREEN_CENTER);
	this->addChild(m_noviceFinger, ZORDER_FINGER_LAYER);
}

void PBase::initSceneJson( const char* jsonFileName )
{
	if (m_sceneJson)
		return;

	m_sceneJson = WJLayerJson::create(jsonFileName);
	m_sceneJson->ignoreAnchorPointForPosition(false);
	m_sceneJson->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_sceneJson->setPosition(SCREEN_CENTER);

	this->addChild(m_sceneJson, ZORDER_SCENE_JSON);

	initProgressBar();
}

void PBase::initPopupMenu()
{
	if (m_popupMenu || getGameNumber() != GameNumber::P014_Main)
		return;

	m_popupMenu = LBPopupMenu::create(CC_CALLBACK_2(PBase::onPopupMenuClick, this));
	m_popupMenu->setAutoCloseMenu(true);	// auto close

	// menu main button
	m_popupMenu->setMainButton("button/menu.png", m_winSize.width - 60, m_visibleRect.getMaxY() - 60, MENU_TAG_POPUP)
               ->setClickAniType(WJButton::ClickAniType::QBreathing)->noClickMoveEffect();

	if (!Store::isUnlockedAll())
	{
		// store button
		m_popupMenu->addHorizontalButton("button/store_popup.png", MENU_TAG_STORE)->setClickAniType(WJButton::ClickAniType::QBreathing)->noClickMoveEffect();
	}

	// home button
	m_popupMenu->addHorizontalButton("button/home.png", MENU_TAG_GOHOME)->setClickAniType(WJButton::ClickAniType::QBreathing)->noClickMoveEffect();

	//if (getGameNumber() != GameNumber::P002)
	//{
	//	// back button
	//	m_popupMenu->addHorizontalButton("button/back.png", MENU_TAG_BACK)->setClickAniType(WJButton::ClickAniType::QBreathing)->noClickMoveEffect();
	//}

	// music button
	m_popupMenu->addVerticalButton("button/music_on.png", MENU_TAG_MUSIC)->setClickAniType(WJButton::ClickAniType::QBreathing)->noClickMoveEffect();
	// camera button
	m_popupMenu->addVerticalButton("button/camera.png", MENU_TAG_CAMERA)->setClickAniType(WJButton::ClickAniType::QBreathing)->noClickMoveEffect();
	// rate button
	m_popupMenu->addVerticalButton("button/rate.png", MENU_TAG_RATE)->setClickAniType(WJButton::ClickAniType::QBreathing)->noClickMoveEffect();
	// moregame button(fixed)
	m_popupMenu->addFixedVerticalButton("button/moregame.png", MENU_TAG_MOREGAME)->setClickAniType(WJButton::ClickAniType::QBreathing)->noClickMoveEffect();

	// set music button textures
	m_popupMenu->setMusicButtonFile("button/music_on.png", "button/music_off.png");
	

	m_popupMenu->saveCurrentPosition();
	m_topLayer->addChild(m_popupMenu, ZORDER_POPUPMENU);
}

void PBase::initSnapshot()
{
	if (m_snapShot)
		return;

	m_snapShot = LBSnapshotLayer::create(CC_CALLBACK_2(PBase::onSnapshotClick, this));

	m_snapShot->addButton("button/snapshot/close.png", MENU_TAG_CLOSE);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (WJUtils::getDeviceOSVersion() >= 6.0f)
	{
		m_snapShot->addButton("button/snapshot/facebook.png", MENU_TAG_FACEBOOK, 0.8f * TO_SCALE_HEIGHT, 0, 0, "_", "selected");
	}
#else
	m_snapShot->addButton("button/snapshot/facebook.png", MENU_TAG_FACEBOOK, 0.8f * TO_SCALE_HEIGHT, 0, 0, "_", "selected");
#endif
	m_snapShot->addButton("button/snapshot/email.png", MENU_TAG_EMAIL, 0.7f * TO_SCALE_HEIGHT, 0, 0, "_", "selected");
	m_snapShot->addButton("button/snapshot/photos.png", MENU_TAG_PHOTOS, 0.7f * TO_SCALE_HEIGHT, 0, 0, "_", "selected");
	this->addChild(m_snapShot, ZORDER_SNAPSHOT);
}


void PBase::initAdsBanner()
{
	if (m_adsLayer || Common::isAdsRemoved())
		return;

	Size adsSize = Common::getAdsSize();
	Vec2 adsLayerPosHide = Vec2(0, m_visibleRect.getMinY() - adsSize.height - 5);

	m_adsLayer = LayerColor::create(Color4B(110, 110, 110, 125));
	m_adsLayer->setContentSize(Size(m_winSize.width, adsSize.height));
	m_adsLayer->ignoreAnchorPointForPosition(false);
	m_adsLayer->setAnchorPoint(Vec2(0, 1.0f));
	m_adsLayer->setPosition(adsLayerPosHide);
	m_adsLayerVisible = false;

	this->addChild(m_adsLayer, ZORDER_ADS_LAYER);

	refreshAdBanner(0);
	this->runAction(RepeatForever::create(
										Sequence::createWithTwoActions(
												DelayTime::create(0.2f),
												CallFunc::create(CC_CALLBACK_0(PBase::refreshAdBanner, this, 0.2f)))));
}

void PBase::refreshAdBanner(float duration)
{
	if (!m_adsLayer)
		return;

	if (WJUtils::isAdsReallyVisible())
	{
		if (!m_adsLayerVisible)
		{
			m_adsLayerVisible = true;
           
			Vec2 adsLayerPos = Vec2(0, m_visibleRect.getMinY() + Common::getAdsSize().height);
            
			m_adsLayer->stopAllActions();
			m_adsLayer->runAction(MoveTo::create(duration, adsLayerPos));

			// move down top layer
		/*	Vec2 pos = m_topLayer->getSavedPosition();
			Vec2 mp = Vec2(0, m_adsLayer->getContentSize().height + 10);
			m_topLayer->stopAllActions();
			m_topLayer->runAction(MoveTo::create(duration, pos - mp));*/
		}
	}
	else if (m_adsLayerVisible)
	{
		m_adsLayerVisible = false;
        
        Size adsSize = Common::getAdsSize();
        Vec2 adsLayerPosHide = Vec2(0, m_visibleRect.getMinY() - adsSize.height - 5);
        
		m_adsLayer->stopAllActions();
		m_adsLayer->runAction(MoveTo::create(duration, adsLayerPosHide));

		//// move up top layer
		//m_topLayer->stopAllActions();
		//m_topLayer->restoreSavedPosition(duration);
	}
}

void PBase::onPopupMenuClick( Node* node, WJTouchEvent* event )
{
	int tag = node->getTag();

	switch (tag)
	{
		case MENU_TAG_POPUP:
			Common::sound.play("Common:button");
			break;

		case MENU_TAG_GOHOME:
			Common::sound.play("Common:button");
			Loading::gotoScene(GameNumber::P014_Main);
			break;

		case MENU_TAG_BACK:
			Common::sound.play("Common:button");
			Loading::gotoScene(GameNumber::P002);
			break;

		case MENU_TAG_MOREGAME:
			Common::sound.play("Common:popup");
			if (DEBUG_APP)
				TestGameMenu::show();
			else
				WJUtils::showMoreGameDialog(THIS_APP_ID, URL_MOREGAME_DEF);

			break;

		case MENU_TAG_RATE:
			showInfoDialog();
			break;

		case MENU_TAG_STORE:
			Store::showStore();
			break;

		case MENU_TAG_CAMERA:
			if (WJUtils::canClick("camera", 2000))
			{
                showSnapshot();
			}
			break;
	}
}

void PBase::showSnapshot(const char* fileName /*= NULL*/)
{
    Sprite* spriteFromFile = nullptr;
    Sprite*	spriteLogo = nullptr;
    
    onSnapshotBefore();
    Common::sound.play("Common:camera");
    
    Rect rect(0, 0, 0, 0);
    
    if (fileName)
    {
        spriteFromFile = Sprite::create(fileName);
        
        float scale = m_visibleSize.width / spriteFromFile->getContentSize().width;
        spriteFromFile->setScale(scale);
        spriteFromFile->setAnchorPoint(Vec2::ZERO);
        spriteFromFile->setPosition(m_origin);
        
        this->addChild(spriteFromFile, ZORDER_SNAPSHOT);
        rect = spriteFromFile->getBoundingBox();
    }
    
    // app logo
    spriteLogo = Sprite::create("appIcon.png");
    spriteLogo->setScale(TO_SCALE_HEIGHT);
    spriteLogo->setAnchorPoint(Vec2(1, 0));
    spriteLogo->setPosition(Vec2(m_visibleRect.getMaxX() - 20, m_origin.y + 20));
    this->addChild(spriteLogo, ZORDER_SNAPSHOT);
    
    m_snapShot->snapShow(this, rect, fileName == nullptr);
    
    spriteLogo->removeFromParent();
    
    if (spriteFromFile)
    {
        Texture2D *texture = spriteFromFile->getTexture();
        spriteFromFile->removeFromParent();
        Director::getInstance()->getTextureCache()->removeTexture(texture);
    }
    
    onSnapshotAfter();
}

void PBase::onSnapshotClick( Node* node, WJTouchEvent* event )
{
	int tag = node->getTag();
	switch (tag)
	{
		case MENU_TAG_FACEBOOK:
			Common::sound.play("Common:button");
			WJUtils::sharePhoto(WJUtils::saveLastScreenShot(true).c_str(), FACEBOOK_SHARE, FACEBOOK_APPID);
			break;

		case MENU_TAG_EMAIL:
			{
				Common::sound.play("Common:button");
				std::string str(EMAIL_SHARE);
				str.append(WJUtils::saveLastScreenShot(true));
				WJUtils::callaction_void(ACTION_VOID_SENDMAIL, str.c_str());
			}
			break;

		case MENU_TAG_PHOTOS:
			if (WJUtils::canClick("photos", 2000))
			{
				Common::sound.play("Common:button");
				WJUtils::saveLastScreenShotToSystemAlbum(SAVETO_ALBUM_FOLDER, true);
			}
			break;

		case MENU_TAG_CLOSE:
			Common::sound.play("Common:close");
			onSnapshotClosed();
			break;
	}
}


void PBase::onSnapshotBefore()
{
	if (m_popupMenu)
		m_popupMenu->setVisible(false);

	if (m_adsLayer)
		m_adsLayer->setVisible(false);

	// hide ads banner
	WJUtils::hideAds();

	// move up top layer
	refreshAdBanner(0);
}

void PBase::onSnapshotAfter()
{

}

void PBase::onSnapshotClosed()
{
	if (m_popupMenu)
		m_popupMenu->setVisible(true);

	if (m_adsLayer)
		m_adsLayer->setVisible(true);

	// 恢复Banner广告显示
	showGameAds();

	// 弹Chartboost，拍照关闭为每次都弹，且弹Default。其它游戏地方请弹默认的 LevelComplete
	Common::showChartBoost(WJChartboostLocation::Default, true);
}

void PBase::onEnter()
{
	WJLayer::onEnter();

	// store notification
	m_storeEventListener = _eventDispatcher->addCustomEventListener(NOTIFI_STORE_IAP_PURCHASED, 
							[&](EventCustom* ec)
							{
								onStoreClosedAndPurchased();
							});

	// getfree notification
	m_getfreeEventListener = _eventDispatcher->addCustomEventListener(NOTIFI_GETFREE_REWARD_VIDEO_CLOSED, 
							[&](EventCustom* ec)
							{
								onGetFreeVideoClosed(WJUtils::getLastRewardVideoLockKey(), 
													 WJUtils::isGetFreeUnlocked(WJUtils::getLastRewardVideoLockKey()));
                                
                                if (WJUtils::isGetFreeUnlocked(WJUtils::getLastRewardVideoLockKey()))
                                {
                                    WJUtils::delayExecute(0.2f, [this](float delay)
                                    {
                                        onGetFreeVideoUnlocked(WJUtils::getLastRewardVideoLockKey());
                                    });
                                }
							});
}

void PBase::onEnterTransitionDidFinish()
{
	WJLayer::onEnterTransitionDidFinish();

	showGameAds();

	playGameBgMusic();

	showLockedGameLayer();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	// nothing
#else
	this->runAction(Sequence::createWithTwoActions(
		DelayTime::create(0.1f),
		CallFunc::create([]()
	{
		LOGD("------------- PBase: release unused resource.------------- ");
		WJSkeletonDataCache::getInstance()->removeUnusedSkeletonData();
		Director::getInstance()->purgeCachedData(); // getTextureCache()->removeUnusedTextures();
	})));
#endif
}

void PBase::showLockedGameLayer()
{
	if (m_lockGameLayer)
		m_lockGameLayer->show();
}

void PBase::onExit()
{
	_eventDispatcher->removeEventListener(m_storeEventListener);
	_eventDispatcher->removeEventListener(m_getfreeEventListener);

	WJLayer::onExit();

	WJUtils::hideAds();
}

void PBase::showGameAds()
{
	if (getGameNumber() != GameNumber::P001)
	{
		Common::showAds();
	}
}

void PBase::onStoreClosedAndPurchased()
{
	if (m_popupMenu && Store::isUnlockedAll())
	{
		m_popupMenu->removeItem(MENU_TAG_STORE);
	}

	/* 如果购买成功, 隐藏加锁层，正常游戏 */
	if (m_lockGameLayer && m_lockGameLayer->isVisible() && !m_lockGameLayer->isGameLocked())
	{
		m_lockGameLayer->runAction(Sequence::create(
			FadeOut::create(1.0f),
			Hide::create(),
		nullptr));
	}
}

void PBase::onGetFreeVideoClosed(const char* getFreeLockKey, bool isUnlocked)
{
	// override me.
}

void PBase::onGetFreeVideoUnlocked(const char* getFreeLockKey)
{
    // override me.
}

void PBase::playGameBgMusic()
{
	switch (getGameNumber())
	{
	case GameNumber::P001:
	case GameNumber::P002:
	case GameNumber::P003:
		Common::sound.playBackgroundMusic("BG:001");
		break;

	default:
		Common::sound.playBackgroundMusic("BG:001");
		break;
	}
}

void PBase::showInfoDialog()
{
	Common::sound.play("Common:popup");

	WJLayerJson *info = WJLayerJson::create("infoDialog/infoDialog.json");
	info->ignoreAnchorPointForPosition(false);
	info->setPosition(SCREEN_CENTER);
	info->setAnchorPoint(Vec2(0.5f, 0.5f));

	auto onClick = [=](Node* node, WJTouchEvent* event)
	{
		const char* key = info->getSubKeyByNode(node);
		if (WJUtils::equals(key, "close"))
		{
			Common::sound.play("Common:close");
			static_cast<LBModalDialog*>(info->getParent())->close();
		}
		else if (WJUtils::equals(key, "likeUs"))
		{
			Common::sound.play("Common:button");
			if (APP_LANG_IS_CN)
				WJUtils::openURL(URL_WECHAT);
			else
				WJUtils::openURL(URL_FACEBOOK);
		}
		else if (WJUtils::equals(key, "contactUs"))
		{
			Common::sound.play("Common:button");

			std::string strTitle = EMAIL_FEEDBACK;
			std::string verName = WJUtils::callaction_retstr(ACTION_RETSTR_GET_THISAPP_VERSION_NAME);
			WJUtils::callaction_void(ACTION_VOID_SENDMAIL, strTitle.append(verName).c_str());
		}
		else if (WJUtils::equals(key, "rateUs"))
		{
			Common::sound.play("Common:button");
			WJUtils::promptForRating(NULL, RATE_MESSAGE, RATE_URL, RATE_NEW_VERSION_RATE_AGAIN);
		}
	};
	info->setSubNodeOnClick("close", onClick)->nodeInstance()->setTag(MENU_TAG_BACK);  // for android back button
	info->setSubNodeOnClick("likeUs", onClick);
	info->setSubNodeOnClick("contactUs", onClick);
	info->setSubNodeOnClick("rateUs", onClick);

	LBModalDialog *dialog = LBModalDialog::create(info, true);
	Director::getInstance()->getRunningScene()->addChild(dialog);

	dialog->show(LBModalDialogAnimationType::Scale);
}

void PBase::startMoveFingerTips(Vec2 &fromPointWorld, Vec2 &toPointWorld, int repeatCount /*= 2*/, float interval /*= 1.0f*/)
{
	m_noviceFinger->startMoveFingerTips(fromPointWorld, toPointWorld, repeatCount, interval);
}

void PBase::startMoveFingerTipsForever(Vec2 &fromPointWorld, Vec2 &toPointWorld, float interval /*= 6.0f*/)
{
	m_noviceFinger->startMoveFingerTipsForever(fromPointWorld, toPointWorld, interval);
}

void PBase::stopMoveFingerTips()
{
	m_noviceFinger->stopMoveFingerTips();
}

void PBase::delayStartMoveFingerTips(Vec2 &fromPointWorld, Vec2 &toPointWorld, int repeatCount /*= 2*/, float interval /*= 1.0f*/, float delay /*= 6.0f*/)
{
	m_noviceFinger->delayStartMoveFingerTips(fromPointWorld, toPointWorld, repeatCount, interval, delay);
}

void PBase::delayStartMoveFingerTipsForever(Vec2 &fromPointWorld, Vec2 &toPointWorld, float interval /*= 6.0f*/, float delay /*= 6.0f*/)
{
	m_noviceFinger->delayStartMoveFingerTipsForever(fromPointWorld, toPointWorld, interval, delay);
}

void PBase::onClickPhotoButton(Node *node, WJTouchEvent *event)
{
	if (WJUtils::canClick("camera", 2000))
	{
		//showSnapshot();
		onSnapshotBefore();
		Common::sound.play("Common:camera");

		SavePhotoDialog *dialog = SavePhotoDialog::create("game/json/savealbum.json", WJUtils::captureScreen(this));
		dialog->setClickButtonEndCallBack(CC_CALLBACK_2(PBase::onSnapshotClick, this));
		Director::getInstance()->getRunningScene()->addChild(dialog, 9999);
		dialog->show(LBModalDialogAnimationType::ScaleEaseBack);
	}
}

void PBase::onClickNextButton(Node *node, WJTouchEvent *event)
{
	Common::sound.play("Common:button");
}

void PBase::showNextButton(bool isShowPar)
{
	if (m_nextButton->isVisible()) return;

	// 显示下一步评星按钮
	m_photoButton->restoreSavedPosition(0.5f);
	m_nextButton->setVisible(true);
	m_nextButton->stopAllActions();
	m_nextButton->setUserTag(NEXT_BUTTON_SHOW_TAG);
	m_nextButton->runAction(Sequence::create(
		CCDelayTime::create(0.3f),
		ScaleTo::create(0.1f, 1.4f * m_nextButton->getSavedScale()),
		ScaleTo::create(0.1f, 1.0f * m_nextButton->getSavedScale()),
		ScaleTo::create(0.1f, 1.2f * m_nextButton->getSavedScale()),
		ScaleTo::create(0.1f, 1.0f * m_nextButton->getSavedScale()),
		ScaleTo::create(0.1f, 1.1f * m_nextButton->getSavedScale()),
		ScaleTo::create(0.1f, 1.0f * m_nextButton->getSavedScale()),
		CCCallFuncN::create(CC_CALLBACK_1(PBase::onShowNextButtonEnded, this)),
		NULL));

	if (isShowPar)
	{
		WJUtils::playParticle("particles/fangshe.plist", m_nextButton, 1000, false);
	}
}

void PBase::hideNextButton()
{
	if (!m_nextButton->isVisible()) return;

	m_nextButton->setVisible(false);
	m_photoButton->runAction(MoveTo::create(0.5f, Vec2(m_winSize.width - 60, m_photoButton->getPositionY())));
}

void PBase::onShowNextButtonEnded(Node *node)
{
	// next 显示出来之后一直播放粒子效果和每隔10s缩放一次
	WJUtils::playParticle("particles/hint.plist", m_nextButton, 1000, false);
	m_nextButton->setNormalAniType(WJButton::NormalAniType::QBreathing);
	m_nextButton->runAction(RepeatForever::create(Sequence::create(
		CCDelayTime::create(10.0f),
		ScaleTo::create(0.3f, 1.4f * m_nextButton->getSavedScale()),
		ScaleTo::create(0.3f, 1.0f * m_nextButton->getSavedScale()),
		ScaleTo::create(0.3f, 1.2f * m_nextButton->getSavedScale()),
		ScaleTo::create(0.3f, 1.0f * m_nextButton->getSavedScale()),
		NULL)));
}

void PBase::onClickHomeButton(Node *node, WJTouchEvent *event)
{
	// 画面已经加载成功了后返回主场景,避免android在loading还在加载的时候点击back照成的未跳转场景但是
	// 屏幕已经不能点击的bug
	Loading* loading = Loading::gotoScene(GameNumber::P014_Main);
	if (loading)
	{
		Common::sound.stopAll();
		Common::sound.play("Common:button");
		this->setEnabled(false);
		NotificationCenter::getInstance()->postNotification("gohome");
	}
}

void PBase::setProgressStarNum(ProgressStar star, bool playSound)
{
	m_progress->setProgressStarNum(star, playSound);
}

Star* PBase::getStar()
{
	return m_star;
}

void PBase::executeCommand(float delay, bool sceneEnable)
{
	this->runAction(Sequence::create(
		DelayTime::create(delay),
		CCCallFunc::create([=]() {
			if (GManager->isExistGameCommand())
			{
				this->setEnabled(sceneEnable);
				GManager->commandVector.front()->execute(this);
			}
		}),
		nullptr));
}

WJLayerJson* PBase::getLayerJson()
{
	return m_sceneJson;
}

ProgressStar PBase::getProgressStarNum()
{
	return m_progress->getStarNum();
}

void PBase::showStarDialog(DiySaveType saveType, Node *node)
{
	this->setEnabled(false);
	ShowStar *star = ShowStar::create(getProgressStarNum());
	star->show(this, 5000);
	star->saveItem(saveType, node);
}

void PBase::showCameraTipsAction(bool isShowPar)
{
	PUtils::playNodeScaleAni(m_photoButton);
	float t = 0.f;
	if (isShowPar)
	{
		t = WJUtils::playParticle("particles/fangshe.plist", m_photoButton, 1000)->getDuration();
	}
	m_photoButton->runAction(Sequence::createWithTwoActions(
		DelayTime::create(t),
		CallFunc::create([&]()
	{
		WJUtils::playParticle("particles/Hint.plist", m_photoButton, 1000);
	})));
}
