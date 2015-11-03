/********************************************************************
author:	ConfigGenerator
*********************************************************************/

#ifndef __CrownTemplate__HPP__NO_CONFLICT
#define __CrownTemplate__HPP__NO_CONFLICT

#include "ConfigDataTemplate.hpp"
#include "ConfigLanguageSetting.h"

class CrownTemplate : public ConfigDataTemplate<CrownTemplate>
{
public:
	static void InitData(void);
public:
	std::string Key;

	std::string firstSlotName;
std::string firstFile;
std::string SecondSlotName;
std::string secondFile;

};

#define CrownTemplateDic				CrownTemplate::Dic()
#define CrownTemplateLis(contextKey)	CrownTemplate::Lis(contextKey)
#define CrownTemplateTem(key)			CrownTemplate::Tem(key)
#define CrownTemplateRelease			CrownTemplate::Release();

#endif