/********************************************************************
author:	ConfigGenerator
*********************************************************************/

#ifndef __ScrollItemTemplate__HPP__NO_CONFLICT
#define __ScrollItemTemplate__HPP__NO_CONFLICT

#include "ConfigDataTemplate.hpp"
#include "ConfigLanguageSetting.h"

class ScrollItemTemplate : public ConfigDataTemplate<ScrollItemTemplate>
{
public:
	static void InitData(void);
public:
	std::string Key;

	float itemBgScale;
float itemsInterval;
std::string itemIconFile;
float itemIconScale;
bool endFlag;
bool cancleFlag;
bool moveAble;
int dragType;
std::string itemFile;
float itemScale;
float itemRotate;
float offsetX;
float offsetY;
int LockType;
bool TRYFREE;
bool LOCK;
int STARS;

};

#define ScrollItemTemplateDic				ScrollItemTemplate::Dic()
#define ScrollItemTemplateLis(contextKey)	ScrollItemTemplate::Lis(contextKey)
#define ScrollItemTemplateTem(key)			ScrollItemTemplate::Tem(key)
#define ScrollItemTemplateRelease			ScrollItemTemplate::Release();

#endif