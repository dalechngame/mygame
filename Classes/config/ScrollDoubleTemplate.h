/********************************************************************
author:	ConfigGenerator
*********************************************************************/

#ifndef __ScrollDoubleTemplate__HPP__NO_CONFLICT
#define __ScrollDoubleTemplate__HPP__NO_CONFLICT

#include "ConfigDataTemplate.hpp"
#include "ConfigLanguageSetting.h"

class ScrollDoubleTemplate : public ConfigDataTemplate<ScrollDoubleTemplate>
{
public:
	static void InitData(void);
public:
	std::string Key;

	std::string itemKey;
bool clickAble;
int count;
int saveType;
float saveIconScale;
float saveFileScale;

};

#define ScrollDoubleTemplateDic				ScrollDoubleTemplate::Dic()
#define ScrollDoubleTemplateLis(contextKey)	ScrollDoubleTemplate::Lis(contextKey)
#define ScrollDoubleTemplateTem(key)			ScrollDoubleTemplate::Tem(key)
#define ScrollDoubleTemplateRelease			ScrollDoubleTemplate::Release();

#endif