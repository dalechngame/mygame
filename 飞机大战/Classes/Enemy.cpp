#include "Enemy.h"  			
#include "AppMacros.h"		

//! �ڴ˲��й��� enemy�İ� sprite������ֵ ����
Enemy::Enemy()
{
	m_psprite = NULL;
	m_nlife = 0;
}
Enemy::~Enemy()
{

}

Enemy *Enemy::create()
{
	Enemy *pRect = new Enemy();
	pRect->autorelease();
	return pRect;
}

void Enemy::bindEnemySprite(CCSprite *sp, int life)
{
	m_psprite = sp;
	m_nlife = life;
	addChild(m_psprite);
}

CCSprite *Enemy::getSprite()
{
	return m_psprite;
}

int Enemy::getLife()
{
	return m_nlife;
}

void Enemy::loseLife()
{
	m_nlife--;
}

//! ��Ҫ�ǽ�boundingbox������תΪ��������
CCRect Enemy::getBoundingBox()
{
	CCRect rect = m_psprite->boundingBox();
	//CCLOG("x= %f, y = %f", rect.origin.x, rect.origin.y);
	CCPoint pos = this->convertToWorldSpace(rect.origin);
	CCLOG("x= %f, y = %f", pos.x, pos.y);

	CCRect enemyRect = CCRect(pos.x, pos.y, rect.size.width, rect.size.height);

	return enemyRect;
}
