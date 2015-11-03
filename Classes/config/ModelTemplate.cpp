#include "ModelTemplate.h"

void ModelTemplate::InitData(void)
{
	isInstance = lastLoadLanguage != ConfigLanguageSetting::instance().getCurLanguageName() ? false : isInstance;
	
	if(isInstance) 
		return;

	Release();
	lastLoadLanguage = ConfigLanguageSetting::instance().getCurLanguageName();
	isInstance		 = true;
	ModelTemplate* temp;

	temp = new ModelTemplate();
temp->Key = "model1";
temp->no = 1;
temp->type = 1;
temp->animal = false;
temp->npcPosY = -50.0f;
temp->norJsonFile = "game/skeleton/libii/princess.json";
temp->norAtlasFile = "game/skeleton/libii/princess.atlas";
temp->skin = "hair10";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new ModelTemplate();
temp->Key = "model2";
temp->no = 2;
temp->type = 2;
temp->animal = false;
temp->npcPosY = 50.0f;
temp->norJsonFile = "game/skeleton/mguest/mguest.json";
temp->norAtlasFile = "game/skeleton/mguest/mguest.atlas";
temp->skin = "a";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new ModelTemplate();
temp->Key = "model3";
temp->no = 3;
temp->type = 2;
temp->animal = false;
temp->npcPosY = 50.0f;
temp->norJsonFile = "game/skeleton/mguest/mguest.json";
temp->norAtlasFile = "game/skeleton/mguest/mguest.atlas";
temp->skin = "b";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new ModelTemplate();
temp->Key = "model4";
temp->no = 4;
temp->type = 3;
temp->animal = false;
temp->npcPosY = 100.0f;
temp->norJsonFile = "game/skeleton/fguest/fguest4.json";
temp->norAtlasFile = "game/skeleton/fguest/fguest4.atlas";
temp->skin = "a";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new ModelTemplate();
temp->Key = "model5";
temp->no = 5;
temp->type = 3;
temp->animal = false;
temp->npcPosY = 80.0f;
temp->norJsonFile = "game/skeleton/fguest/fguest4.json";
temp->norAtlasFile = "game/skeleton/fguest/fguest4.atlas";
temp->skin = "b";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new ModelTemplate();
temp->Key = "model6";
temp->no = 6;
temp->type = 3;
temp->animal = false;
temp->npcPosY = 100.0f;
temp->norJsonFile = "game/skeleton/fguest/fguest4.json";
temp->norAtlasFile = "game/skeleton/fguest/fguest4.atlas";
temp->skin = "c";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new ModelTemplate();
temp->Key = "model7";
temp->no = 7;
temp->type = 5;
temp->animal = true;
temp->npcPosY = 50.0f;
temp->norJsonFile = "game/skeleton/dog/dog.json";
temp->norAtlasFile = "game/skeleton/dog/dog.atlas";
temp->skin = "a";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new ModelTemplate();
temp->Key = "model8";
temp->no = 8;
temp->type = 4;
temp->animal = true;
temp->npcPosY = 50.0f;
temp->norJsonFile = "game/skeleton/cat/cat.json";
temp->norAtlasFile = "game/skeleton/cat/cat.atlas";
temp->skin = "a";
mapdata.insert(std::make_pair(temp->Key,temp));
temp = new ModelTemplate();
temp->Key = "model9";
temp->no = 9;
temp->type = 6;
temp->animal = false;
temp->npcPosY = 50.0f;
temp->norJsonFile = "game/skeleton/p008/hair/hair.json";
temp->norAtlasFile = "game/skeleton/p008/hair/hair.atlas";
temp->skin = "hair01";
mapdata.insert(std::make_pair(temp->Key,temp));

}