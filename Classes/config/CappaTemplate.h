/********************************************************************
author:	ConfigGenerator
*********************************************************************/

#ifndef __CappaTemplate__HPP__NO_CONFLICT
#define __CappaTemplate__HPP__NO_CONFLICT

#include "ConfigDataTemplate.hpp"
#include "ConfigLanguageSetting.h"

class CappaTemplate : public ConfigDataTemplate<CappaTemplate>
{
public:
	static void InitData(void);
public:
	std::string Key;

	std::string leftSlotName;
std::string leftFile;
std::string rightSlotName;
std::string rightFile;
std::string specilSlotName;
std::string specilFile;

};

#define CappaTemplateDic				CappaTemplate::Dic()
#define CappaTemplateLis(contextKey)	CappaTemplate::Lis(contextKey)
#define CappaTemplateTem(key)			CappaTemplate::Tem(key)
#define CappaTemplateRelease			CappaTemplate::Release();

#endif