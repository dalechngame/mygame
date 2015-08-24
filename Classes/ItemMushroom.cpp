#include "ItemMushroom.h"
#include "Mario.h"
#include "GameInfo.h"
#include "LayerGame.h"

ItemMushroom* ItemMushroom::create(CCDictionary* dict)
{
	ItemMushroom* ret = new ItemMushroom;
	ret->init(dict);
	ret->autorelease();
	return ret;
}

bool ItemMushroom::init(CCDictionary* dict)
{
	Item::init();
	_type = Item::IT_mushroom;

	setPositionByProperty(dict);

	_dir = Common::LEFT;
	_speed = 50;
	_speedAcc = _speedDown = 10;
	_isDead = false;

	this->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("MushroomMoving"))));

	return true;
}

void ItemMushroom::die()
{
	this->removeFromParent();

}

void ItemMushroom::move(float dt)
{
	//! 回收
	if (isFarAwayFromMario())
	{
		removeFromParent();
		return;
	}
	if (_isDead || !isLeftInWindow())
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

void ItemMushroom::collision()
{
	// 防止持续碰撞检测
	if (_bGodMode || _isDead || _mario->_status==Mario::DEAD || _mario->_bGodMode)
	{
		return;
	}

	CCRect rcMario = _mario->boundingBox();
	CCRect rcItem = this->boundingBox();
	CCTMXTiledMap* map = getMap();

	// 判断碰撞检测
	if (rcMario.intersectsRect(rcItem))
	{
		// 蘑菇怪死亡的条件，1.Mario在飞，2.Mario在下降，3.Mario的位置比蘑菇怪高，高半个身位
		if (_mario->_status ==Mario::FLY && _mario->_speedUp <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("CaiSiGuaiWu.wav");
			{
				this->_isDead = true;
				this->setGodMode(0.2f);
				this->stopAllActions();
				this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("MushroomDead1"));

			}
			{
				LayerGame *game = (LayerGame *)map->getParent();
				CCPoint ptItem = ccp(rcItem.getMinX() + 10, rcItem.getMinY() + 10);
				game->monsterScore->stopAllActions();
				game->monsterScore->setVisible(true);
				game->monsterScore->setPosition(ptItem);
				game->monsterScore->setString(CCString::createWithFormat("%d", MUSHROOM_SCORE)->getCString());

				CCMoveBy *move = CCMoveBy::create(0.7f, ccp(0, 50));
				CCCallFunc *disappear = CCCallFunc::create(this, callfunc_selector(LayerGame::disaCall));
				CCSequence *sequ = CCSequence::create(move, disappear, nullptr);
				game->monsterScore->runAction(sequ);
				GameInfo::setScore(MUSHROOM_SCORE);
			}

			{
				CCDelayTime *delay = CCDelayTime::create(0.5f);
				CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(ItemMushroom::die));
				CCSequence *delayDie = CCSequence::create(delay, call, nullptr);
				this->runAction(delayDie);

				//! mario 小跳
				_mario->_speedUp = 150;
				_mario->_speedDown = 10;
				_mario->_status = Mario::FLY;
				_mario->updateStatus();
			}
		}
		else
		{
			_mario->die(false);
		}
	}
}