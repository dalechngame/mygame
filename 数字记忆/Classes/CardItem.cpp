#include "CardItem.h"

CardItem *CardItem::create(int idx)
{
	CardItem *pRet = new CardItem;

	if (pRet && pRet->init(idx))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	
	return pRet;
}

bool CardItem::init(int idx)
{
	CCSprite::init();
	setIdx(idx);

	//设置精灵大小
	setContentSize(CCSizeMake(80, 80));
	//设置精灵锚点
	setAnchorPoint(ccp(0, 0));

	//为精灵添加字体
	CCString *str = CCString::createWithFormat("%d", idx);
	//设置字体类型
	ttf = CCLabelTTF::create(str->getCString(), "Courier New", 50);
	//设置字体位置   相对于精灵
	ttf->setPosition(ccp(40, 40));
	this->addChild(ttf);

	bg = CCSprite::create();
	bg->setTextureRect(CCRectMake(0, 0, 77, 77));
	bg->setAnchorPoint(ccp(0, 0));
	bg->setColor(ccc3(255, 255, 255));

	this->addChild(bg);

	return true;
}

void CardItem::showWhite()
{
	ttf->setVisible(false);
	bg->setVisible(true);
}

void CardItem::showText()
{
	ttf->setVisible(true);
	bg->setVisible(false);
}