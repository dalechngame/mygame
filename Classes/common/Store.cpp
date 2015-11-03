// Created by wangwenjie on 2015/01

#include "Star.h"
#include "Store.h"
#include "TipDialog.h"

#define RESTORE_TAG     9001
#define PURCHASED_TAG	9002

#define STORE_JSON				"game/json/shopbig.json"
#define STORE_MINI_JSON			"game/json/shopmini.json"

#define JSON_EVERYTHING_KEY		"img_everything"
#define JSON_ALL_ITEM_KEY		"img_item"
#define JSON_ALL_DRESS_KEY		"img_dress"
#define JSON_HAIR_PACK_KEY		"img_hair_pack"
#define JSON_TREAT_PACK_KEY		"img_treat_pack"
#define JSON_COOK_PACK_KEY		"img_cook_pack"

#define JSON_MINI_KEY			"img_mini"
#define JSON_NO_ADS_KEY			"noads"
#define JSON_CLOSE_KEY			"close"
#define JSON_MAIN_STORE_KEY		"main_store"
#define JSON_RESTORE_KEY		"restore"

#define NO_ADS_TIP_JSON			"game/json/getfree.json"

Store* Store::showStore()
{
	return showStoreMini("");
}

Store* Store::showStoreMini( const char* key )
{
	if (Store::isUnlockedAll() || (key && strlen(key) > 0 && Common::isInAppPurchasedByKey(key)))
	{
		CCAssert(false, "bug");
		return nullptr;
	}

	Store *store = new Store();
	if (store && !store->init(key))
	{
		CC_SAFE_DELETE(store);
		return nullptr;
	}

	store->autorelease();

	Common::sound.play("Common:popup");

	// show
	Scene *runningScene = Director::getInstance()->getRunningScene();
	runningScene->addChild(store);

	store->show(LBModalDialogAnimationType::ScaleEaseBack);
	if (!key || strlen(key) <= 0)
	{
		store->runAction(Sequence::create(DelayTime::create(0.3f),
										  CallFunc::create(CC_CALLBACK_0(Store::showStoreEnterAnimation, store)),
										  nullptr));
	}
	else
	{
		store->runAction(Sequence::create(DelayTime::create(0.3f),
										  CallFunc::create(CC_CALLBACK_0(Store::showStoreMiniEnterAnimation, store)),
										  nullptr));
	}
    
    return store;
}


Store::Store() : LBModalDialog(), 
				 m_layerJson(nullptr),
				 m_purchased(false),
                 m_storeClosedCallback(nullptr),
                 m_adsIsVisible(false),
				 m_doClosedCallBack(false)
{}

Store::~Store()
{}

bool Store::init(const char* miniKey)
{
	// init
	m_miniKey = miniKey;

	if (m_miniKey.size() > 0)
		initStoreMini();
	else
		initStore();

	m_layerJson->playAllAnimation();
	m_layerJson->setSubNodeOnClick(JSON_CLOSE_KEY, [&](Node* node, WJTouchEvent* event)
	{
		closeStore();
	})->nodeInstance()->setTag(MENU_TAG_BACK);


	const Size& winSize = Director::getInstance()->getWinSize();
	m_layerJson->ignoreAnchorPointForPosition(false);
	m_layerJson->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_layerJson->setPosition(winSize.width / 2, winSize.height / 2);

	// init dialog
	if (!LBModalDialog::init(m_layerJson, true))
	{
		CC_SAFE_DELETE(m_layerJson);
		return false;
	}

	return true;
}

bool Store::isUnlockedAll()
{
	std::string unlockAllIapId = Common::getUnlockAllIapId();
	if (Common::isInAppPurchased(unlockAllIapId.c_str(), false))
		return true;

	std::string unlockNoAdsIapId = Common::getUnlockNoAdsIapId();

	ValueVector infoArray = Common::getProductInfoArray();
	int n = 0;
	for (auto it = infoArray.begin(); it != infoArray.end(); it++)
	{
		ValueMap& dict = it->asValueMap();
		std::string productId = dict["productID"].asString();
		if (productId.compare(unlockAllIapId) == 0 || productId.compare(unlockNoAdsIapId) == 0)
			continue;

		n++;
		if (!Common::isInAppPurchased(productId.c_str(), false))
			return false;
	}

	return n > 0;
}

std::string Store::getPrice( const char* iapId, ValueVector* productInfoArray )
{
	for (auto it = productInfoArray->begin(); it != productInfoArray->end(); it++)
	{
		ValueMap& dict = it->asValueMap();
		if (dict["productID"].asString().compare(iapId) == 0)
		{
			return dict["productPrice"].asString();
		}
	}

	return "";
}

std::string Store::getPrice( const char* iapId )
{
	ValueVector infoArray = Common::getProductInfoArray();

	return getPrice(iapId, &infoArray);
}

void Store::doCloseHide()
{
	LBModalDialog::doCloseHide();
}

void Store::doShowAnimation( int animationType )
{
	// 默认效果
	LBModalDialog::doShowAnimation(animationType);

	// 自定义动画效果
	/*
	Node *node = m_contentNode->nodeInstance();
	node->stopAllActions();

	const Size& winSize = Director::getInstance()->getWinSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	node->setPositionY(winSize.height - origin.y + m_contentNode->getContentScaleSize().height / 2 + 20);
	node->setVisible(true);
	node->runAction(EaseBackOut::create(MoveTo::create(0.5f, m_contentNode->getSavedPosition())));
	*/
}

void Store::doCloseAnimation( int animationType )
{
	// 默认效果
	LBModalDialog::doCloseAnimation(animationType);

	// 自定义动画效果
	/*
	Node *node = m_contentNode->nodeInstance();
	node->stopAllActions();

	const Size& winSize = Director::getInstance()->getWinSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Vec2 pos = Vec2(node->getPositionX(), winSize.height - origin.y + m_contentNode->getContentScaleSize().height / 2 + 20);

	node->runAction(MoveTo::create(0.3f, pos));
	*/
}


void Store::initStore()
{
	m_layerJson = WJLayerJson2x::create(STORE_JSON);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	m_layerJson->getSubNodeByKey(JSON_RESTORE_KEY)->setVisible(false);

#else
	m_layerJson->setSubNodeOnClick(JSON_RESTORE_KEY, [&](Node* node, WJTouchEvent* event)
	{
		Common::sound.play("Common:button");
		WJUtils::inAppRestore(RESTORE_TAG, CC_CALLBACK_2(Store::onInAppPurchaseCallback, this));
	});
#endif

	refreshStore();
}

void Store::initStoreMini()
{
	m_layerJson = WJLayerJson2x::create(STORE_MINI_JSON);

	m_layerJson->setSubNodeOnClick(JSON_MAIN_STORE_KEY, [&](Node* node, WJTouchEvent* event)
	{
        StoreClosedCallback callback = m_storeClosedCallback;
		closeStore(false);
        
		Store *store = Store::showStore();
        store->setOnClosed(callback);
	});

	refreshStoreMini();
}


void Store::refresh()
{
	if (m_miniKey.size() > 0)
		refreshStoreMini();
	else
		refreshStore();
}

void Store::refreshStore()
{
	initStoreItemButton(JSON_EVERYTHING_KEY, IAP_EVERYTHING_KEY, CC_CALLBACK_2(Store::onStoreItemClick, this));
	initStoreItemButton(JSON_ALL_ITEM_KEY, IAP_ALL_ITEM_KEY, CC_CALLBACK_2(Store::onStoreItemClick, this));
	initStoreItemButton(JSON_ALL_DRESS_KEY, IAP_ALL_DRESS_KEY, CC_CALLBACK_2(Store::onStoreItemClick, this));
	initStoreItemButton(JSON_HAIR_PACK_KEY, IAP_HAIR_PACK_KEY, CC_CALLBACK_2(Store::onStoreItemClick, this));
	initStoreItemButton(JSON_TREAT_PACK_KEY, IAP_TREAT_PACK_KEY, CC_CALLBACK_2(Store::onStoreItemClick, this));
	initStoreItemButton(JSON_COOK_PACK_KEY, IAP_COOK_PACK_KEY, CC_CALLBACK_2(Store::onStoreItemClick, this));
	initStoreItemButton(JSON_NO_ADS_KEY, IAP_NO_ADS_KEY, CC_CALLBACK_2(Store::onStoreNoAdsButtonClick, this));
}

void Store::refreshStoreMini()
{
	initStoreItemButton(JSON_EVERYTHING_KEY, IAP_EVERYTHING_KEY, CC_CALLBACK_2(Store::onStoreMiniItemClick, this));
	initStoreItemButton(JSON_MINI_KEY, m_miniKey.c_str(), CC_CALLBACK_2(Store::onStoreMiniItemClick, this));

	// load image
	if (m_miniKey.compare(IAP_ALL_ITEM_KEY) == 0)
	{
		m_layerJson->getSubSpriteByKey(JSON_MINI_KEY)->loadSpriteTexture("store/freeitems.png");
	}
	else if (m_miniKey.compare(IAP_ALL_DRESS_KEY) == 0)
	{
		m_layerJson->getSubSpriteByKey(JSON_MINI_KEY)->loadSpriteTexture("store/dress.png");
	}
	else if (m_miniKey.compare(IAP_HAIR_PACK_KEY) == 0)
	{
		m_layerJson->getSubSpriteByKey(JSON_MINI_KEY)->loadSpriteTexture("store/white.png");
	}
	else if (m_miniKey.compare(IAP_TREAT_PACK_KEY) == 0)
	{
		m_layerJson->getSubSpriteByKey(JSON_MINI_KEY)->loadSpriteTexture("store/blackgirl.png");
	}
	else if (m_miniKey.compare(IAP_COOK_PACK_KEY) == 0)
	{
		m_layerJson->getSubSpriteByKey(JSON_MINI_KEY)->loadSpriteTexture("store/asia.png");
	}
}

void Store::initStoreItemButton( const char* buttonName, const char* lockKey, const WJTouchCallback& onClick )
{
	WJSprite *button = m_layerJson->getSubSpriteByKey(buttonName);
	button->setUserString(lockKey);

	bool purchased = Common::isInAppPurchasedByKey(lockKey);
	
	button->setOnClick(onClick);
	button->setClickAble(!purchased);

	Node *purchaseNode = WJUtils::findChildByNameStartWith(button, "purchase");
	if (purchaseNode)
	{
		purchaseNode->setVisible(purchased);
	}

	Node* boxNode = WJUtils::findChildByNameStartWith(button, "box");
	if (boxNode)
	{
		boxNode->setVisible(!purchased);
		if (strcmp(buttonName, JSON_MINI_KEY) == 0)
		{
			if (strcmp(lockKey, IAP_HAIR_PACK_KEY) == 0)
				boxNode->setPosition(boxNode->getPosition() + Vec2(-40.0f, 6.0f));
			else if (strcmp(lockKey, IAP_TREAT_PACK_KEY) == 0)
				boxNode->setPosition(boxNode->getPosition() + Vec2(-240, 6.0f));
			else if (strcmp(lockKey, IAP_ALL_DRESS_KEY) == 0)
				boxNode->setPosition(boxNode->getPosition() + Vec2(-200, 4.0f));
			else if (strcmp(lockKey, IAP_ALL_ITEM_KEY) == 0)
				boxNode->setPosition(boxNode->getPosition() + Vec2(-40, 4.0f));
		}

		// 中文版的大商店调整
		if (ConfigLanguageSetting::instance().getCurLanguage() == LanguageType::CHINESE)
		{
			// 小商店
			if (strcmp(buttonName, JSON_MINI_KEY) == 0)
			{
				if (strcmp(lockKey, IAP_TREAT_PACK_KEY) == 0)
					boxNode->setPosition(boxNode->getPosition() + Vec2(18, -11));
			}
			// 大商店
			else
			{
				if (strcmp(lockKey, IAP_TREAT_PACK_KEY) == 0)
					boxNode->setPosition(boxNode->getPosition() + Vec2(0, -5));
				else if (strcmp(lockKey, IAP_HAIR_PACK_KEY) == 0)
					boxNode->setPosition(boxNode->getPosition() + Vec2(0, -0.5f));
			}
		}

	}

	Label *priceLabel = static_cast<Label*>(WJUtils::findChildByNameStartWith(button, "price"));
	if (priceLabel)
	{
		priceLabel->setVisible(!purchased);
		priceLabel->setString(getPrice(Common::getLockIapId(lockKey).c_str()).c_str());

		/* 在Mini Store里将价格标签移动至合适位置 */
		if (strcmp(buttonName, JSON_MINI_KEY) == 0)
		{
			if (strcmp(lockKey, IAP_HAIR_PACK_KEY) == 0)
				priceLabel->setPosition(priceLabel->getPosition() + Vec2(-40.0f, 6.0f));
			else if ( strcmp(lockKey, IAP_TREAT_PACK_KEY) == 0)
				priceLabel->setPosition(priceLabel->getPosition() + Vec2(-240, 6.0f));
			else if (strcmp(lockKey, IAP_ALL_DRESS_KEY) == 0)
				priceLabel->setPosition(priceLabel->getPosition() + Vec2(-200, 4.0f));
			else if (strcmp(lockKey, IAP_ALL_ITEM_KEY) == 0)
				priceLabel->setPosition(priceLabel->getPosition() + Vec2(-40, 4.0f));

		}

		// 中文版的大商店调整、在英文版移动的基础上在加个偏移
		if (ConfigLanguageSetting::instance().getCurLanguage() == LanguageType::CHINESE)
		{
			/* 在Mini Store里将价格标签移动至合适位置 */
			if (strcmp(buttonName, JSON_MINI_KEY) == 0)
			{
				if ( strcmp(lockKey, IAP_TREAT_PACK_KEY) == 0)
					priceLabel->setPosition(priceLabel->getPosition() + Vec2(18, -11));
			}
			else
			{
				if (strcmp(lockKey, IAP_TREAT_PACK_KEY) == 0)
					priceLabel->setPosition(priceLabel->getPosition() + Vec2(0, -5));
				else if (strcmp(lockKey, IAP_HAIR_PACK_KEY) == 0)
					priceLabel->setPosition(priceLabel->getPosition() + Vec2(0, -0.5f));
			}
		}
	}
	
	if (strcmp(buttonName, JSON_NO_ADS_KEY) != 0)
	{
		button->setVisible(false);
		button->setScale(0.01f);
	}
}

void Store::onStoreItemClick( Node* node, WJTouchEvent* event )
{
	Common::sound.play("Common:button");

	WJBase *base = dynamic_cast<WJBase*>(node);
	std::string lockKey = base->getUserString();

	WJUtils::inAppPurchase(PURCHASED_TAG, Common::getLockIapId(lockKey.c_str()).c_str(),
										  CC_CALLBACK_2(Store::onInAppPurchaseCallback, this));
}

void Store::onStoreNoAdsButtonClick(Node* pNode, WJTouchEvent* event)
{
	Common::sound.play("Common::button");

	WJBase* pBase = dynamic_cast<WJBase*>(pNode);
	std::string lockKey = pBase->getUserString();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	TipDialog* pTipDialog = TipDialog::create(NO_ADS_TIP_JSON);
	pTipDialog->setOnOkCallBack([=](TipDialog* pDialog)
								{
									Store* pStore = static_cast<Store*>(pDialog->getParent());
									WJUtils::inAppPurchase(PURCHASED_TAG, 
														   Common::getLockIapId(lockKey.c_str()).c_str(),
														   CC_CALLBACK_2(Store::onTipDialogInAppPurchaseCallback, pStore));

								});
	this->addChild(pTipDialog, 9999);
	pTipDialog->show();

#else
	WJUtils::inAppPurchase(PURCHASED_TAG, Common::getLockIapId(lockKey.c_str()).c_str(),
										  CC_CALLBACK_2(Store::onInAppPurchaseCallback, this));
#endif
}

void Store::onStoreMiniItemClick( Node* node, WJTouchEvent* event )
{
	onStoreItemClick(node, event);
}

void Store::onInAppPurchaseCallback( int tag, const char* value )
{
	if (tag == PURCHASED_TAG || (tag == RESTORE_TAG && *value == ACTION_RESULT_IAP_RESTORE_SUCCESS))
	{
		m_purchased = true;

		// 购买成功立即关闭，请根据需要控制
		bool purchaseOne = true;

		// * 第一次全部购买时赠送50金币 */
		if (tag == PURCHASED_TAG && isUnlockedAll() && UserDefault::getInstance()->getBoolForKey("IAPGold", true))
		{
			Common::sound.play("Common:0042_princess");
			Star::addStar(50);
			UserDefault::getInstance()->setBoolForKey("IAPGold", false);
			UserDefault::getInstance()->flush();
		}
		// 去除广告
		else if (tag == PURCHASED_TAG && WJUtils::equals(Common::getLockIapId("_unlockAds_").c_str(), value))
		{
			Common::sound.play("Common:0045_princess");
		}
		// 购买了公主服装包的时候播放
		else if (tag == PURCHASED_TAG && WJUtils::equals(Common::getLockIapId("Dress").c_str(), value))
		{
			Common::sound.play("Common:0043_princess");
		}
		// 购买工具包的时候播放
		else if (tag == PURCHASED_TAG && WJUtils::equals(Common::getLockIapId("Item").c_str(), value))
		{
			Common::sound.play("Common:0041_princess");
		}
		// 购买了游戏包的时候播放
		else if (tag == PURCHASED_TAG && (WJUtils::equals(Common::getLockIapId("TreatPack").c_str(), value) ||
			WJUtils::equals(Common::getLockIapId("HairPack").c_str(), value)) 
			|| WJUtils::equals(Common::getLockIapId("cookPack").c_str(), value))
		{
			Common::sound.play("Common:0044_princess");
		}

		if (purchaseOne || isUnlockedAll())
		{
			// 关闭
			closeStore();
		}
		else
		{
			// 刷新显示
			refresh();
		}
	}
}

void Store::onTipDialogInAppPurchaseCallback(int tag, const char* value)
{
	if (tag == PURCHASED_TAG)
	{
		// 去除广告
		if (WJUtils::equals(Common::getLockIapId("_unlockAds_").c_str(), value))
		{
			Common::sound.play("Common:0045_princess");
		}
		m_purchased = true;
		/* 延迟1秒关闭，达到先关闭顶层Dialog，再关闭Store的效果 */
		this->runAction(Sequence::create(DelayTime::create(1.0f),
										 CallFunc::create(CC_CALLBACK_0(Store::closeStore, this, true)),
										 nullptr));
	}
}

void Store::closeStore(bool bInvokeCloseEvent)
{
	if (m_purchased)
	{
		// notification
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NOTIFI_STORE_IAP_PURCHASED);
	}
    
	//if (bInvokeCloseEvent && m_storeClosedCallback)
	if (m_storeClosedCallback && m_doClosedCallBack)
	{
		m_storeClosedCallback(this);
	}

	Common::sound.play("Common:close");
    
    if (m_adsIsVisible && !WJUtils::isAdsVisible())
        Common::showAds();

    LBModalDialog::close();
}

void Store::setOnClosed(const StoreClosedCallback &callback)
{
    m_storeClosedCallback = callback;
}

void Store::onEnter()
{
    LBModalDialog::onEnter();
    
    m_adsIsVisible = WJUtils::isAdsVisible();
    if (m_adsIsVisible)
        WJUtils::hideAds();
}

void Store::showStoreEnterAnimation()
{
	this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(Store::showStoreItemButtonEnterAnimation, this, JSON_EVERYTHING_KEY, 0.0f)),
								     DelayTime::create(1.60f),
									 CallFunc::create(CC_CALLBACK_0(Store::showEverythingButtonQBreathingAnimation, this)),
									 nullptr));
													  
	showStoreItemButtonEnterAnimation(JSON_ALL_DRESS_KEY, 0.55f);
	showStoreItemButtonEnterAnimation(JSON_ALL_ITEM_KEY, 0.55f);
	showStoreItemButtonEnterAnimation(JSON_TREAT_PACK_KEY, 0.55f);
	showStoreItemButtonEnterAnimation(JSON_HAIR_PACK_KEY, 0.55f);
	showStoreItemButtonEnterAnimation(JSON_COOK_PACK_KEY, 0.55f);
}

void Store::showStoreMiniEnterAnimation()
{
	this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(Store::showStoreItemButtonEnterAnimation, this, JSON_EVERYTHING_KEY, 0.0f)),
								     DelayTime::create(1.60f),
									 CallFunc::create(CC_CALLBACK_0(Store::showEverythingButtonQBreathingAnimation, this)),
									 nullptr));

	showStoreItemButtonEnterAnimation(JSON_MINI_KEY, 0.55f);
}

void Store::showStoreItemButtonEnterAnimation(const char* buttonName, float delay)
{
	WJSprite* pButton = m_layerJson->getSubSpriteByKey(buttonName);
	pButton->setEnabled(false);
	if (delay <= 0)
	{
		pButton->runAction(Sequence::create(Show::create(),
											EaseBackOut::create(ScaleTo::create(1.0f, 1.0f)),
											CallFunc::create(CC_CALLBACK_0(WJSprite::setEnabled, pButton, true, false)), 
											nullptr));
	}
	else
	{
		pButton->runAction(Sequence::create(DelayTime::create(delay),
											Show::create(),
											EaseBackOut::create(ScaleTo::create(1.0f, 1.0f)),
											CallFunc::create(CC_CALLBACK_0(WJSprite::setEnabled, pButton, true, false)),
											nullptr));
	}
}

void Store::showEverythingButtonQBreathingAnimation()
{
	WJSprite* pButton = m_layerJson->getSubSpriteByKey(JSON_EVERYTHING_KEY);
	float scale = pButton->getScale();
	pButton->runAction(RepeatForever::create(Sequence::create(
															  ScaleTo::create(1.0f, scale * 0.95f, scale * 1.0f),
															  ScaleTo::create(1.0f, scale * 1.0f, scale * 0.95f),
															  nullptr)));
}