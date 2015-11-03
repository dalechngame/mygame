/********************************************************************
author:	ConfigGenerator
*********************************************************************/

#ifndef __ClothTemplate__HPP__NO_CONFLICT
#define __ClothTemplate__HPP__NO_CONFLICT

#include "ConfigDataTemplate.hpp"
#include "ConfigLanguageSetting.h"

class ClothTemplate : public ConfigDataTemplate<ClothTemplate>
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
std::string slot5Name;
std::string slot5File;
std::string slot6Name;
std::string slot6File;

};

#define ClothTemplateDic				ClothTemplate::Dic()
#define ClothTemplateLis(contextKey)	ClothTemplate::Lis(contextKey)
#define ClothTemplateTem(key)			ClothTemplate::Tem(key)
#define ClothTemplateRelease			ClothTemplate::Release();

#endif