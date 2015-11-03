/********************************************************************
author:	ConfigGenerator
*********************************************************************/

#ifndef __SoundTemplate__HPP__NO_CONFLICT
#define __SoundTemplate__HPP__NO_CONFLICT

#include "ConfigDataTemplate.hpp"
#include "ConfigLanguageSetting.h"

class SoundTemplate : public ConfigDataTemplate<SoundTemplate>
{
public:
	static void InitData(void);
public:
	std::string Key;

	std::string Name;
std::string PlayMode;
float Duration;
float Delay;
bool IsLoop;
std::string Mutex;
float Volume;

};

#define SoundTemplateDic				SoundTemplate::Dic()
#define SoundTemplateLis(contextKey)	SoundTemplate::Lis(contextKey)
#define SoundTemplateTem(key)			SoundTemplate::Tem(key)
#define SoundTemplateRelease			SoundTemplate::Release();

#endif