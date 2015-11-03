#include "LockHelper.h"
#include "Store.h"
#include "Star.h"

#define STAR_PREFIX				"star_"
#define GETFREE_KEY				"test"

#define LOCK_AVAILABLE_STR		"available"
#define LOCK_PRICE_STR			"price"
#define LOCK_IAP_STR			"iap"
#define LOCK_GETFREE_STR		"getfree"

#define PRICE_SPRITE_TAG		10400
#define PRICE_LABEL_TAG			10410
#define IAP_SPRITE_TAG			10420
#define GETFREE_SPRITE_TAG		10430

#define PRICE_SPRITE_FILE		"game/01_common/starSmall.png"
#define PRICE_LABEL_FONT		"fonts/numbers02.fnt"
#define IAP_LOCK_FILE			"game/01_common/lockSmall.png"
#define GETFREE_FILE			"game/01_common/tryfreeSmall.png"

// singleton stuff
static LockHelper *s_lockHelper = NULL;

LockHelper* LockHelper::getInstance(void)
{
	if (!s_lockHelper)
	{
		s_lockHelper = new LockHelper();
		s_lockHelper->init();
	}

	return s_lockHelper;
}

bool LockHelper::init()
{
	return true;
}

void LockHelper::initLockOnScrollLayer(WJBase *itemBase, const char* key)
{
	if (itemBase)
	{
		LockStatus status = LockHelper::getItemLockStatusFromScrollItem(key);
		clearLockStatus(itemBase, key, status);
		ScrollItemTemplate object = ScrollItemTemplateTem(key);
		if (status == LockStatus::Available)
		{
			itemBase->setUserString(LOCK_AVAILABLE_STR);
		}
		else if (status == LockStatus::PriceLocked)
		{
			itemBase->setUserString(LOCK_PRICE_STR);
			LockHelper::addPriceSpriteAndLabel(itemBase, object.STARS, 1.0f);
		}
		else if (status == LockStatus::IAPLocked)
		{
			itemBase->setUserString(LOCK_IAP_STR);
			LockHelper::addIAPLockSprite(itemBase, 1.0f);
		}
		else if (status == LockStatus::GetfreeLocked)
		{
			itemBase->setUserString(LOCK_GETFREE_STR);
			LockHelper::addGetfreeSprite(itemBase, 1.0f);
		}
	}
}

LockStatus LockHelper::getItemLockStatusFromScrollItem(const char * key)
{
	ScrollItemTemplate lockEntry = ScrollItemTemplateTem(key);
	const char* IAPKey = LockHelper::getIAPKeyByLockType((IAPLockType)lockEntry.LockType);
	if (lockEntry.TRYFREE)
	{
		if (Common::isInAppPurchasedByKey(IAPKey, true) || WJUtils::isGetFreeUnlocked(key))
			return LockStatus::Available;

		/* 购买No Ads后不再显示Getfree */
		if (Common::isInAppPurchasedByKey(IAP_NO_ADS_KEY, true) || !WJUtils::isGetFreeRewardVideoReady(key))
			return LockStatus::IAPLocked;

		return LockStatus::GetfreeLocked;
	}
	else if (lockEntry.LOCK)
	{
		if (Common::isInAppPurchasedByKey(IAPKey, true))
			return LockStatus::Available;
		else return LockStatus::IAPLocked;
	}
	else if (lockEntry.STARS > 0)
	{
		bool bPurchased = isItemPurchased(key);
		return bPurchased ? LockStatus::Available : LockStatus::PriceLocked;
	}
	else
		return LockStatus::Available;
}

void LockHelper::clearLockStatus(WJBase* pItemBase, const char * key, LockStatus status)
{
	pItemBase->setUserString("");

	Node* pItemNode = pItemBase->nodeInstance();
	if (pItemNode)
	{
		Node* pPriceNode = pItemNode->getChildByTag(PRICE_SPRITE_TAG);
		if (pPriceNode)
		{
			pPriceNode->setVisible(false);
			pPriceNode->removeFromParentAndCleanup(true);
		}

		Node* pPriceLabel = pItemNode->getChildByTag(PRICE_LABEL_TAG);
		if (pPriceLabel)
		{
			pPriceLabel->setVisible(false);
			pPriceLabel->removeFromParentAndCleanup(true);
		}

		Node* pIAPNode = pItemNode->getChildByTag(IAP_SPRITE_TAG);
		if (pIAPNode)
		{
			pIAPNode->setVisible(false);
			pIAPNode->removeFromParentAndCleanup(true);
		}

		Node* pGetfreeNode = pItemNode->getChildByTag(GETFREE_SPRITE_TAG);
		if (pGetfreeNode)
		{
			// 当前播放的视频
			if (WJUtils::equals(WJUtils::getLastRewardVideoLockKey(), key) &&
				status == LockStatus::Available)
			{
				// 播放完了视频后
				playLookGetfreeVideoAfterAni(pItemNode);
			}
			else
			{
				pGetfreeNode->setVisible(false);
				pGetfreeNode->removeFromParentAndCleanup(true);
			}
		}
	}
}

void LockHelper::saveItemPurchased(const std::string& itemKey)
{
	std::string key = std::string(STAR_PREFIX).append(itemKey);
	UserDefault::getInstance()->setBoolForKey(key.c_str(), true);
	UserDefault::getInstance()->flush();
}

bool LockHelper::isItemPurchased(const std::string& itemKey)
{
	std::string key = std::string(STAR_PREFIX).append(itemKey);
	return UserDefault::getInstance()->getBoolForKey(key.c_str(), false);
}

const char* LockHelper::getIAPKeyByLockType(IAPLockType type)
{
	if (type == IAPLockType::Dress)
		return IAP_ALL_DRESS_KEY;
	else if (type == IAPLockType::Item)
		return IAP_ALL_ITEM_KEY;
	else if (type == IAPLockType::HairPack)
		return IAP_HAIR_PACK_KEY;
	else if (type == IAPLockType::TreatPack)
		return IAP_TREAT_PACK_KEY;
	else if (type == IAPLockType::CookPack)
		return IAP_COOK_PACK_KEY;
	else return nullptr;
}

void LockHelper::addPriceSpriteAndLabel(WJBase* pItemBase, int price, float scaleFactor)
{
	Node* pItemNode = pItemBase->nodeInstance();
    Size itemSize = pItemNode->getContentSize();

	std::string priceStr = WJUtils::stringAddInt("", price);
	Label* pPriceLabel = Label::createWithBMFont(PRICE_LABEL_FONT, priceStr, TextHAlignment::CENTER);
	pPriceLabel->ignoreAnchorPointForPosition(false);
	pPriceLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pPriceLabel->setPosition(Vec2(itemSize.width - 47, 23));
	if (scaleFactor > 1.0f)
		pPriceLabel->setScale(1.0f * scaleFactor * 0.8f);
	pPriceLabel->setTag(PRICE_LABEL_TAG);
	pItemNode->addChild(pPriceLabel, 210);
    
    WJSprite* pPriceSprite = WJSprite::create(PRICE_SPRITE_FILE);
    pPriceSprite->ignoreAnchorPointForPosition(false);
    pPriceSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pPriceSprite->setScale(0.8f * scaleFactor);
    pPriceSprite->setPosition(Vec2(itemSize.width - 35.0f, 30.0f));
    pPriceSprite->setTag(PRICE_SPRITE_TAG);
    pItemNode->addChild(pPriceSprite, 200);
}

void LockHelper::addIAPLockSprite(WJBase* pItemBase, float scaleFactor)
{
	Node* pItemNode = pItemBase->nodeInstance();
    float itemScale = pItemNode->getScale();
	
	WJSprite* pLockSprite = WJSprite::create(IAP_LOCK_FILE);
	pLockSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pLockSprite->setScale(0.8f * scaleFactor);
    Size itemSize = pItemNode->getContentSize();
    Size lockSpriteSize = pLockSprite->getContentScaleSize();
	pLockSprite->setPosition(Vec2(itemSize.width - 30.0f, 34.0f));
	pLockSprite->setTag(IAP_SPRITE_TAG);
	pItemNode->addChild(pLockSprite, 200);
}

void LockHelper::addGetfreeSprite(WJBase* pItemBase, float scaleFactor)
{
	Node* pItemNode = pItemBase->nodeInstance();
    float itemScale = pItemNode->getScale();

	WJSprite* pGetfreeSprite = WJSprite::create(GETFREE_FILE);
	pGetfreeSprite->ignoreAnchorPointForPosition(false);
	pGetfreeSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pGetfreeSprite->setScale(0.9f * scaleFactor);
    Size itemSize = pItemNode->getContentSize();

	pGetfreeSprite->setPosition(Vec2(itemSize.width - 47, 30.0f));
	pGetfreeSprite->setTag(GETFREE_SPRITE_TAG);
	pItemNode->addChild(pGetfreeSprite, 200);
}

void LockHelper::playLookGetfreeVideoAfterAni(Node* pItemNode)
{
	Node* pGetfreeNode = pItemNode->getChildByTag(GETFREE_SPRITE_TAG);
	if (pGetfreeNode)
	{
		pItemNode->runAction(Sequence::create(
			ScaleTo::create(0.15f, pItemNode->getScale() * 1.15f),
			ScaleTo::create(0.15f, pItemNode->getScale() * 1.0f),
			ScaleTo::create(0.15f, pItemNode->getScale() * 1.1f),
			ScaleTo::create(0.15f, pItemNode->getScale() * 1.0f),
			CCCallFunc::create([=]() {
				// 播放音效
				Common::sound.play("Common:0071");
				// 播放粒子效果
				ParticleSystemQuad *particle = ParticleSystemQuad::create("particles/unlock.plist");
				particle->setPosition(pItemNode->getPosition());
				pItemNode->getParent()->addChild(particle, pItemNode->getLocalZOrder() + 1);
			}),
			CCCallFunc::create([=]() {
				Node* getfreeNode = pItemNode->getChildByTag(GETFREE_SPRITE_TAG);
				if (getfreeNode)
				{
					getfreeNode->runAction(Sequence::create(
						MoveBy::create(0.1f, Vec2(0, 30)),
						Spawn::create(
						MoveBy::create(0.3f, Vec2(50, -60)),
						FadeOut::create(0.3f),
						NULL),
						CCCallFunc::create([=](){
							Common::sound.play("Common:0072");
							// 播放粒子效果
							ParticleSystemQuad *particle = ParticleSystemQuad::create("particles/unlockend.plist");
							particle->setPosition(pItemNode->getPosition());
							pItemNode->getParent()->addChild(particle, pItemNode->getLocalZOrder() + 1);
					}),
						Hide::create(),
						RemoveSelf::create(),
					NULL));
				}
			}),
			NULL));
	}
}

void LockHelper::showUseStarDialog(Node* pNode, WJTouchEvent* e, const char *key, bool bCallback, WJTouchCallback callback)
{
    TipDialog* pTipDialog = TipDialog::create("game/json/usestar.json");

	LockItemClickInfo* pInfo = new LockItemClickInfo();
	pInfo->m_node = pNode;
	pInfo->cloneTouchEvent(e);
	pInfo->m_itemKey = key;
	pInfo->m_bCallback = bCallback;
	pInfo->m_callback = callback;

	pTipDialog->setUserData(pInfo);
    pTipDialog->setOnOkCallBack(CC_CALLBACK_1(LockHelper::onUseStarDialogOKClick, this));
	pTipDialog->setOnCloseCallBack(CC_CALLBACK_1(LockHelper::onUseStarDialogCancelClick, this));
    
	Common::sound.play("Common:popup");
    LockHelper::updateMenuPositionInUseStarDialog(pNode, pTipDialog);
	Director::getInstance()->getRunningScene()->addChild(pTipDialog, 9999);
    pTipDialog->show(LBModalDialogAnimationType::ScaleEaseBack);
}

void LockHelper::onUseStarDialogOKClick(TipDialog *pTipDialog)
{
	LockItemClickInfo* pInfo = reinterpret_cast<LockItemClickInfo*>(pTipDialog->getUserData());
    
    if (LockHelper::purchaseItemByStar(pInfo->m_itemKey.c_str()))
    {
        Node* pNode = pInfo->m_node;
		WJBase* pItemBase = dynamic_cast<WJBase*>(pNode);
		pItemBase->setUserString(LOCK_AVAILABLE_STR);

        Node* pPriceSprite = pNode->getChildByTag(PRICE_SPRITE_TAG);
        if (pPriceSprite)
            pPriceSprite->setVisible(false);

        Node* pPriceLabel = pNode->getChildByTag(PRICE_LABEL_TAG);
        if (pPriceLabel)
            pPriceLabel->setVisible(false);

		/* 回调 */
		if (pInfo->m_bCallback && pInfo->m_callback)
			(pInfo->m_callback)(pNode, pInfo->m_event);
    }
    else
    {
       showStarTipDialog(WJUtils::startWith(pInfo->m_itemKey, "P001"));
    }

	CC_SAFE_DELETE(pInfo);
}

bool LockHelper::purchaseItemByStar(const char * key)
{
	ScrollItemTemplate object = ScrollItemTemplateTem(key);
	if (object.STARS <= 0)
		return true;

	if (Star::getTotalStar() < object.STARS)
		return false;

	Star::subStar(object.STARS);
	LockHelper::saveItemPurchased(key);
	return true;
}

void LockHelper::onUseStarDialogCancelClick(TipDialog* pTipDialog)
{
	LockItemClickInfo* pInfo = reinterpret_cast<LockItemClickInfo*>(pTipDialog->getUserObject());
	CC_SAFE_DELETE(pInfo);
}

void LockHelper::updateMenuPositionInUseStarDialog(Node* pNode, TipDialog* pTipDialog)
{
    WJBase* pItemBase = dynamic_cast<WJBase*>(pNode);
    Vec2 itemPosWorld = pItemBase->getPositionWorld();
    
    const Size& winSize = Director::getInstance()->getWinSize();
    bool xRight = itemPosWorld.x < winSize.width * 0.5f;
    bool yUp = itemPosWorld.y < winSize.height * 0.5f;
    
    float diffX = itemPosWorld.x - winSize.width * 0.5f;
    float diffY = itemPosWorld.y - winSize.height * 0.5f;
    
    WJSprite* pBgSprite = pTipDialog->getLayerJson()->getSubSpriteByKey("bg");
    WJButton* pNoButton = pTipDialog->getLayerJson()->getSubButtonByKey("no");
    WJButton* pYesButton = pTipDialog->getLayerJson()->getSubButtonByKey("yes");
    
    Vec2 noOffset = pNoButton->getPosition() - pBgSprite->getPosition();
    Vec2 yesOffset = pYesButton->getPosition() - pBgSprite->getPosition();
    
    float bgX = 682 + diffX + (xRight ? 250.0f : -250.0f);
    float bgY = 384 + diffY + (yUp ? 100.0f : -100.0f);
    
    pBgSprite->setPosition(Vec2(bgX, bgY));
    pNoButton->setPosition(pBgSprite->getPosition() + noOffset);
    pYesButton->setPosition(pBgSprite->getPosition() + yesOffset);
}

bool LockHelper::onLockItemClick(Node* pNode, WJTouchEvent* e, const char * key, bool bCallback, WJTouchCallback callback)
{
	WJBase* pItemBase = dynamic_cast<WJBase*>(pNode);
	ScrollItemTemplate object = ScrollItemTemplateTem(key);
	if (pItemBase->getUserString() == LOCK_AVAILABLE_STR)
		return true;

	if (!WJUtils::canClick("LOCK_ITEM", 550))
		return false;

	if (pItemBase->getUserString() == LOCK_PRICE_STR)
	{
		int price = object.STARS;
		if (Star::getTotalStar() < price)
		{
			/* 金币不足的情况下直接弹出金币提示弹窗 */
			showStarTipDialog(WJUtils::startWith(key, "P001"));
		}
		else
		{
			showUseStarDialog(pNode, e, key, bCallback, callback);
		}
		return false;
	}
	else if (pItemBase->getUserString() == LOCK_IAP_STR)
	{
		const char* iapKey = LockHelper::getIAPKeyByLockType((IAPLockType)object.LockType);
		Store::showStoreMini(iapKey);

		return false;
	}
	else if (pItemBase->getUserString() == LOCK_GETFREE_STR)
	{	
		showGetfreeVideo(key);

		return false;
	}
	
	return true;
}

void LockHelper::showStarTipDialog(bool dressUp)
{
	TipDialog* pDialog = TipDialog::create("game/json/nostar.json");

	WJSprite* pCancelSprite = pDialog->getLayerJson()->getSubSpriteByKey("cancel");
	pCancelSprite->setVisible(false);
	if (!dressUp)
	{
		/* 隐藏yes按钮 */
		pDialog->getLayerJson()->getSubSpriteByKey("yes")->setVisible(false);

		/* 调整no按钮位置和图片 */
		WJButton* pNoButton = pDialog->getLayerJson()->getSubButton("no");
		pNoButton->setPosition(pCancelSprite->getPosition());
		pNoButton->keepContentSize(false);
		pNoButton->setNormalFileName(pCancelSprite->getSpriteFileName());
		pNoButton->loadSpriteTexture(pCancelSprite->getSpriteFileName());

		/* 调整显示的提示文本 */
		pDialog->getLayerJson()->getSubSpriteByKey("words")->setVisible(false);
		pDialog->getLayerJson()->getSubSpriteByKey("wordp001")->setVisible(true);
	}

	Common::sound.play("Common:popup");
	pDialog->setOnOkCallBack(CC_CALLBACK_1(LockHelper::onStarTipDialogOkClick, this));
	Director::getInstance()->getRunningScene()->addChild(pDialog, 9999);
	pDialog->show();
}

void LockHelper::showGetfreeVideo(const char* id)
{
	if (WJUtils::canClick(id, 3000))
	{
		WJUtils::playGetFreeRewardVideo(id);
	}
}

void LockHelper::onStarTipDialogOkClick(TipDialog* pDialog)
{
	/*std::vector<GameNumber> gameList;
	gameList.push_back(GameNumber::P002_Puzzle_Choose);
	gameList.push_back(GameNumber::P003);
	gameList.push_back(GameNumber::P004);
	gameList.push_back(GameNumber::P006_Choose);
	if (Common::isInAppPurchasedByKey(IAP_HAIR_PACK_KEY, true))
	{
		gameList.push_back(GameNumber::P008_Hair);
	}
	else if (Common::isInAppPurchasedByKey(IAP_TREAT_PACK_KEY, true))
	{
		gameList.push_back(GameNumber::P009_HealthCare);
	}
	else if (Common::isInAppPurchasedByKey(IAP_COOK_PACK_KEY, true))
	{
		gameList.push_back(GameNumber::P010_Menu);
	}

	int randomInt = WJUtils::randomInt(gameList.size());
	Loading::gotoScene(gameList[randomInt]);*/
}