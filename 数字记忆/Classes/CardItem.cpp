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

	//���þ����С
	setContentSize(CCSizeMake(80, 80));
	//���þ���ê��
	setAnchorPoint(ccp(0, 0));

	//Ϊ�����������
	CCString *str = CCString::createWithFormat("%d", idx);
	//������������
	ttf = CCLabelTTF::create(str->getCString(), "Courier New", 50);
	//��������λ��   ����ھ���
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