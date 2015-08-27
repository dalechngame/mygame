#include "LayerEnemy.h"  			
#include "AppMacros.h"			


//! 在此层中管理enemy的运动 爆炸 移除 和添加
bool LayerEnemy::init() 			
{ 						
	CCLayer::init(); 			

	m_psmallArray = CCArray::create();
	CC_SAFE_RETAIN(m_psmallArray);
	m_psmallArray->retain();
	m_psmallArray->autorelease();

	//! 缓存爆炸动画
	CCAnimation *smallAnimation = CCAnimation::create();
	smallAnimation->setDelayPerUnit(0.1f);
	char nameBuf[100];
	for (int i = 0; i < 4; i++)
	{
		memset(nameBuf, 0, sizeof(nameBuf));
		sprintf(nameBuf, "enemy1_down%d.png", i + 1);
		smallAnimation->addSpriteFrame
			(CCSpriteFrameCache::sharedSpriteFrameCache()
			->spriteFrameByName(nameBuf));
	}
	CCAnimationCache::sharedAnimationCache()->addAnimation(smallAnimation, "SmallBlowUp");

	schedule(schedule_selector(LayerEnemy::addSmallEnemy), 0.5f);

	return true;				
} 						

void LayerEnemy::addSmallEnemy(float dt)
{
	Enemy *smallPlane = Enemy::create();
	smallPlane->bindEnemySprite(CCSprite::createWithSpriteFrameName("enemy1.png"), SMALL_MAXLIFE);
	m_psmallArray->addObject(smallPlane);
	addChild(smallPlane);

	//! 随机产生0.0~1.0的数
	float x = CCRANDOM_0_1()*(winSize.width - smallPlane->getBoundingBox().size.width) + smallPlane->getBoundingBox().size.width /2;
	float y = winSize.height + smallPlane->boundingBox().size.height / 2;

	CCPoint smallBirth = ccp(x, y);
	smallPlane->setPosition(smallBirth);
	CCMoveTo *to = CCMoveTo::create(3, ccp(
			smallBirth.x, 
			smallBirth.y - winSize.height - smallPlane->getBoundingBox().size.height /2
		));

	CCCallFuncN *actionDone = CCCallFuncN::create(this, callfuncN_selector(LayerEnemy::smallEnemyMoveFinishedFCallBack));
	CCSequence *sequence = CCSequence::create(to, actionDone, NULL);

	smallPlane->runAction(sequence);
//CCLog("%p", smallPlane);
//CCLog("before count  = %d", m_psmallArray->count());
}

void LayerEnemy:: smallEnemyMoveFinishedFCallBack(CCNode *node)
{
	//CCSprite * smallEnemy = (CCSprite *)node;
	m_psmallArray->removeObject(node);
	node->removeFromParentAndCleanup(true);

//CCLog("after count  = %d", m_psmallArray->count());
}

void LayerEnemy::smallEnemyBlowUp(Enemy *smallEnemy)
{
	CCAnimate *smallAnimate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("SmallBlowUp"));
	CCCallFuncND *actionDone = CCCallFuncND::create(this, callfuncND_selector(LayerEnemy::removeSmallEnemyFCallBack), (void*)smallEnemy);
	CCSequence *sequence = CCSequence::create(smallAnimate, actionDone, NULL);
	smallEnemy->getSprite()->runAction(sequence);
//CCLog("smallEnemy = %p , smallSprite = %p", smallEnemy, smallEnemy->getSprite());
}

//! 传入enemy和enemy中的sprite
void LayerEnemy::removeSmallEnemyFCallBack(CCNode *target, void *data)
{

	Enemy *smallEnemy = (Enemy *)data;
	if (smallEnemy)
	{
		m_psmallArray->removeObject(smallEnemy);
		smallEnemy->removeFromParentAndCleanup(true);
	}
}


void LayerEnemy::removeAllSmallEnemy()
{
	CCObject *obj;
	CCARRAY_FOREACH(m_psmallArray, obj)
	{
		Enemy *enemy = (Enemy *)obj;
		if (enemy->getLife() >= 1)
		{
			smallEnemyBlowUp(enemy);
		}
	}
}

void LayerEnemy::removeAllEnemy()
{
	removeAllSmallEnemy();
}