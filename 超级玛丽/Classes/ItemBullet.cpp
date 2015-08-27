#include "ItemBullet.h"

ItemBullet* ItemBullet::create(CCDictionary* dict)
{
	ItemBullet* ret = new ItemBullet;
	ret->init(dict);
	ret->autorelease();
	return ret;
}
bool ItemBullet::init(CCDictionary* dict)
{
	Item::init();
	_type = Item::IT_mushroom;

	setPositionByProperty(dict);



	// ͼƬ
	setDisplayFrameWithAnimationName("MushroomMoving", 0);


	return true;
}
