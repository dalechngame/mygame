#include "GloveTemplate.h"

void GloveTemplate::InitData(void)
{
	isInstance = lastLoadLanguage != ConfigLanguageSetting::instance().getCurLanguageName() ? false : isInstance;
	
	if(isInstance) 
		return;

	Release();
	lastLoadLanguage = ConfigLanguageSetting::instance().getCurLanguageName();
	isInstance		 = true;
	GloveTemplate* temp;

	temp = new GloveTemplate();
temp->Key = "P001_glove_001";
temp->filepath = "game/03_minigame/p001/clothes/";
temp->slot1Name = "glove001";
temp->slot1File = "glove001/cloth001_glove001.png";
temp->slot2Name = "glove002";
temp->slot2File = "glove002/cloth001_glove002.png";
temp->slot3Name = "glove003";
temp->slot3File = "glove003/cloth001_glove003.png";
temp->slot4Name = "glove004";
temp->slot4File = "glove004/cloth001_glove004.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new GloveTemplate();
temp->Key = "P001_glove_002";
temp->filepath = "game/03_minigame/p001/clothes/";
temp->slot1Name = "glove001";
temp->slot1File = "none.png";
temp->slot2Name = "glove002";
temp->slot2File = "none.png";
temp->slot3Name = "glove003";
temp->slot3File = "glove003/cloth002_glove003.png";
temp->slot4Name = "glove004";
temp->slot4File = "glove004/cloth002_glove004.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new GloveTemplate();
temp->Key = "P001_glove_003";
temp->filepath = "game/03_minigame/p001/clothes/";
temp->slot1Name = "glove001";
temp->slot1File = "glove001/cloth003_glove001.png";
temp->slot2Name = "glove002";
temp->slot2File = "glove002/cloth003_glove002.png";
temp->slot3Name = "glove003";
temp->slot3File = "glove003/cloth003_glove003.png";
temp->slot4Name = "glove004";
temp->slot4File = "glove004/cloth003_glove004.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new GloveTemplate();
temp->Key = "P001_glove_004";
temp->filepath = "game/03_minigame/p001/clothes/";
temp->slot1Name = "glove001";
temp->slot1File = "glove001/cloth004_glove001.png";
temp->slot2Name = "glove002";
temp->slot2File = "glove002/cloth004_glove002.png";
temp->slot3Name = "glove003";
temp->slot3File = "glove003/cloth004_glove003.png";
temp->slot4Name = "glove004";
temp->slot4File = "glove004/cloth004_glove004.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new GloveTemplate();
temp->Key = "P001_glove_005";
temp->filepath = "game/03_minigame/p001/clothes/";
temp->slot1Name = "glove001";
temp->slot1File = "glove001/cloth005_glove001.png";
temp->slot2Name = "glove002";
temp->slot2File = "glove002/cloth005_glove002.png";
temp->slot3Name = "glove003";
temp->slot3File = "glove003/cloth005_glove003.png";
temp->slot4Name = "glove004";
temp->slot4File = "glove004/cloth005_glove004.png";
mapdata.insert(std::make_pair(temp->Key,temp));

}