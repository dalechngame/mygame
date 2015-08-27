#include "Plane.h"
#include "LayerGameOver.h"
#include "LayerGameMain.h"
#include "LayerBullet.h"
#include "LayerControl.h"

Plane* Plane::_splane = NULL;

Plane* Plane::getInstance()
{
	if (!_splane)
	{
		_splane = new Plane;
		if (_splane && _splane->init())
		{
		}
	}
	return _splane;
}

bool  Plane::init()
{
	CCSprite::init();
	// 用帧动画的帧来创建精灵
	this->initWithSpriteFrameName("hero1.png");
	life = 3;

	CCBlink *blink = CCBlink::create(1, 3);
	CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(Plane::animatePlane));
	CCSequence *sequence = CCSequence::create(blink, callFunc, NULL);
	runAction(sequence);

	//暴炸动画缓存
	CCAnimation * blowup = CCAnimation::create();
	blowup->setDelayPerUnit(0.2f);
	blowup->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n1.png"));
	blowup->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n2.png"));
	blowup->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n3.png"));

	//飞行动画缓存
	CCAnimation *fly = CCAnimation::create();
	fly->setDelayPerUnit(0.2f);
	fly->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero1.png"));
	fly->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero2.png"));

	CCAnimationCache::sharedAnimationCache()->addAnimation(blowup, "PlaneBlowUp");
	CCAnimationCache::sharedAnimationCache()->addAnimation(fly, "PlaneFly");

	return true;
}

void  Plane::animatePlane()
{

	CCAnimate * animate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("PlaneFly"));

	this->runAction(CCRepeatForever::create(animate));

}

void  Plane::blowUp()
{
	stopAllActions();
	CCAnimate * animate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("PlaneBlowUp"));
	CCCallFunc * actionDone = CCCallFunc::create(this, callfunc_selector(Plane::removePlane));
	CCBlink *blink = CCBlink::create(1.0f, 3);
	CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(Plane::animatePlane));
	CCCallFunc *update = CCCallFunc::create(this, callfunc_selector(Plane::planeUpdate));
	CCSequence *sequence = CCSequence::create(animate, actionDone, blink, callFunc, update, NULL);

	this->runAction(sequence);
}
void Plane::planeUpdate()
{
	CCLayer *LayerGame = ((CCLayer*)this->getParent());

	LayerGame->scheduleUpdate();
}

void Plane::removePlane()
{
	life--;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	LayerGameMain *LayerGame = ((LayerGameMain*)this->getParent());
	LayerBullet *bullet = LayerGame->m_pbulletLayer;
	LayerControl *ctrl = LayerGame->m_pctrlLayer;

	this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero1.png"));
	this->setPosition(ccp(winSize.width / 2, this->getContentSize().height / 2));
	this->setZOrder(200);
	bullet->startShoot();
	ctrl->updateHeroLife(life);
	LayerGame->setTouchEnabled(true);

	if (life < 0)
	{
		CCDirector::sharedDirector()->replaceScene(LayerGameOver::scene());
	}
}
