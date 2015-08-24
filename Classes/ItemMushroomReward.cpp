#include "ItemMushroomReward.h"
#include "Mario.h"

ItemMushroomReward* ItemMushroomReward::create(CCDictionary* dict)
{
	ItemMushroomReward* ret = new ItemMushroomReward;
	ret->init(dict);
	ret->autorelease();
	return ret;
}

bool ItemMushroomReward::init(CCDictionary* dict)
{
	ItemAddMushroom::init(dict);

	setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("MushroomReward"));
	_type = Item::IT_RewardGrow;

	return true;
}

void ItemMushroomReward::wakeup()
{
	if (_mario->_grow != Mario::NONE)
	{
		runAnimation("RewardFlower");
		_type = Item::IT_RewardFlower;
	}
	ItemAddMushroom::wakeup();
}

void ItemMushroomReward::move(float dt)
{
	if (_type == IT_RewardFlower)
	{
		return;
	}
	ItemAddMushroom::move(dt);

}
