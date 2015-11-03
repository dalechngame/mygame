/********************************************************************
author:	ConfigGenerator
*********************************************************************/

#ifndef __ModelTemplate__HPP__NO_CONFLICT
#define __ModelTemplate__HPP__NO_CONFLICT

#include "ConfigDataTemplate.hpp"
#include "ConfigLanguageSetting.h"

class ModelTemplate : public ConfigDataTemplate<ModelTemplate>
{
public:
	static void InitData(void);
public:
	std::string Key;

	int no;
int type;
bool animal;
float npcPosY;
std::string norJsonFile;
std::string norAtlasFile;
std::string skin;

};

#define ModelTemplateDic				ModelTemplate::Dic()
#define ModelTemplateLis(contextKey)	ModelTemplate::Lis(contextKey)
#define ModelTemplateTem(key)			ModelTemplate::Tem(key)
#define ModelTemplateRelease			ModelTemplate::Release();

#endif