// Created by wangwenjie on 2013/04

#include "Common.h"

const bool DEBUG_NO_IAP				= true;	// false or true 是否去掉IAP购买，即全解锁的状态 (默认false)
const bool DEBUG_NO_ADS				= false;	// false or true 是否去掉广告 (默认false)

const bool DEBUG_APP				= true;		// false or true 是否处于调试状态 (默认false)

float  TO_SCALE_WIDTH   = 1.0f;
float  TO_SCALE_HEIGHT	= 1.0f;

LBAudioControllerT<SoundTemplate> Common::sound;
int Common::s_bgMusicIndex = -1;

ValueMap* Common::s_lockDict = NULL;

std::map<GameNumber, std::string> Common::s_gameNamesMap;


void Common::loadCommonEffect()
{
    sound.preload("Common");
}

bool Common::isInAppPurchasedByKey( const char* lockKey, bool checkAll)
{
	return isInAppPurchased(getLockIapId(lockKey).c_str(), checkAll);
}

bool Common::isInAppPurchased( const char* iapId, bool checkAll )
{
    // check noad iap
	if (WJUtils::equals(getUnlockNoAdsIapId(), iapId) && WJUtils::isAdsRemoved())
		return true;

    // check unlockall iap
    if (checkAll)
		return WJUtils::isInAppPurchased(iapId, getUnlockAllIapId().c_str());
    
    return WJUtils::isInAppPurchased(iapId, NULL);
}


bool Common::isGetFreeUnlocked( const char* getFreeLockKey )
{
    return isInAppPurchased(getLockIapId(getFreeLockKey).c_str(), true)
            || WJUtils::isGetFreeUnlocked(getFreeLockKey);
}

void Common::showAds()
{
    if (WJUtils::isAdsRemoved())
        return;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    const std::string& deviceType = WJUtils::getDeviceType();
    if (WJUtils::startWith(deviceType, "iPad"))
		WJUtils::showAds(GOOGLE_ADS_ID, ADSIZE_IAB_LEADERBOARD, ADPOS_X_CENTER, ADPOS_Y_BOTTOM, ADWH_AUTO, ADWH_AUTO, wjalNone);
    else
		WJUtils::showAds(GOOGLE_ADS_ID, ADSIZE_BANNER, ADPOS_X_CENTER, ADPOS_Y_BOTTOM, ADWH_AUTO, ADWH_AUTO, wjalNone);

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	WJUtils::showAds(GOOGLE_ADS_ID , ADSIZE_BANNER_AUTO, ADPOS_X_CENTER, ADPOS_Y_BOTTOM, ADWH_AUTO, ADWH_AUTO, wjalNone, Vec2::ZERO);

#endif
}

cocos2d::Size Common::getAdsSize()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    const std::string& deviceType = WJUtils::getDeviceType();
    if (WJUtils::startWith(deviceType, "iPad"))
        return WJUtils::getAdsSize(ADSIZE_IAB_LEADERBOARD);
    else
        return WJUtils::getAdsSize(ADSIZE_BANNER);

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    return WJUtils::getAdsSize(ADSIZE_BANNER_AUTO);

#endif
}

bool Common::isAdsRemoved()
{
    return WJUtils::isAdsRemoved();
}

void Common::showChartBoost(WJChartboostLocation location, bool forceCount, bool forceShow)
{
	WJUtils::showChartboost(1, forceCount, forceShow, location);
}

void Common::showChartBoostOnPlayButton()
{
	WJUtils::showChartboostOnPlayButton(true, true, 1);
}


ValueVector Common::getProductInfoArray()
{
	// read file every time
	std::string newIapPlistFile = WJUtils::getCachesFolderPath(IAP_PLIST_FOLDER);
	newIapPlistFile.append("/").append(IAP_PLIST_FILENAME);
	if (WJUtils::fileExists(newIapPlistFile.c_str()))
	{
		// read new iap file
		return FileUtils::getInstance()->getValueVectorFromFile(newIapPlistFile.c_str());
	}

	// read old iap file
	std::string oldIapPlistFile = IAP_PLIST_FOLDER;
	oldIapPlistFile.append("/").append(IAP_PLIST_FILENAME);

	return FileUtils::getInstance()->getValueVectorFromFile(oldIapPlistFile.c_str());
}

ValueMap* Common::getLockDict()
{
    if (s_lockDict == NULL)
		s_lockDict = new ValueMap(FileUtils::getInstance()->getValueMapFromFile("store/lock.plist"));
    
	return s_lockDict;
}

int Common::getLockStartNo( const char* key )
{
    if (key)
    {
        auto it = getLockDict()->find(key);
		if (it != getLockDict()->end())
			return it->second.asValueMap()["lock"].asInt();
    }
    return 99999;
}

std::string Common::getLockIapId( const char* key )
{
	auto it = getLockDict()->find(key);
	if (it != getLockDict()->end())
        return it->second.asValueMap()["productId"].asString();

    return "";
}


std::string Common::getUnlockAllIapId()
{
	return getLockIapId(IAP_UNLOCKALL_KEY);
}

std::string Common::getUnlockNoAdsIapId()
{
	return getLockIapId(IAP_UNLOCKNOADS_KEY);
}

std::string Common::getLockOtherString(const char *key, const char *otherKey)
{
	auto it = getLockDict()->find(key);
	if (it != getLockDict()->end())
		return it->second.asValueMap()[otherKey].asString();

	return "";
}

std::string Common::getGameName( GameNumber gameNumber )
{
	static bool inited = false;
	
	if (!inited)
	{
		inited = true;

		s_gameNamesMap.insert(make_pair(GameNumber::P001, "P001Welcome"));
		s_gameNamesMap.insert(make_pair(GameNumber::P002, "P002SelectPet"));
		s_gameNamesMap.insert(make_pair(GameNumber::P003, "P003Box2d"));
		s_gameNamesMap.insert(make_pair(GameNumber::P005, "P005Room"));
		s_gameNamesMap.insert(make_pair(GameNumber::P007_MakeUp, "P007Makeup"));
		s_gameNamesMap.insert(make_pair(GameNumber::P008, "P008Nail"));
		s_gameNamesMap.insert(make_pair(GameNumber::P009, "P009Line"));
		s_gameNamesMap.insert(make_pair(GameNumber::P011_Choose, "P011Choose"));
		s_gameNamesMap.insert(make_pair(GameNumber::P011_Draw, "P011Draw"));
		s_gameNamesMap.insert(make_pair(GameNumber::P012, "P012cakepop"));
		s_gameNamesMap.insert(make_pair(GameNumber::P012_show, "P012cakeshow"));
		s_gameNamesMap.insert(make_pair(GameNumber::P013, "P013Photo"));
		s_gameNamesMap.insert(make_pair(GameNumber::P014_Main, "P014Main"));
	}

	auto it = s_gameNamesMap.find(gameNumber);
	if (it == s_gameNamesMap.end())
	{
		return WJUtils::stringAddInt("P", (int)gameNumber, 3);   // default PXXX
	}

	return (*it).second;
}

GameNumber Common::getGameNumber( const std::string& gameName )
{
	getGameName(GameNumber::P001);  // init s_gameNamesMap

	auto it = std::find_if(s_gameNamesMap.begin(), s_gameNamesMap.end(), 
		[&](std::map<GameNumber, std::string>::value_type value) -> bool
		{
			return value.second == gameName;
		});

	if (it != s_gameNamesMap.end())
		return (*it).first;

	// parse game name string
	int n = atoi(gameName.c_str() + 1);
	return (GameNumber)n;
}

// ------------------------ PLoader -----------------------

std::map<GameNumber, PLoader*>* PLoader::s_loaderMap = nullptr;

PLoader::PLoader( GameNumber gameNumber, SEL_CreateSceneNode createSceneSelector, SEL_AsyncLoadSceneResource asyncLoadSceneSelector )
{
	m_gameNumber = gameNumber;
	m_createSceneSelector = createSceneSelector;
	m_asyncLoadSceneSelector = asyncLoadSceneSelector;

    if (!s_loaderMap)
        s_loaderMap = new std::map<GameNumber, PLoader*>();
    
	s_loaderMap->insert(make_pair(gameNumber, this));
}

GameNumber PLoader::getGameNumber()
{
	return m_gameNumber;
}

SEL_CreateSceneNode PLoader::getCreateSceneSelector()
{
	return m_createSceneSelector;
}

SEL_AsyncLoadSceneResource PLoader::getAsyncLoadSceneSelector()
{
	return m_asyncLoadSceneSelector;
}

PLoader* PLoader::getLoader( GameNumber gameNumber )
{
	auto it = s_loaderMap->find(gameNumber);
	if (it == s_loaderMap->end())
		return NULL;

	return it->second;
}
