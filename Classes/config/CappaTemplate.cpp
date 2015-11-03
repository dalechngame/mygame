#include "CappaTemplate.h"

void CappaTemplate::InitData(void)
{
	isInstance = lastLoadLanguage != ConfigLanguageSetting::instance().getCurLanguageName() ? false : isInstance;
	
	if(isInstance) 
		return;

	Release();
	lastLoadLanguage = ConfigLanguageSetting::instance().getCurLanguageName();
	isInstance		 = true;
	CappaTemplate* temp;

	temp = new CappaTemplate();
temp->Key = "P001_cappa_001";
temp->leftSlotName = "glove005";
temp->leftFile = "game/03_minigame/p001/clothes/glove005/cloth001_glove005.png";
temp->rightSlotName = "glove006";
temp->rightFile = "game/03_minigame/p001/clothes/glove006/cloth001_glove006.png";
temp->specilSlotName = "glove012";
temp->specilFile = "game/03_minigame/p001/clothes/none.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CappaTemplate();
temp->Key = "P001_cappa_002";
temp->leftSlotName = "glove005";
temp->leftFile = "game/03_minigame/p001/clothes/glove005/cloth002_glove005.png";
temp->rightSlotName = "glove006";
temp->rightFile = "game/03_minigame/p001/clothes/glove006/cloth002_glove006.png";
temp->specilSlotName = "glove012";
temp->specilFile = "game/03_minigame/p001/clothes/none.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CappaTemplate();
temp->Key = "P001_cappa_003";
temp->leftSlotName = "glove005";
temp->leftFile = "game/03_minigame/p001/clothes/glove005/cloth003_glove005.png";
temp->rightSlotName = "glove006";
temp->rightFile = "game/03_minigame/p001/clothes/glove006/cloth003_glove006.png";
temp->specilSlotName = "glove012";
temp->specilFile = "game/03_minigame/p001/clothes/none.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CappaTemplate();
temp->Key = "P001_cappa_004";
temp->leftSlotName = "glove005";
temp->leftFile = "game/03_minigame/p001/clothes/glove005/cloth004_glove005.png";
temp->rightSlotName = "glove006";
temp->rightFile = "game/03_minigame/p001/clothes/glove006/cloth004_glove006.png";
temp->specilSlotName = "glove012";
temp->specilFile = "game/03_minigame/p001/clothes/none.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CappaTemplate();
temp->Key = "P001_cappa_005";
temp->leftSlotName = "glove005";
temp->leftFile = "game/03_minigame/p001/clothes/glove005/cloth005_glove005.png";
temp->rightSlotName = "glove006";
temp->rightFile = "game/03_minigame/p001/clothes/glove006/cloth005_glove006.png";
temp->specilSlotName = "glove012";
temp->specilFile = "game/03_minigame/p001/clothes/none.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CappaTemplate();
temp->Key = "P001_cappa_006";
temp->leftSlotName = "glove005";
temp->leftFile = "game/03_minigame/p001/clothes/glove005/cloth006_glove005.png";
temp->rightSlotName = "glove006";
temp->rightFile = "game/03_minigame/p001/clothes/glove006/cloth006_glove006.png";
temp->specilSlotName = "glove012";
temp->specilFile = "game/03_minigame/p001/clothes/none.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CappaTemplate();
temp->Key = "P001_cappa_007";
temp->leftSlotName = "glove005";
temp->leftFile = "game/03_minigame/p001/clothes/glove005/cloth007_glove005.png";
temp->rightSlotName = "glove006";
temp->rightFile = "game/03_minigame/p001/clothes/glove006/cloth007_glove006.png";
temp->specilSlotName = "glove012";
temp->specilFile = "game/03_minigame/p001/clothes/glove012/cloth007_glove012.png";
mapdata.insert(std::make_pair(temp->Key,temp));

}