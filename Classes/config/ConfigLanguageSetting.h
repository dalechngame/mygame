/********************************************************************
	created:	2014/08/22
	created:	22:8:2014   15:27
	author:		lic
*********************************************************************/
#ifndef __CONFIGLANGUAGESETTING__H__
#define __CONFIGLANGUAGESETTING__H__
#include <string> 
#include <map> 
#include <vector>
#include "CCApplication.h"

class ConfigLanguageSetting
{
	private:
		  ConfigLanguageSetting();
		 ~ConfigLanguageSetting();

	public:
		static ConfigLanguageSetting&	instance	(void);
		static void						release		(void);
	
	public:
		cocos2d::LanguageType getCurLanguage		(void);
		std::string				getCurLanguageName	(void);
		void					setCurLanguage		(cocos2d::LanguageType type);
	
	protected:
		cocos2d::LanguageType	queryUsedLanguage	(void);
		std::string				calcLanguageName	(cocos2d::LanguageType type);

	private:
		static ConfigLanguageSetting*	s_pInstance;
		std::string						m_languageName;	
		cocos2d::LanguageType			m_languageType;				
};
#endif