#include "ItemTortoiseRound.h"
#include "Mario.h"
ItemTortoiseRound* ItemTortoiseRound::create(CCDictionary* dict)
{
	ItemTortoiseRound* ret = new ItemTortoiseRound;
	ret->init(dict);
	ret->autorelease();
	return ret;
}

bool ItemTortoiseRound::init(CCDictionary* dict)
{
	Item::init();
	_type = Item::IT_tortoiseRound;

	setPositionByProperty(dict);
	_speed = 50;
	_speedAcc = _speedDown = 10;

	_dir = Common::LEFT;
	_status = NORMAL;

	_roundDis = dict->valueForKey("roundDis")->intValue();// 巡逻的距离
	_rightMost = getPositionX();
	_leftMost = _rightMost - _roundDis;

	updateStatus();

#if 0
	<object name = "enemy" type = "tortoise_round" x = "1664" y = "112">
		<properties>
		<property name = "roundDis" value = "64" / >
		< / properties>
		< / object>
#endif

	return true;
}

void ItemTortoiseRound::move(float dt)
{
	if (isFarAwayFromMario())
	{
		removeFromParent();
		return;
	}
	if (!isLeftInWindow() || _status ==SLEEP)
	{
		return;
	}

	if (_status == NORMAL)
	{
		if (_dir == Common::LEFT)
		{
			Common::moveNode(this, ccp(-dt*_speed, 0));
			if (getPositionX() <= _leftMost)
			{
				_dir = Common::RIGHT;
				updateStatus();
			}
		}
		else
		{
			Common::moveNode(this, ccp(dt*_speed, 0));
			if (getPositionX() >= _rightMost)
			{
				_dir = Common::LEFT;
				updateStatus();
			}
		}
	}

	if (_status == CRAZY)
	{
		if (_dir == Common::LEFT)
		{
			if (canMoveLeft(dt))
				moveLeft(dt);
			else
				_dir = Common::RIGHT;
		}
		else
		{
			if (canMoveRight(dt))
				moveRight(dt);
			else
				_dir = Common::LEFT;
		}

		if (canMoveDown(dt))
			moveDown(dt);
	}
}

void ItemTortoiseRound::updateStatus()
{
	if (_status == NORMAL)
	{
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
	else if (_status==SLEEP)
	{
		stopAllActions();
		runAnimation("TortoiseDead");

		scheduleOnce(schedule_selector(ItemTortoiseRound::Recover), 3);
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
		unschedule(schedule_selector(ItemTortoiseRound::Recover));
		_speed = 200;
		_dir = _mario->_dir;
	}

}

void ItemTortoiseRound::Recover(float)
{
	_status = NORMAL;
	updateStatus();
}

void ItemTortoiseRound::collision()
{
	if (_bGodMode || _mario->_status == Mario::DEAD || _mario->_bGodMode)
	{
		return;
	}
	CCRect rcMario = _mario->boundingBox();
	CCRect rcItem = this->boundingBox();

	// 判断碰撞检测
	if (rcMario.intersectsRect(rcItem))
	{
		if (_status == NORMAL)
		{
			if (_mario->_status==Mario::FLY && _mario->_speedUp <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2)
			{
				_status = SLEEP;
				updateStatus();
			}
			else
			{
				_mario->die(false);
			}
		}
		else if (_status == SLEEP && !_bGodMode)
		{
			_status = CRAZY;
			updateStatus();
		}
	}
}
