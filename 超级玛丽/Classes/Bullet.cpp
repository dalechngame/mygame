#include "Bullet.h"
#include "Mario.h"

Bullet* Bullet::create(CCDictionary* dict)
{
	Bullet* ret = new Bullet;
	ret->init(dict);
	ret->autorelease();
	return ret;
}

bool Bullet::init(CCDictionary* dict)
{
	Item::init();

	_type = Item::IT_BULLET;
	setPositionByProperty(dict);
	_pbulletArray->addObject(this);

	_speed = 200;
	_speedUp = 0;
	_speedAcc = 10;
	_speedDown = 100;
	_status = NORMAL;
	std::string str = dict->valueForKey("dir")->m_sString;

	if (str == "right")
	{
		_dir = Common::RIGHT;
	}
	else
	{
		_dir = Common::LEFT;
	}
	runAnimation("FireRight");


	return true;
}

void Bullet::updateStatus()
{
	if (_status == NORMAL)
	{
		//...
	}
	else
	{
		_speedUp = 0;
		stopAllActions();
		CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->animationByName("FireBomb");
		CCAnimate *animate = CCAnimate::create(animation);
		CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(Bullet::bomb));
		CCSequence *seq = CCSequence::createWithTwoActions(animate, call);
		runAction(seq);
	}
}

bool Bullet::canMoveUp(float dt)
{
	CCRect rcItem = boundingBox();
	CCTMXTiledMap* map = getMap();
	CCPoint pt[3];
	pt[0] = ccp(rcItem.getMidX(), rcItem.getMaxY() + dt*_speedUp);
	pt[1] = ccp(rcItem.getMinX(), rcItem.getMaxY() + dt*_speedUp);
	pt[2] = ccp(rcItem.getMaxX(), rcItem.getMaxY() + dt*_speedUp);


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
				return false;
			}
		}
	}

	return true;
}

void Bullet::moveUp(float dt)
{
	Common::moveNode(this, ccp(0, dt*_speedUp));
}

void Bullet::move(float dt)
{
	if (isFarAwayFromMario())
	{
		removeFromParent();
		_pbulletArray->removeObject(this);
		return;
	}
	if (_status == BOMB)
	{
		return;
	}

	if (_dir == Common::LEFT)
	{
		if (canMoveLeft(dt))
		{
			moveLeft(dt);
		}
		else
		{
			if (_status !=BOMB)
			{
				_status = BOMB;
				updateStatus();
			}
		}
	}
	else
	{
		if (canMoveRight(dt))
		{
			moveRight(dt);
		}
		else
		{
			if (_status != BOMB)
			{
				_status = BOMB;
				updateStatus();
			}
		}
	}
	Up(dt);
	Down(dt);
}

void Bullet::collision()
{

}

void Bullet::Up(float dt)
{
	if (_speedUp <= 0)
	{
		return;
	}
	if (canMoveUp(dt))
	{
		moveUp(dt);
		_speedUp -= _speedAcc;
	}
	else
	{
		_speedUp = 0;
		_speedDown = 10;
	}
}

void Bullet::Down(float dt)
{
	if (_speedUp <= 0)
	{
		if (canMoveDown(dt))
		{
			moveDown(dt);
		}
		else
		{
			_speedDown = _speedAcc;
			_speedUp = 150;
		}
	}
}

void Bullet::bomb()
{
	removeFromParent();
	_pbulletArray->removeObject(this);
}
