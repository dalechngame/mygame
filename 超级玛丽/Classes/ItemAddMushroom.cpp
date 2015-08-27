#include "ItemAddMushroom.h"
#include "Mario.h"

ItemAddMushroom* ItemAddMushroom::create(CCDictionary* dict)
{
	ItemAddMushroom* ret = new ItemAddMushroom;
	ret->init(dict);
	ret->autorelease();
	return ret;
}

bool ItemAddMushroom::init(CCDictionary* dict)
{
	Item::init();
	setPositionByProperty(dict);

	_itemReward->addObject(this);
	_dir = Common::LEFT;
	_status = SLEEP;
	_speed = 50;
	_speedDown = _speedAcc = 10;

	setVisible(false);
	setZOrder(0);

	return true;
}

void ItemAddMushroom::wakeup()
{
	_status = GROW;
	updateStatus();
	setVisible(true);

	_itemReward->removeObject(this);
}

void ItemAddMushroom::updateStatus()
{
	if (_status == GROW)
	{
		CCMoveBy* moveBy = CCMoveBy::create(1.0f, ccp(0, 16));
		CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(ItemAddMushroom::afterGrowFuncCall));
		runAction(CCSequence::createWithTwoActions(moveBy, callFunc));
	}
	else if (_status == MOVE)
	{
		// Nothing to do
	}
}

void ItemAddMushroom::afterGrowFuncCall()
{
	_status = MOVE;
	updateStatus();
	setZOrder(100);
}

void ItemAddMushroom::move(float dt)
{
	//! 回收
	if (isFarAwayFromMario())
	{
		removeFromParent();
		return;
	}

	if (_status != MOVE || !isLeftInWindow())
	{
		return;
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

	if (canMoveDown(dt))
	{
		moveDown(dt);
	}
	else
	{
		_speedDown = 10;
	}

}

void ItemAddMushroom::collision()
{
	if (_mario->_bGodMode || _mario->_status == Mario::DEAD || _status != MOVE)
	{
		return;
	}

	if (_mario->boundingBox().intersectsRect(boundingBox()))
	{
		_mario->eatMushroom(_type);
		removeFromParent();
	}
}