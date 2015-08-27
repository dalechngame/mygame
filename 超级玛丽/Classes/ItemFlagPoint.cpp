#include "ItemFlagPoint.h"
#include "Mario.h"
ItemFlagPoint* ItemFlagPoint::create(CCDictionary* dict)
{
	ItemFlagPoint* ret = new ItemFlagPoint;
	ret->init(dict);
	ret->autorelease();
	return ret;
}
bool ItemFlagPoint::init(CCDictionary* dict)
{
	Item::init();
	_Flag = this;

	_type = Item::IT_flagpoint;

	setPositionByProperty(dict);

	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("flag.png");
	setTexture(texture);
	setTextureRect(CCRect(0, 0, texture->getContentSize().width, texture->getContentSize().height));

	Common::moveNode(this, ccp(-texture->getContentSize().width / 2, 0));

	return true;
}

void ItemFlagPoint::collision()
{
	if (_mario->boundingBox().intersectsRect(boundingBox()))
	{
		_mario->autoRunRight();
	}
}
