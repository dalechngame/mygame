#include "LayerBullet.h"  			
#include "AppMacros.h"		
#include "Plane.h"
	
bool LayerBullet::init() 			
{ 						
	CCLayer::init(); 			

	m_pbulletArray = CCArray::create();
	CC_SAFE_RETAIN(m_pbulletArray);

	//! batchnode 的构建必须是全部的图片集合
	m_pbulletBatchNode = CCSpriteBatchNode::create("ui/shoot.png");
	addChild(m_pbulletBatchNode);

	startShoot();

	return true;				
} 						

void LayerBullet::startShoot()
{
	schedule(schedule_selector(LayerBullet::addBulletCallback), 0.1f);
}
void LayerBullet::stopShoot()
{
	unschedule(schedule_selector(LayerBullet::addBulletCallback));
}

void LayerBullet::addBulletCallback(float dt)
{
	CCSprite *hero = Plane::getInstance();
	//! 在飞机头产生子弹
	CCPoint birthPlace = hero->getPosition() + ccp(0, hero->getContentSize().height /2 +2);
	CCSprite * bullet = CCSprite::createWithSpriteFrameName("bullet1.png");
	bullet->setPosition(birthPlace);

	m_pbulletBatchNode->addChild(bullet);
	m_pbulletArray->addObject(bullet);

	//! 产生运动的时间
	float distance = winSize.height - hero->getPositionY() - hero->getContentSize().height / 2;
	float velocity = 800 / 1;
	float movedt = distance / velocity;

	//! 移动到屏幕外
	CCMoveTo *to = CCMoveTo::create(movedt, ccp(birthPlace.x, winSize.height + bullet->getContentSize().height/2));
	CCCallFuncN * actionDone = CCCallFuncN::create(this, callfuncN_selector(LayerBullet::bulletMoveFinished));

	CCSequence * sequence = CCSequence::create(to, actionDone, NULL);

	bullet->runAction(sequence);

}

void LayerBullet::bulletMoveFinished(CCNode *node)
{
	CCSprite *bullet = (CCSprite  *)node;
	m_pbulletBatchNode->removeChild(bullet, true);
	m_pbulletArray->removeObject(bullet);
}

//! 为炸弹准备
void LayerBullet::removeBullet(CCSprite* bullet)
{
	m_pbulletBatchNode->removeChild(bullet, true);
	m_pbulletArray->removeObject(bullet);
}