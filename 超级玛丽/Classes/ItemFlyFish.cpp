#include "ItemFlyFish.h"
#include "Mario.h"
ItemFlyFish* ItemFlyFish::create(CCDictionary* dict)
{
	ItemFlyFish* ret = new ItemFlyFish;
	ret->init(dict);
	ret->autorelease();
	return ret;
}
bool ItemFlyFish::init(CCDictionary* dict)
{
	Item::init();
	_type = Item::IT_flyfish;

	setPositionByProperty(dict);

	// 三个参数分别是持续时间x,y轴的偏移量
	_duration = dict->valueForKey("duration")->intValue();
	_offsetH = dict->valueForKey("offsetH")->intValue();
	_offsetV = dict->valueForKey("offsetV")->intValue();

	_dir = _offsetH > 0 ? Common::RIGHT : Common::LEFT;

	_alreadyFly = false;


	return true;
}

void ItemFlyFish::move(float dt)
{
	if ((getPositionX() > _mario->getPositionX()) || _alreadyFly)
	{
		return;

	}
	_alreadyFly = true;
	updateStatus();

	// 让飞鱼做贝塞尔曲线运动
	ccBezierConfig c;
	c.controlPoint_1.x = CCRANDOM_0_1() * _offsetH;
	c.controlPoint_1.y = CCRANDOM_0_1() * _offsetV;
	c.controlPoint_2.x = CCRANDOM_0_1() * _offsetH;
	c.controlPoint_2.y = CCRANDOM_0_1() * _offsetV;
	c.endPosition = ccp(_offsetH, _offsetV);

	CCBezierBy* bezier = CCBezierBy::create(_duration,  c);

	ccBezierConfig c1;
	c1.controlPoint_1.x = CCRANDOM_0_1() * _offsetH;
	c1.controlPoint_1.y = CCRANDOM_0_1() * _offsetV;
	c1.controlPoint_2.x = CCRANDOM_0_1() * _offsetH;
	c1.controlPoint_2.y = CCRANDOM_0_1() * _offsetV;
	c1.endPosition = ccp(_offsetH, -_offsetV);

	CCBezierBy* bezier1 = CCBezierBy::create(_duration, c1);

	CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(ItemFlyFish::removeFromParent));

	runAction(CCSequence::create( bezier, bezier1, callFunc, NULL));
}

void ItemFlyFish::updateStatus()
{
	stopAllActions();

	CCAnimation* animation = NULL;
	if (_dir == Common::LEFT)
	{
		animation = CCAnimationCache::sharedAnimationCache()->animationByName("FlyFishLeft");
	}
	else
	{
		animation = CCAnimationCache::sharedAnimationCache()->animationByName("FlyFishRight");
	}

	if (animation)
	{
		CCAnimate* animate = CCAnimate::create(animation);
		CCRepeatForever* rep = CCRepeatForever::create(animate);
		runAction(rep);
	}
}

void ItemFlyFish::collision()
{
	if (_mario->_status == Mario::DEAD || _mario->_bGodMode)
	{
		return;
	}
	if (_mario->boundingBox().intersectsRect(this->boundingBox()))
	{
		_mario->die(false);
	}
}
