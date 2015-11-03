/********************************************************************
author:	ConfigGenerator
*********************************************************************/

#ifndef __AnimeTemplate__HPP__NO_CONFLICT
#define __AnimeTemplate__HPP__NO_CONFLICT

#include "ConfigDataTemplate.hpp"
#include "ConfigLanguageSetting.h"

class AnimeTemplate : public ConfigDataTemplate<AnimeTemplate>
{
public:
	static void InitData(void);
public:
	std::string Key;

	bool princessFirst;
std::string princessSoundId;
std::string princessTalk;
std::string princessAni;
std::string princessFonts;
float princessTime;
int guestModel;
std::string guestSoundId;
std::string guestTalk;
std::string guestAni;
std::string guestFonts;
float guestTime;
int healthModel;

};

#define AnimeTemplateDic				AnimeTemplate::Dic()
#define AnimeTemplateLis(contextKey)	AnimeTemplate::Lis(contextKey)
#define AnimeTemplateTem(key)			AnimeTemplate::Tem(key)
#define AnimeTemplateRelease			AnimeTemplate::Release();

#endif