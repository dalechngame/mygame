#include "LayerGameMain.h"  			
#include "AppMacros.h"		
#include "Plane.h"
#include "LayerGameOver.h"

int LayerGameMain::score = 0;
CCScene * LayerGameMain::scene()
{
	CCScene * scene = CCScene::create();
	LayerGameMain * layer = LayerGameMain::create();
	scene->addChild(layer);
	return scene;
}

bool LayerGameMain::init()
{
	CCLayer::init();

	scheduleUpdate();
	setTouchMode(kCCTouchesOneByOne);
	setTouchEnabled(true);


	//! 初始化炸弹个数
	bigBoomCount = 0;

	addBackGround();
	addHero();
	addBulletLayer();
	addEnemyLayer();
	addCtrlLayer();
	addFoodLayer();


	return true;
}

void LayerGameMain::addBackGround()
{
	CCSprite *bg1 = CCSprite::createWithSpriteFrameName("background.png");
	bg1->setTag(BACK1);
	bg1->setAnchorPoint(CCPointZero);
	bg1->setPosition(CCPointZero);
	addChild(bg1);


	CCSprite * bg2 = CCSprite::createWithSpriteFrameName("background.png");
	bg2->setTag(BACK2);
	bg2->setAnchorPoint(ccp(0, 0));
	//! -2作为微调
	bg2->setPosition(ccp(0, bg2->getContentSize().height - 2));
	addChild(bg2);

	schedule(schedule_selector(LayerGameMain::movingBackGround), 0.01f);

}
void LayerGameMain::movingBackGround(float dt)
{
	CCSprite *bg1 = (CCSprite *)getChildByTag(BACK1);
	CCSprite *bg2 = (CCSprite *)getChildByTag(BACK2);

	bg1->setPositionY(bg1->getPositionY() - 2);
	bg2->setPositionY(bg1->getPositionY() + bg2->getContentSize().height - 2);

	if (bg2->getPositionY() < 0)
	{
		bg1->setPositionY(0);
	}
}

void LayerGameMain::addHero()
{
	Plane *hero = Plane::getInstance();
	hero->setPosition(ccp(winSize.width / 2, hero->getContentSize().height / 2));
	addChild(hero);
}

void LayerGameMain::addBulletLayer()
{
	m_pbulletLayer = LayerBullet::create();
	addChild(m_pbulletLayer);
}

void LayerGameMain::addEnemyLayer()
{
	m_penemyLayer = LayerEnemy::create();
	addChild(m_penemyLayer);
}


void LayerGameMain::addCtrlLayer()
{
	m_pctrlLayer = LayerControl::create();
	addChild(m_pctrlLayer);
}


void LayerGameMain::addFoodLayer()
{
	m_pfoodLayer = LayerFood::create();
	addChild(m_pfoodLayer);
}

bool LayerGameMain::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCRect planeRect = Plane::getInstance()->boundingBox();
	//! 扩大触摸范围
	planeRect.origin.x -= 30;
	planeRect.origin.y -= 40;
	planeRect.size.width += 60;
	planeRect.size.height += 80;
	if (planeRect.containsPoint(pTouch->getLocation()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void LayerGameMain::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	Plane *hero = Plane::getInstance();
	CCPoint position = hero->getPosition() + pTouch->getDelta();

	if (position.x < hero->boundingBox().size.width / 2 ||
		position.x > winSize.width - hero->boundingBox().size.width / 2 ||
		position.y <hero->boundingBox().size.height / 2 ||
		position.y > winSize.height - hero->boundingBox().size.height / 2)
	{
		return;
	}
	else
	{
		Plane::getInstance()->setPosition(position);
	}
}

//! 碰撞检测
void LayerGameMain::update(float dt)
{

	//! bullet vs enemysmall 
	CCObject * bullet = NULL;
	CCObject * enemy = NULL;
	CCArray  *bulletsToDel = CCArray::create();
	CCArray  * smallEnemyToDel = CCArray::create();

	//! 取出每一个子弹
	CCARRAY_FOREACH(m_pbulletLayer->m_pbulletArray, bullet)
	{
		CCSprite * tbullet = (CCSprite*)bullet;
		//! 取出每一个敌机
		CCARRAY_FOREACH(m_penemyLayer->m_psmallArray, enemy)
		{
			Enemy * smallEnemy = (Enemy *)enemy;
			if (tbullet->boundingBox().intersectsRect(smallEnemy->getBoundingBox()))
			{

				//CCLog("%d", smallEnemy->getLife());
				if (smallEnemy->getLife() == 1)
				{
					smallEnemy->loseLife();
					bulletsToDel->addObject(tbullet);
					smallEnemyToDel->addObject(smallEnemy);
					//! 更新分数
					score += SMALL_SCORE;
					m_pctrlLayer->updateScore(score);
				}
			}
		}
		//! 销毁敌机
		CCARRAY_FOREACH(smallEnemyToDel, enemy)
		{
			Enemy * smallEnemy = (Enemy*)enemy;
			m_penemyLayer->smallEnemyBlowUp(smallEnemy);
		}
	}
	//! 销毁子弹
	CCARRAY_FOREACH(bulletsToDel, bullet)
	{
		CCSprite * tbullet = (CCSprite*)bullet;
		m_pbulletLayer->removeBullet(tbullet);
	}
	bulletsToDel->release();
	smallEnemyToDel->release();


	//! hero vs enemySmall
	//! 缩小hero的尺寸
	CCRect planeRect = Plane::getInstance()->boundingBox();
	planeRect.origin.x += 30;
	planeRect.origin.y += 20;
	planeRect.size.width -= 60;
	planeRect.size.height -= 40;

	CCARRAY_FOREACH(m_penemyLayer->m_psmallArray, enemy)
	{
		Enemy  * smallEnemy = (Enemy  *)enemy;
		if (planeRect.intersectsRect(smallEnemy->getBoundingBox()))
		{
			m_pbulletLayer->stopShoot();
			unscheduleAllSelectors();
			setTouchEnabled(false);
			Plane::getInstance()->blowUp();
			m_penemyLayer->smallEnemyBlowUp(smallEnemy);
		}
	}


	//! food vs hero
	CCObject * food;

	CCARRAY_FOREACH(m_pfoodLayer->m_pbigBoomArray, food)
	{
		CCSprite * bigBoom = (CCSprite*)food;
		if (bigBoom->boundingBox().intersectsRect(Plane::getInstance()->boundingBox()))
		{
			m_pfoodLayer->removeBigBoom(bigBoom);
			bigBoomCount++;

			updateBigBoomCount(bigBoomCount);
		}
	}

}

void  LayerGameMain::updateBigBoomCount(int bigBoomCount)
{
	//! 显示炸弹个数
	CCString strBoomCount = "";
	if (bigBoomCount < 0)
	{
		return;
	}
	//! 当炸弹个数为0的时候删除图片
	else if (bigBoomCount == 0)
	{
		if (getChildByTag(TAG_BIGBOOM))
		{
			removeChildByTag(TAG_BIGBOOM, true);
		}
		if (getChildByTag(TAG_BIGBOOMCOUNT))
		{
			removeChildByTag(TAG_BIGBOOMCOUNT, true);
		}
	}
	else if (bigBoomCount == 1)
	{
		CCSprite * norBoom = CCSprite::createWithSpriteFrameName("bomb.png");
		//! TAG_BIGBOOM = 1000
		if (!getChildByTag(TAG_BIGBOOM))
		{
			//! 必须要有两张图片一张正常一张按下
			CCSprite * selBoom = CCSprite::createWithSpriteFrameName("bomb.png");
			CCMenuItemSprite * boomItem = CCMenuItemSprite::create(norBoom, selBoom, this, menu_selector(LayerGameMain::boomMenuCallBack));
			boomItem->setPosition(ccp(norBoom->getContentSize().width / 2, norBoom->getContentSize().height / 2));

			CCMenu * boomMenu = CCMenu::create(boomItem, NULL);
			boomMenu->setPosition(CCPointZero);
			addChild(boomMenu, 100, TAG_BIGBOOM);
		}
		//! 		//! TAG_BIGBOOM =2000
		if (!getChildByTag(TAG_BIGBOOMCOUNT))
		{
			strBoomCount.initWithFormat("X%d", bigBoomCount);
			CCLabelBMFont * labelBoomCount = CCLabelBMFont::create(strBoomCount.getCString(), "font/font.fnt");

			labelBoomCount->setAnchorPoint(ccp(0, 0.5));
			labelBoomCount->setPosition(ccp(norBoom->getContentSize().width + 15, norBoom->getContentSize().height / 2));
			addChild(labelBoomCount, 200, TAG_BIGBOOMCOUNT);
		}
	}
	else if (bigBoomCount > 1)
	{
		strBoomCount.initWithFormat("X%d", bigBoomCount);
		((CCLabelBMFont*)(getChildByTag(TAG_BIGBOOMCOUNT)))->setCString(strBoomCount.getCString());
	}

}


void LayerGameMain::boomMenuCallBack(CCObject * obj)
{
	//! 更新炸弹个数
	bigBoomCount--;
	CCString *strBoomCount = CCString::createWithFormat("X%d", bigBoomCount);
	((CCLabelBMFont*)(getChildByTag(TAG_BIGBOOMCOUNT)))->setCString(strBoomCount->getCString());

	//! 更新分数
	score += m_penemyLayer->m_psmallArray->count() * SMALL_SCORE;
	m_pctrlLayer->updateScore(score);

	//!销毁敌机
	m_penemyLayer->removeAllEnemy();
	if (bigBoomCount == 0)
	{
		removeChildByTag(TAG_BIGBOOM);
		removeChildByTag(TAG_BIGBOOMCOUNT);
	}
}