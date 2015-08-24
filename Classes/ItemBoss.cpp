#include "ItemBoss.h"
#include "ItemBullet.h"

ItemBoss* ItemBoss::create(CCDictionary* dict)
{
	ItemBoss* ret = new ItemBoss;
	ret->init(dict);
	ret->autorelease();
	return ret;
}
bool ItemBoss::init(CCDictionary* dict)
{
	Item::init();
	_type = Item::IT_boss;

	setPositionByProperty(dict);

	_speedDown = _speedAcc = 10;
	_speed = 50;
	_left = getPositionX() - 7 * 16;
	_right = getPositionX() + 2 * 16;
	_status = NORMAL;
	_dir = Common::LEFT;

	updateStatus();

	_Boss = this;

	return true;
}

void ItemBoss::updateStatus()
{
	if (_dir == Common::LEFT)
	{
		runAnimation("BossWalkLeft");
	}
	else if (_dir == Common::RIGHT)
	{
		runAnimation("BossWalkRight");
	}
}

void ItemBoss::move(float dt)
{
	if (_status == NORMAL)
	{
		int randNum = CCRANDOM_0_1() * 60;
		if (randNum==1  && _bCanJump)
		{
			_bCanJump = false;

			CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(ItemBoss::setAccJumpTrue));
			CCJumpBy* jump = CCJumpBy::create(.3f, ccp(0, 0), 20, 1);
			runAction(CCSequence::create(jump, callFunc, NULL));
		}

		randNum = CCRANDOM_0_1() * 60;
		if (randNum == 1)
		{
			//// 发射子弹
			//ItemBullet* bullet = ItemBullet::create();
			//getMap()->addChild(bullet);
			// 设置bullet的位置
			// 设置bullet的方向
			// 让bullet运动
		}

		if (_dir == Common::LEFT)
		{
			moveLeft(dt);
			if (getPositionX() < _left)
			{
				_dir = Common::RIGHT;
				updateStatus();
			}
		}
		else
		{
			moveRight(dt);
			if (getPositionX() > _right)
			{
				_dir = Common::LEFT;
				updateStatus();
			}
		}
	}
	else if (_status == DROPPING)
	{
		moveDown(dt);
	}
}