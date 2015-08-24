#include "ItemBattery.h"

ItemBattery* ItemBattery::create(CCDictionary* dict)
{
	ItemBattery* ret = new ItemBattery;
	ret->init(dict);
	ret->autorelease();
	return ret;
}
bool ItemBattery::init(CCDictionary* dict)
{
	Item::init();
	_type = Item::IT_mushroom;

	setPositionByProperty(dict);



	// ͼƬ
	setDisplayFrameWithAnimationName("MushroomMoving", 0);


	return true;
}
