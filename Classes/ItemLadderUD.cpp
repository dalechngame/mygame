#include "ItemLadderUD.h"
#include"Mario.h"

ItemLadderUD* ItemLadderUD::create(CCDictionary* dict)
{
	ItemLadderUD* ret = new ItemLadderUD;
	ret->init(dict);
	ret->autorelease();
	return ret;
}

bool ItemLadderUD::init(CCDictionary* dict)
{
	Item::init();
	_type = Item::IT_LadderUD;

	setPositionByProperty(dict);

	// 梯子的显示
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("ladder.png");
	setTexture(texture);
	setTextureRect(CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height));

	Common::moveNode(this, ccp(-texture->getContentSize().width / 2, 0));

	// 属性
	_UorD = 1-dict->valueForKey("UorD")->intValue();  // 一开始向左还是向右
	_ladderDis = dict->valueForKey("ladderDis")->intValue();  // 摆动的幅度

	// 梯子一个回合已经移动过的距离
	_dis = 0;
	// 梯子移动速度
	_speed = 20;
	// 标记mario在我这个梯子上
	_marioOnThisLadder = false;


#if 0
	<object name = "gadget" type = "ladderUD" x = "904" y = "128">
		<properties>
		<property name = "UorD" value = "0" / >
		<property name = "ladderDis" value = "48" / >
		< / properties>
		< / object>
#endif

	return true;
}

void ItemLadderUD::move(float dt)
{
	// 移动
	if (_UorD) // 向左
	{
		Common::moveNode(this, ccp(0, -dt*_speed));
		if (_marioOnThisLadder)
		{
			Common::moveNode(_mario, ccp(0, -dt*_speed));
		}
	}
	else
	{
		Common::moveNode(this, ccp(0, dt*_speed));
		if (_marioOnThisLadder)
		{
			Common::moveNode(_mario, ccp(0, dt*_speed));
		}
	}

	// 累加移动的距离，判断转向
	_dis += dt*_speed;
	if (_dis >= _ladderDis)
	{
		_UorD = 1 - _UorD;
		_dis = 0;
	}
}

void ItemLadderUD::collision()
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