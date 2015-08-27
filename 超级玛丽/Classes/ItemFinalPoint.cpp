#include "ItemFinalPoint.h"
#include "Mario.h"
#include "LayerGame.h"
#include "GameInfo.h"

ItemFinalPoint* ItemFinalPoint::create(CCDictionary* dict)
{
	ItemFinalPoint* ret = new ItemFinalPoint;
	ret->init(dict);
	ret->autorelease();
	return ret;
}

bool ItemFinalPoint::init(CCDictionary* dict)
{
	Item::init();
	_type = Item::IT_finalpoint;

	setPositionByProperty(dict);

	return true;
}

void ItemFinalPoint::collision()
{
	if (_mario->getPositionX() > getPositionX())
	{

		CC_SAFE_RELEASE(_itemReward);
		CC_SAFE_RELEASE(_pbulletArray);
		GameInfo::Utime = 300;
		LayerGame::_idx++;
		GameInfo::ULevel = LayerGame::_idx + 1;
		Mario::_life = 3;

		CCDirector::sharedDirector()->replaceScene(Common::scene(LayerGame::create(LayerGame::_idx)));
	}
}
