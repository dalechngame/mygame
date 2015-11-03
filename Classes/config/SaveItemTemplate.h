/********************************************************************
author:	ConfigGenerator
*********************************************************************/

#ifndef __SaveItemTemplate__HPP__NO_CONFLICT
#define __SaveItemTemplate__HPP__NO_CONFLICT

#include "ConfigDataTemplate.hpp"
#include "ConfigLanguageSetting.h"

class SaveItemTemplate : public ConfigDataTemplate<SaveItemTemplate>
{
public:
	static void InitData(void);
public:
	std::string Key;

	std::string savePreKey;
int count;
bool onlyPicture;
float scale;

};

#define SaveItemTemplateDic				SaveItemTemplate::Dic()
#define SaveItemTemplateLis(contextKey)	SaveItemTemplate::Lis(contextKey)
#define SaveItemTemplateTem(key)			SaveItemTemplate::Tem(key)
#define SaveItemTemplateRelease			SaveItemTemplate::Release();

#endif