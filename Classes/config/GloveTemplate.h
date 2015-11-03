/********************************************************************
author:	ConfigGenerator
*********************************************************************/

#ifndef __GloveTemplate__HPP__NO_CONFLICT
#define __GloveTemplate__HPP__NO_CONFLICT

#include "ConfigDataTemplate.hpp"
#include "ConfigLanguageSetting.h"

class GloveTemplate : public ConfigDataTemplate<GloveTemplate>
{
public:
	static void InitData(void);
public:
	std::string Key;

	std::string filepath;
std::string slot1Name;
std::string slot1File;
std::string slot2Name;
std::string slot2File;
std::string slot3Name;
std::string slot3File;
std::string slot4Name;
std::string slot4File;

};

#define GloveTemplateDic				GloveTemplate::Dic()
#define GloveTemplateLis(contextKey)	GloveTemplate::Lis(contextKey)
#define GloveTemplateTem(key)			GloveTemplate::Tem(key)
#define GloveTemplateRelease			GloveTemplate::Release();

#endif