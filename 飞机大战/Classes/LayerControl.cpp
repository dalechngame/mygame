#include "LayerControl.h"  			
#include "AppMacros.h"			
#include "Plane.h"

bool LayerControl::init() 			
{ 						
	CCLayer::init(); 			


	CCSprite * nor = CCSprite::createWithSpriteFrameName("game_pause_nor.png");
	CCSprite * press = CCSprite::createWithSpriteFrameName("game_pause_pressed.png");

	//! 暂停按钮
	m_ppauseMenuItem = CCMenuItemSprite::create(nor, press, this, menu_selector(LayerControl::menuCallBack));
	//! 放在左下角
	CCPoint menuBirth = ccp(winSize.width - m_ppauseMenuItem->getContentSize().width / 2, m_ppauseMenuItem->getContentSize().height / 2);
	m_ppauseMenuItem->setPosition(menuBirth);

	CCMenu *pauseMenu = CCMenu::create(m_ppauseMenuItem, NULL);
	pauseMenu->setPosition(CCPointZero);

	//! 计分板
	m_pscoreItem = CCLabelBMFont::create("0", "font/font.fnt");
	m_pscoreItem->setColor(ccc3(143, 146, 147));
	m_pscoreItem->setPosition(ccp(winSize.width /2, winSize.height - m_pscoreItem->getContentSize().height));

	//! hero生命值
	const char *life = CCString::createWithFormat("%d", Plane::getInstance()->getLife())->getCString();
	m_pheroLife = CCLabelBMFont::create(life, "font/font.fnt");
	m_pheroLife->setColor(ccc3(143, 146, 147));
	m_pheroLife->setAnchorPoint(ccp(0, 0.5));
	m_pheroLife->setPosition(ccp(winSize.width - m_pscoreItem->getContentSize().width, winSize.height - m_pscoreItem->getContentSize().height));

	addChild(pauseMenu);
	addChild(m_pscoreItem);
	addChild(m_pheroLife);

	return true;
}

void LayerControl::menuCallBack(CCObject *obj)
{
	if (!CCDirector::sharedDirector()->isPaused())
	{
		m_ppauseMenuItem->setNormalImage(CCSprite::createWithSpriteFrameName("game_resume_nor.png"));
		m_ppauseMenuItem->setSelectedImage(CCSprite::createWithSpriteFrameName("game_resume_pressed.png"));

		CCDirector::sharedDirector()->pause();
		//! Plane停止移动
		((CCLayer*)(Plane::getInstance()->getParent()))->setTouchEnabled(false);
	}
	else
	{
		m_ppauseMenuItem->setNormalImage(CCSprite::createWithSpriteFrameName("game_pause_nor.png"));
		m_ppauseMenuItem->setSelectedImage(CCSprite::createWithSpriteFrameName("game_pause_pressed.png"));

		CCDirector::sharedDirector()->resume();
		//! Plane恢复移动
		((CCLayer*)(Plane::getInstance()->getParent()))->setTouchEnabled(true);
	}
}

void LayerControl::updateScore(int score)
{
	CCString * strScore = CCString::createWithFormat("%d", score);
	m_pscoreItem->setCString(strScore->getCString());
}

void LayerControl::updateHeroLife(int life)
{
	CCString * strScore = CCString::createWithFormat("%d", life);
	m_pheroLife->setCString(strScore->getCString());
}