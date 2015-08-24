#include "Item.h"
#include "Mario.h"
#include "ItemMushroom.h"
#include "ItemTortoise.h"
#include "ItemFlower.h"
#include "ItemMushroomReward.h"
#include "ItemFlagPoint.h"
#include "ItemMushroomAddLife.h"
#include "ItemFinalPoint.h"
#include "ItemLadderLR.h"
#include "ItemFlyFish.h"
#include "ItemTortoiseRound.h"
#include "ItemTortoiseFly.h"
#include "ItemLadderUD.h"
#include "ItemFireString.h"
#include "ItemBoss.h"
#include "ItemBridgeStartPos.h"
#include "Bullet.h"

CCArray* Item::_itemReward;
CCArray *Item::_pbulletArray;
Item* Item::_Flag;
Item* Item::_Boss;
Item* Item::_bridge;

Item* Item::create(CCDictionary* dict)
{
	const CCString* type = dict->valueForKey("type");

	if (type->m_sString == "mushroom")
		return ItemMushroom::create(dict);
	else if (type->m_sString == "tortoise")
		return ItemTortoise::create(dict);
	else if (type->m_sString == "flower")
		return ItemFlower::create(dict);
	else if (type->m_sString == "MushroomReward")
		return ItemMushroomReward::create(dict);
	else if (type->m_sString == "flagpoint")
		return ItemFlagPoint::create(dict);
	else if (type->m_sString == "MushroomAddLife")
		return ItemMushroomAddLife::create(dict);
	else if (type->m_sString == "finalpoint")
		return ItemFinalPoint::create(dict);
	else if (type->m_sString == "ladderLR")
		return ItemLadderLR::create(dict);
	else if (type->m_sString == "ladderUD")
		return ItemLadderUD::create(dict);
	else if (type->m_sString == "flyfish")
		return ItemFlyFish::create(dict);
	else if (type->m_sString == "tortoise_round")
		return ItemTortoiseRound::create(dict);
	else if (type->m_sString == "tortoise_fly")
		return ItemTortoiseFly::create(dict);
	else if (type->m_sString == "fire_string")
		return ItemFireString::create(dict);
	else if (type->m_sString == "boss")
		return ItemBoss::create(dict);
	else if (type->m_sString == "bridgestartpos")
		return ItemBridgeStartPos::create(dict);
	else if (type->m_sString == "fire")
		return Bullet::create(dict);

	return NULL;
}

bool Item::init()
{
	CCSprite::init();

	_bGodMode = false;
	setZOrder(100);
	// 为道具提供动力
	scheduleUpdate();

	return true;
}

void Item::setPositionByProperty(CCDictionary* dict)
{
	const CCString* x = dict->valueForKey("x");
	const CCString* y = dict->valueForKey("y");
	setPosition(ccp(x->intValue(), y->intValue() - 16));
	setAnchorPoint(ccp(0, 0));
}

bool Item::isLeftInWindow()
{
	CCRect rcItem = boundingBox();
	CCPoint ptInMap = ccp(rcItem.getMinX(), rcItem.getMinY());

	CCTMXTiledMap* map = getMap();
	CCPoint ptInWorld = map->convertToWorldSpace(ptInMap);
	//CCLOG("x = %f, y =%f", ptInWorld.x, ptInWorld.y);
	if (ptInWorld.x <= winSize.width)
	{
		return true;
	}

	return false;
}

bool Item::isFarAwayFromMario()
{
	CCRect rcItem = boundingBox();
	CCRect rcMario = _mario->boundingBox();
	CCTMXTiledMap* map = getMap();

	CCPoint ptItem = ccp(rcItem.getMinX(), rcItem.getMinY());
	CCPoint ptItemInWorld = map->convertToWorldSpace(ptItem);

	if ((rcMario.getMinX() - rcItem.getMaxX() )>= winSize.width || ptItemInWorld.x<=0)
	{
		return true;
	}
	return false;
}

bool Item::canMoveLeft(float dt)
{
	CCRect rcItem = boundingBox();
	CCPoint ptItem = ccp(rcItem.getMinX(), rcItem.getMinY());
	CCTMXTiledMap* map = getMap();
	CCPoint ptItemInWorld = map->convertToWorldSpace(ptItem);
	if (ptItemInWorld.x - dt*_speed < 0) // - dt*_speed表示移动后是不是出去了，而不是当前已经出去了
		return true;

	CCPoint pt[3];
	pt[0] = ccp(rcItem.getMinX() - dt*_speed, rcItem.getMidY());
	pt[1] = ccp(rcItem.getMinX() - dt*_speed, rcItem.getMinY());
	pt[2] = ccp(rcItem.getMinX() - dt*_speed, rcItem.getMaxY());

	for (int i = 0; i < 3; ++i)
	{
		if (pt[i].y >= map->getContentSize().height || pt[i].y < 0)
		{
			continue;
		}

		CCPoint ptTile = Common::Point2Tile(map, pt[i]);
		// 水管、砖头，地板
		static const char* layerName[3] = { "block", "pipe", "land" };
		for (int j = 0; j < 3; ++j)
		{
			CCTMXLayer* layer = map->layerNamed(layerName[j]);
			int gid = layer->tileGIDAt(ptTile);
			if (gid != 0)
			{
				return false;
			}
		}
	}

	return true;
}

bool Item::canMoveRight(float dt)
{
	CCRect rcItem = boundingBox();
	CCTMXTiledMap* map = getMap();
	CCPoint pt[3];
	pt[0] = ccp(rcItem.getMaxX() + dt*_speed, rcItem.getMidY());
	pt[1] = ccp(rcItem.getMaxX() + dt*_speed, rcItem.getMinY());
	pt[2] = ccp(rcItem.getMaxX() + dt*_speed, rcItem.getMaxY());

	for (int i = 0; i < 3; ++i)
	{
		if (pt[i].y >= map->getContentSize().height || pt[i].y < 0)
		{
			continue;
		}

		CCPoint ptTile = Common::Point2Tile(map, pt[i]);
		// 水管、砖头，地板
		static const char* layerName[3] = { "block", "pipe", "land" };
		for (int j = 0; j < 3; ++j)
		{
			CCTMXLayer* layer = map->layerNamed(layerName[j]);
			int gid = layer->tileGIDAt(ptTile);
			if (gid != 0)
			{
				return false;
			}
		}
	}

	return true;
}

bool Item::canMoveDown(float dt)
{

	CCRect rcItem = boundingBox();
	CCTMXTiledMap* map = getMap();
	CCPoint pt[3];
	pt[0] = ccp(rcItem.getMidX(), rcItem.getMinY() - dt*_speedDown);
	pt[1] = ccp(rcItem.getMinX(), rcItem.getMinY() - dt*_speedDown);
	pt[2] = ccp(rcItem.getMaxX(), rcItem.getMinY() - dt*_speedDown);

	if (pt[0].y >= map->getContentSize().height || pt[0].y <= 0)
	{
		return true;
	}

	for (int i = 0; i < 3; ++i)
	{
		CCPoint ptTile = Common::Point2Tile(map, pt[i]);
		// 水管、砖头，地板
		static const char* layerName[3] = { "block", "pipe", "land" };
		for (int j = 0; j < 3; ++j)
		{
			CCTMXLayer* layer = map->layerNamed(layerName[j]);
			int gid = layer->tileGIDAt(ptTile);
			if (gid != 0)
			{
				// 微调
				CCPoint ptLB = Common::Tile2PointLB(map, ptTile + ccp(0, -1));
				this->setPositionY(ptLB.y);

				return false;
			}
		}
	}

	return true;
}

void Item::moveLeft(float dt)
{
	Common::moveNode(this, ccp(-dt*_speed, 0));
}

void Item::moveRight(float dt)
{
	Common::moveNode(this, ccp(dt*_speed, 0));
}

void Item::moveDown(float dt)
{
	Common::moveNode(this, ccp(0, -dt*_speedDown));
	_speedDown += _speedAcc;
}

void Item::update(float dt)
{
	move(dt);
	collision();
}

void Item::runAnimation(const char* name)
{
	stopAllActions();
	CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->animationByName(name);
	runAction(CCRepeatForever::create(CCAnimate::create(animation)));
}