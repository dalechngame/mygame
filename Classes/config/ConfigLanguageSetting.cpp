#include "ConfigLanguageSetting.h"
#include "cocos2d.h"

ConfigLanguageSetting* ConfigLanguageSetting::s_pInstance = NULL;

ConfigLanguageSetting::ConfigLanguageSetting()
{
	
}

ConfigLanguageSetting::~ConfigLanguageSetting()
{

}

ConfigLanguageSetting& ConfigLanguageSetting::instance()
{
	if(!s_pInstance)
	{
		s_pInstance = new ConfigLanguageSetting();
		s_pInstance->queryUsedLanguage();
	}
	return (*s_pInstance);
}

void ConfigLanguageSetting::release()
{
	if(s_pInstance)
	{
		delete s_pInstance;
		s_pInstance = NULL;
	}
}

cocos2d::LanguageType ConfigLanguageSetting::queryUsedLanguage(void)
{
	cocos2d::LanguageType	type;
	int						result;
	result = cocos2d::UserDefault::getInstance()->getIntegerForKey("LanguageType",-1);
	if(result == -1)
	{
		type = cocos2d::Application::getInstance()->getCurrentLanguage();
	}
	else
	{
		type = (cocos2d::LanguageType)result;
	}
	m_languageType = type;
	m_languageName = calcLanguageName(type);
	return type;
}

std::string ConfigLanguageSetting::calcLanguageName(cocos2d::LanguageType type)
{
	std::string result;
	result = "Default";
	switch(type)
	{
		case cocos2d::LanguageType::ENGLISH:
			result = "English";
			break;
		case cocos2d::LanguageType::CHINESE:
			result = "Chinese";
			break;
		case cocos2d::LanguageType::FRENCH:
			result = "French";
			break;
		case cocos2d::LanguageType::ITALIAN:
			result = "Italian";
			break;
		case cocos2d::LanguageType::GERMAN:
			result = "German";
			break;
		case cocos2d::LanguageType::SPANISH:
			result = "Spanish";
			break;
		case cocos2d::LanguageType::RUSSIAN:
			result = "Russian";
			break;
		case cocos2d::LanguageType::KOREAN:
			result = "Korean";
			break;
		case cocos2d::LanguageType::JAPANESE:
			result = "Japanese";
			break;
		case cocos2d::LanguageType::HUNGARIAN:
			result = "Hungarian";
			break;
		case cocos2d::LanguageType::PORTUGUESE:
			result = "Portuguese";
			break;
		case cocos2d::LanguageType::ARABIC:
			result = "Arabic";
			break;
	}
	return result;
}

cocos2d::LanguageType ConfigLanguageSetting::getCurLanguage(void)
{
	return m_languageType;
}

std::string ConfigLanguageSetting::getCurLanguageName(void)
{
	return m_languageName;
}

void ConfigLanguageSetting::setCurLanguage(cocos2d::LanguageType type)
{
	cocos2d::UserDefault::getInstance()->setIntegerForKey("LanguageType",(int)type);
	cocos2d::UserDefault::getInstance()->flush();	
	m_languageType = type;
	m_languageName = calcLanguageName(type);		
}
