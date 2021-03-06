﻿#include "CrownTemplate.h"

void CrownTemplate::InitData(void)
{
	isInstance = lastLoadLanguage != ConfigLanguageSetting::instance().getCurLanguageName() ? false : isInstance;
	
	if(isInstance) 
		return;

	Release();
	lastLoadLanguage = ConfigLanguageSetting::instance().getCurLanguageName();
	isInstance		 = true;
	CrownTemplate* temp;

	temp = new CrownTemplate();
temp->Key = "P001_crown_001";
temp->firstSlotName = "crown";
temp->firstFile = "game/03_minigame/p001/clothes/crown/cloth001_crown.png";
temp->SecondSlotName = "crown001";
temp->secondFile = "game/03_minigame/p001/clothes/none.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CrownTemplate();
temp->Key = "P001_crown_002";
temp->firstSlotName = "crown";
temp->firstFile = "game/03_minigame/p001/clothes/crown/cloth002_crown.png";
temp->SecondSlotName = "crown001";
temp->secondFile = "game/03_minigame/p001/clothes/none.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CrownTemplate();
temp->Key = "P001_crown_003";
temp->firstSlotName = "crown";
temp->firstFile = "game/03_minigame/p001/clothes/crown/cloth003_crown.png";
temp->SecondSlotName = "crown001";
temp->secondFile = "game/03_minigame/p001/clothes/none.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CrownTemplate();
temp->Key = "P001_crown_004";
temp->firstSlotName = "crown";
temp->firstFile = "game/03_minigame/p001/clothes/crown/cloth004_crown.png";
temp->SecondSlotName = "crown001";
temp->secondFile = "game/03_minigame/p001/clothes/none.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CrownTemplate();
temp->Key = "P001_crown_005";
temp->firstSlotName = "crown";
temp->firstFile = "game/03_minigame/p001/clothes/crown/cloth005_crown.png";
temp->SecondSlotName = "crown001";
temp->secondFile = "game/03_minigame/p001/clothes/none.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CrownTemplate();
temp->Key = "P001_crown_006";
temp->firstSlotName = "crown";
temp->firstFile = "game/03_minigame/p001/clothes/crown/cloth006_crown.png";
temp->SecondSlotName = "crown001";
temp->secondFile = "game/03_minigame/p001/clothes/none.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CrownTemplate();
temp->Key = "P001_crown_007";
temp->firstSlotName = "crown";
temp->firstFile = "game/03_minigame/p001/clothes/crown/cloth007_crown.png";
temp->SecondSlotName = "crown001";
temp->secondFile = "game/03_minigame/p001/clothes/crown001/cloth007_crown001.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CrownTemplate();
temp->Key = "P001_crown_008";
temp->firstSlotName = "crown";
temp->firstFile = "game/03_minigame/p001/clothes/crown/cloth008_crown.png";
temp->SecondSlotName = "crown001";
temp->secondFile = "game/03_minigame/p001/clothes/none.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CrownTemplate();
temp->Key = "P001_crown_009";
temp->firstSlotName = "crown";
temp->firstFile = "game/03_minigame/p001/clothes/crown/cloth009_crown.png";
temp->SecondSlotName = "crown001";
temp->secondFile = "game/03_minigame/p001/clothes/crown001/cloth009_crown001.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CrownTemplate();
temp->Key = "P001_crown_010";
temp->firstSlotName = "crown";
temp->firstFile = "game/03_minigame/p001/clothes/crown/cloth010_crown.png";
temp->SecondSlotName = "crown001";
temp->secondFile = "game/03_minigame/p001/clothes/crown001/cloth010_crown001.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CrownTemplate();
temp->Key = "P001_crown_011";
temp->firstSlotName = "crown";
temp->firstFile = "game/03_minigame/p001/clothes/crown/cloth011_crown.png";
temp->SecondSlotName = "crown001";
temp->secondFile = "game/03_minigame/p001/clothes/none.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CrownTemplate();
temp->Key = "P001_crown_012";
temp->firstSlotName = "crown";
temp->firstFile = "game/03_minigame/p001/clothes/crown/cloth012_crown.png";
temp->SecondSlotName = "crown001";
temp->secondFile = "game/03_minigame/p001/clothes/crown001/cloth012_crown001.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CrownTemplate();
temp->Key = "P001_crown_013";
temp->firstSlotName = "crown";
temp->firstFile = "game/03_minigame/p001/clothes/crown/cloth013_crown.png";
temp->SecondSlotName = "crown001";
temp->secondFile = "game/03_minigame/p001/clothes/crown001/cloth013_crown001.png";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new CrownTemplate();
temp->Key = "";
temp->firstSlotName = "";
temp->firstFile = "";
temp->SecondSlotName = "";
temp->secondFile = "";
mapdata.insert(std::make_pair(temp->Key,temp));

}