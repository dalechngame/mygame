#include "ItemBridgeStartPos.h"

ItemBridgeStartPos* ItemBridgeStartPos::create(CCDictionary* dict)
{
	ItemBridgeStartPos* ret = new ItemBridgeStartPos;
	ret->init(dict);
	ret->autorelease();
	return ret;
}
bool ItemBridgeStartPos::init(CCDictionary* dict)
{
	Item::init();
	_type = Item::IT_mushroom;

	setPositionByProperty(dict);



	// ͼƬ
	setDisplayFrameWithAnimationName("MushroomMoving", 0);


	return true;
}
