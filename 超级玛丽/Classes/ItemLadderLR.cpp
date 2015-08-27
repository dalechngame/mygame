#include "ItemLadderLR.h"
#include "Mario.h"

ItemLadderLR* ItemLadderLR::create(CCDictionary* dict)
{
	ItemLadderLR* ret = new ItemLadderLR;
	ret->init(dict);
	ret->autorelease();
	return ret;
}

bool ItemLadderLR::init(CCDictionary* dict)
{
	Item::init();
	_type = Item::IT_LadderLR;

	setPositionByProperty(dict);

	// 梯子的显示
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("ladder.png");
	setTexture(texture);
	setTextureRect(CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height));

	Common::moveNode(this, ccp(-texture->getContentSize().width / 2, 0));

	// 属性
	_LorR = 1-dict->valueForKey("LorR")->intValue();  // 一开始向左还是向右
	_ladderDis = dict->valueForKey("ladderDis")->intValue();  // 摆动的幅度

	// 梯子一个回合已经移动过的距离
	_dis = 0;
	// 梯子移动速度
	_speed = 20;
	// 标记mario在我这个梯子上
	_marioOnThisLadder = false;

	return true;
}

void ItemLadderLR::move(float dt)
{
	// 移动
	if (_LorR) // 向左
	{
		Common::moveNode(this, ccp(-dt*_speed, 0));
		if (_marioOnThisLadder)
		{
			Common::moveNode(_mario, ccp(-dt*_speed, 0));
		}
	}
	else
	{
		Common::moveNode(this, ccp(dt*_speed, 0));
		if (_marioOnThisLadder)
		{
			Common::moveNode(_mario, ccp(dt*_speed, 0));
		}
	}

	// 累加移动的距离，判断转向
	_dis += dt*_speed;
	if (_dis >= _ladderDis)
	{
		_LorR = 1 - _LorR;
		_dis = 0;
	}
}

void ItemLadderLR::collision()
{
	CCRect rcMario = _mario->boundingBox();
	CCRect rcItem = boundingBox();

	if (rcItem.intersectsRect(rcMario))
	{
		// 脑袋撞上了
		if (_mario->_speedUp > 0)
		{
			_mario->_speedDown = 10;
			_mario->_speedUp = 0;
		}
		else
		{
			// 刚落地
			if (_mario->_status ==Mario::FLY)
			{
				_mario->_onLadder = true;
				_marioOnThisLadder = true;
				_mario->setPositionY(rcItem.getMaxY());

			}
		}
	}
	else
	{
		if (_marioOnThisLadder)
		{
			_marioOnThisLadder = false;
			_mario->_onLadder = false;
		}
	}
	
}