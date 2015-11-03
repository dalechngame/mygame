/********************************************************************
author:	ConfigGenerator
*********************************************************************/

#ifndef __CommonTemplate__HPP__NO_CONFLICT
#define __CommonTemplate__HPP__NO_CONFLICT

#include "ConfigDataTemplate.hpp"
#include "ConfigLanguageSetting.h"

class CommonTemplate : public ConfigDataTemplate<CommonTemplate>
{
public:
	static void InitData(void);
public:
	std::string Key;

	std::string Value;

};

#define CommonTemplateDic				CommonTemplate::Dic()
#define CommonTemplateLis(contextKey)	CommonTemplate::Lis(contextKey)
#define CommonTemplateTem(key)			CommonTemplate::Tem(key)
#define CommonTemplateRelease			CommonTemplate::Release();

#endif