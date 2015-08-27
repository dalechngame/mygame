#include "ItemMushroomAddLife.h"

ItemMushroomAddLife* ItemMushroomAddLife::create(CCDictionary* dict)
{
	ItemMushroomAddLife* ret = new ItemMushroomAddLife;
	ret->init(dict);
	ret->autorelease();
	return ret;
}

bool ItemMushroomAddLife::init(CCDictionary* dict)
{
	ItemAddMushroom::init(dict);

	_type = Item::IT_RewardAddLife;
	setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("MushroomAddLife"));

	return true;
}
