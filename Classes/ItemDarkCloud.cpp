#include "ItemDarkCloud.h"

ItemDarkCloud* ItemDarkCloud::create(CCDictionary* dict)
{
	ItemDarkCloud* ret = new ItemDarkCloud;
	ret->init(dict);
	ret->autorelease();
	return ret;
}
bool ItemDarkCloud::init(CCDictionary* dict)
{
	Item::init();
	_type = Item::IT_mushroom;

	setPositionByProperty(dict);



	// ͼƬ
	setDisplayFrameWithAnimationName("MushroomMoving", 0);


	return true;
}
