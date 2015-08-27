#include "ItemTortoiseFly.h"
#include "Mario.h"

ItemTortoiseFly* ItemTortoiseFly::create(CCDictionary* dict)
{
	ItemTortoiseFly* ret = new ItemTortoiseFly;
	ret->init(dict);
	ret->autorelease();
	return ret;
}

bool ItemTortoiseFly::init(CCDictionary* dict)
{
	Item::init();
	_type = Item::IT_tortoiseFly;

	setPositionByProperty(dict);

	_status = NORMAL;
	_dir = Common::LEFT;

	_flyDis = dict->valueForKey("flyDis")->intValue();
	_right = getPositionX();
	_left = _right-_flyDis;

	_speed = 50;
	_speedDown = _speedAcc = 10;

	updateStatus();

	return true;
}

void ItemTortoiseFly::updateStatus()
{
	if (_status == NORMAL)
	{
		stopAllActions();
		if (_dir == Common::LEFT)
		{
			runAnimation("TortoiseFlyLeft");
		}
		else
		{
			runAnimation("TortoiseFlyRight");

		}
	}
	else if (_status == DROPPING)
	{
		stopAllActions();

		// 设置它下降的速度
		_speedDown = _mario->_speedDown + 10;
		_mario->_speedDown = 10;

		if (_dir == Common::LEFT)
		{
			runAnimation("TortoiseMoveLeft");
		}
		else
		{
			runAnimation("TortoiseMoveRight");
		}
	}
	else if (_status == ONLAND)
	{
		// .....
	}
	else if (_status == SLEEP)
	{
		stopAllActions();
		runAnimation("TortoiseDead");

		scheduleOnce(schedule_selector(ItemTortoiseFly::Recover), 10);
		setGodMode(0.2f);
		// 微调位置
		if (_mario->getPositionX() < getPositionX())
		{
			setPositionX(_mario->getPositionX() + _mario->boundingBox().size.width + 1);
		}
		else
		{
			setPositionX(_mario->getPositionX() - boundingBox().size.width - 1);
		}

	}
	else if (_status == CRAZY)
	{
		unschedule(schedule_selector(ItemTortoiseFly::Recover));
		_speed = 200;
		_speedDown = _speedAcc = 10;
	}
}

void ItemTortoiseFly::Recover(float dt)
{
	_status = ONLAND;
	updateStatus();
}

void ItemTortoiseFly::move(float dt)
{
	if (_status == NORMAL)
	{
		if (_dir == Common::LEFT)
		{
			moveLeft(dt);
			if (getPositionX() <= _left)
			{
				_dir = Common::RIGHT;
				updateStatus();
			}
		}
		else
		{
			moveRight(dt);
			if (getPositionX() >= _right)
			{
				_dir = Common::LEFT;
				updateStatus();
			}
		}
	}
	else if (_status == DROPPING)
	{
		if (canMoveDown(dt))
		{
			moveDown(dt);
		}
		else
		{
			_speedDown = _speedAcc;
			_status = ONLAND;
			updateStatus();
		}
	}
	else if (_status == ONLAND || _status == CRAZY)
	{
		if (canMoveDown(dt))
		{
			moveDown(dt);
		}
		else
		{
			_speedDown = _speedAcc;
		}

		if (_dir == Common::LEFT)
		{
			if (canMoveLeft(dt))
			{
				moveLeft(dt);
			}
			else
			{
				_dir = Common::RIGHT;
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
			}
		}
	}
}

void ItemTortoiseFly::collision()
{
	if (_bGodMode || _mario->_status == Mario::DEAD || _mario->_bGodMode)
	{
		return;
	}

	CCRect rcItem = boundingBox();
	CCRect rcMario = _mario->boundingBox();
	if (rcItem.intersectsRect(rcMario))
	{
		if (_status == NORMAL)
		{
			if (_mario->_status == Mario::FLY && _mario->_speedUp <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2)
			{
				_status = DROPPING;
				updateStatus();
			}
			else
			{
				_mario->die(false);
			}
		}
		else if (_status == DROPPING)
		{
			// ... nothing to do
			// 设计中，乌龟下降的速度应该大于mario下降的速度，所以不应该有碰撞
		}
		else if (_status == ONLAND)
		{
			if (_mario->_status == Mario::FLY && _mario->_speedUp <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2)
			{
				_status = SLEEP;
				updateStatus();
			}
			else
			{
				_mario->die(false);
			}
		}
		else if (_status == SLEEP)
		{
			if (_bGodMode) return;

			_status = CRAZY;
			updateStatus();
		}
	}
}