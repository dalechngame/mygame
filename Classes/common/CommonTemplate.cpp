#include "CommonTemplate.h"

void CommonTemplate::InitData(void)
{
	isInstance = lastLoadLanguage != ConfigLanguageSetting::instance().getCurLanguageName() ? false : isInstance;

	if (isInstance)
		return;

	Release();
	lastLoadLanguage = ConfigLanguageSetting::instance().getCurLanguageName();
	isInstance = true;
	CommonTemplate* temp;

	// --- 如果游戏没有中文版，请将中文这一大段代码注释或删除，避免潜在的Bug发生 ---
	/*if (lastLoadLanguage == "Chinese")
	{
		// --------------------------------- iOS (zh-cn) ----------------------------------------
		temp = new CommonTemplate();
		temp->Key = "iOS:THIS_APP_ID";
		temp->Value = "debug_appid";				// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));
		
		temp = new CommonTemplate();
		temp->Key = "iOS:THIS_APP_STORE_URL";
		temp->Value = "http://xxxxxxxxxx";			// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));
		
		temp = new CommonTemplate();
		temp->Key = "iOS:THIS_APP_STORE_URL_LONG";
		temp->Value = "https://xxxxxxxxxxx";		// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "iOS:GOOGLE_ADS_ID";
		temp->Value = "ca-app-pub-8236190732156944/1016996119";		// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "iOS:GOOGLE_ADS_ID_INTERSTITIAL";
		temp->Value = "none";										// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "iOS:EMAIL_FEEDBACK";
		temp->Value = "contact@libii.com|Feedback about Winter Party! V";
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "iOS:EMAIL_SHARE";
		temp->Value = "|Winter Party!|<html><body>找到一个很好玩的女孩游戏，赶快下载下来玩玩吧！<br><br>这是下载地址: <a href='%s'>%s</a><br><br><a href='%s'><img src='data:image/png;base64,%%@'/></a></body></html>|";
		mapdata.insert(std::make_pair(temp->Key, temp));

		// --------------------------------- Amazon (zh-cn) ----------------------------------------

		temp = new CommonTemplate();
		temp->Key = "Amazon:THIS_APP_ID";
		temp->Value = "debug_appid";						// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Amazon:THIS_APP_STORE_URL";
		temp->Value = "amzn://apps/android?p=com.libii.winterparty";
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Amazon:THIS_APP_STORE_URL_LONG";
		temp->Value = "http://www.amazon.com/gp/mas/dl/android?p=com.libii.winterparty&showAll=0";
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Amazon:GOOGLE_ADS_ID";
		temp->Value = "xxxxxxxxxxxxx";						// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Amazon:GOOGLE_ADS_ID_INTERSTITIAL";
		temp->Value = "xxxxxxxxxxxxx";						// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Amazon:EMAIL_FEEDBACK";
		temp->Value = "contact@libii.com|Feedback about Winter Party!(Kindle Fire Edition) V";
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Amazon:EMAIL_SHARE";
		temp->Value = "|Winter Party!|找到一个很好玩的女孩游戏，赶快下载下来玩玩吧！\n\n这是下载地址： %s|";
		mapdata.insert(std::make_pair(temp->Key, temp));

		// --------------------------------- Google Play (zh-cn) ----------------------------------------

		temp = new CommonTemplate();
		temp->Key = "Google:THIS_APP_ID";
		temp->Value = "debug_appid";						// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Google:THIS_APP_STORE_URL";
		temp->Value = "market://details?id=com.libii.winterparty";
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Google:THIS_APP_STORE_URL_LONG";
		temp->Value = "https://play.google.com/store/apps/details?id=com.libii.winterparty";
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Google:GOOGLE_ADS_ID";
		temp->Value = "xxxxxxxxxxxxx";						// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Google:GOOGLE_ADS_ID_INTERSTITIAL";
		temp->Value = "xxxxxxxxxxxxx";						// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Google:EMAIL_FEEDBACK";
		temp->Value = "contact@libii.com|Feedback about Winter Party!(Android) V";
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Google:EMAIL_SHARE";
		temp->Value = "|Winter Party!|找到一个很好玩的女孩游戏，赶快下载下来玩玩吧！\n\n这是下载地址： %s|";
		mapdata.insert(std::make_pair(temp->Key, temp));

	}
	else */
	{
		// --------------------------------- iOS (default) ----------------------------------------
		temp = new CommonTemplate();
		temp->Key = "iOS:THIS_APP_ID";
		temp->Value = "debug_appid";						// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "iOS:THIS_APP_STORE_URL";
		temp->Value = "http://xxxxxxxxxx";					// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "iOS:THIS_APP_STORE_URL_LONG";
		temp->Value = "https://xxxxxxxxxxx";				// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "iOS:GOOGLE_ADS_ID";
		temp->Value = "ca-app-pub-8236190732156944/1016996119";		// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "iOS:GOOGLE_ADS_ID_INTERSTITIAL";
		temp->Value = "none";										// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "iOS:EMAIL_FEEDBACK";
		temp->Value = "contact@libii.com|Feedback about Winter Party! V";
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "iOS:EMAIL_SHARE";
		temp->Value = "|Winter Party!|<html><body>Look the best App for you! <br><br> It's the most fun Girls game.<br><br>Here is a link to Winter Party in the App Store: <a href='%s'>%s</a><br><br><a href='%s'><img src='data:image/png;base64,%%@'/></a></body></html>|";
		mapdata.insert(std::make_pair(temp->Key, temp));

		// --------------------------------- Amazon (default) ----------------------------------------

		temp = new CommonTemplate();
		temp->Key = "Amazon:THIS_APP_ID";
		temp->Value = "debug_appid";								// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Amazon:THIS_APP_STORE_URL";
		temp->Value = "amzn://apps/android?p=com.libii.winterparty";
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Amazon:THIS_APP_STORE_URL_LONG";
		temp->Value = "http://www.amazon.com/gp/mas/dl/android?p=com.libii.winterparty&showAll=0";
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Amazon:GOOGLE_ADS_ID";
		temp->Value = "xxxxxxxxxxxxx";								// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Amazon:GOOGLE_ADS_ID_INTERSTITIAL";
		temp->Value = "xxxxxxxxxxxxx";								// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Amazon:EMAIL_FEEDBACK";
		temp->Value = "contact@libii.com|Feedback about Winter Party!(Kindle Fire Edition) V";
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Amazon:EMAIL_SHARE";
		temp->Value = "|Winter Party!|Look at the best app for you!\n\nIt's the most fun Girls game.\n\nHere is a link to Winter Party in the App Store: %s|";
		mapdata.insert(std::make_pair(temp->Key, temp));

		// --------------------------------- GooglePlay (default) ----------------------------------------

		temp = new CommonTemplate();
		temp->Key = "Google:THIS_APP_ID";
		temp->Value = "debug_appid";									// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Google:THIS_APP_STORE_URL";
		temp->Value = "market://details?id=com.libii.winterparty";
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Google:THIS_APP_STORE_URL_LONG";
		temp->Value = "https://play.google.com/store/apps/details?id=com.libii.winterparty";
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Google:GOOGLE_ADS_ID";
		temp->Value = "xxxxxxxxxxxxx";									// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Google:GOOGLE_ADS_ID_INTERSTITIAL";
		temp->Value = "xxxxxxxxxxxxx";									// TODO: fix me
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Google:EMAIL_FEEDBACK";
		temp->Value = "contact@libii.com|Feedback about Winter Party!(Android) V";
		mapdata.insert(std::make_pair(temp->Key, temp));

		temp = new CommonTemplate();
		temp->Key = "Google:EMAIL_SHARE";
		temp->Value = "|Winter Party!|Look at the best app for you!\n\nIt's the most fun Girls game.\n\nHere is a link to Winter Party in the App Store: %s|";
		mapdata.insert(std::make_pair(temp->Key, temp));

	}

}