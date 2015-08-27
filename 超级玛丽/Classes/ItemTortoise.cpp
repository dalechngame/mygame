#include "ItemTortoise.h"
#include "Mario.h"

ItemTortoise* ItemTortoise::create(CCDictionary* dict)
{
	ItemTortoise* ret = new ItemTortoise;
	ret->init(dict);
	ret->autorelease();
	return ret;
}

bool ItemTortoise::init(CCDictionary* dict)
{
	Item::init();
	_type = Item::IT_tortoise;

	setPositionByProperty(dict);
	
	_dir = Common::LEFT;
	_speed = 50;
	_speedAcc = _speedDown = 10;

	_status = NORMAL;

	runAnimation("TortoiseMoveLeft");
	return true;
}

void ItemTortoise::move(float dt)
{
	if (isFarAwayFromMario())
	{
		removeFromParent();
		return;
	}
	if (_status == SLEEP || _status ==DEAD || !isLeftInWindow())
	{
		return ;
	}

	// 根据方向，再具体确定
	if (_dir == Common::LEFT)
	{
		if (canMoveLeft(dt))
		{
			moveLeft(dt);
		}
		else
		{
			_dir = Common::RIGHT;
			if (_status == NORMAL)
			{
				stopAllActions();
				runAnimation("TortoiseMoveLeft");
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
			_dir = Common::LEFT;
			if (_status == NORMAL)
			{
				stopAllActions();
				runAnimation("TortoiseMoveRight");
			}
		}
	}

	if (canMoveDown(dt))
	{
		moveDown(dt);
	}
	else
	{
		_speedDown = 10;
	}

}

void ItemTortoise::collision()
{
	// 防止持续碰撞检测
	if ( _bGodMode || _mario->_status ==Mario::DEAD ||_mario->_bGodMode)
	{
		return;
	}
	CCRect rcMario = _mario->boundingBox();
	CCRect rcItem = this->boundingBox();
	bool coll = rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2;
	// mario 碰撞
	if (rcMario.intersectsRect(rcItem))
	{
		if ((_status == NORMAL))
		{
			if (marioUpHit(rcMario, rcItem, coll))
			{
				_status = SLEEP;
				scheduleOnce(schedule_selector(ItemTortoise::revive), 10.0f);
			}
			else
			{
				_mario->die(false);
			}
		}
		else if (_status == SLEEP)
		{
			Common::playMusic(std::string("CaiSiGuaiWu"));

			_speed = 150;
			_status = CRAZY;
			setGodMode(1.0f);
			unschedule(schedule_selector(ItemTortoise::revive));

			if (_mario->getPositionX() < getPositionX())
			{
				_dir = Common::RIGHT;
			}
			else
			{
				_dir = Common::LEFT;
			}

		}
		else if (_status == CRAZY)
		{
			if (marioUpHit(rcMario, rcItem, coll))
			{
				_status = DEAD;
			}
			else
			{
				_mario->die(false);
			}
		}
		else 
		{
			Common::playMusic(std::string("CaiSiGuaiWu"));

			_speed = 0;
			CCJumpBy* moveUp = CCJumpBy::create(0.7f, ccp(7, -10), 8, 1);
			CCMoveBy* moveDown = CCMoveBy::create(8.0f, ccp(0, -winSize.height));
			CCCallFunc* callfunc = CCCallFunc::create(this, callfunc_selector(ItemTortoise::die));
			CCSequence* seq = CCSequence::create(moveUp, moveDown, callfunc, NULL);

			this->runAction(seq);
		}
	}
}

void ItemTortoise::revive(float dt)
{
	_status = NORMAL;
	_speed = 50;
	stopAllActions();

	if (_dir == Common::LEFT)
	{
		runAnimation("TortoiseMoveLeft");
	}
	else
	{
		runAnimation("TortoiseMoveRight");
	}
}

bool ItemTortoise::marioUpHit(CCRect rcMario, CCRect rcItem, bool coll)
{
	if (_mario->_status == Mario::FLY && _mario->_speedUp <= 0 && coll)
	{
		stopAllActions();
		setGodMode(0.5f);
		runAnimation("TortoiseDead");
		_speed = 0;

		//! mario 小跳
		_mario->_speedUp = 150;
		_mario->_speedDown = 10;
		_mario->_status = Mario::FLY;
		_mario->updateStatus();
		// 微调位置
		if (_mario->getPositionX() < getPositionX())
		{
			Common::moveNode(this, ccp(5, 0));
		}
		else
		{
			Common::moveNode(this, ccp(-5, 0));
		}
		return true;
	}
	return false;
}

void ItemTortoise::die()
{
	this->removeFromParent();
}