#include "Block.h"
#include "AppMacros.h"

CCArray * Block::array = NULL;

Block *Block::create(CCSize size, ccColor3B color, 
					 CCString str, float strSize, ccColor3B strColor)
{
	if (array == NULL)
	{
		array = CCArray::create();
		//������Ⱦ������·��Ҫ�ֶ�retain
		array->retain();
	}
	Block *pRet = new Block;
	if (pRet && pRet->init(size, color, str, strSize, strColor))
	{
		pRet->autorelease();
		//ÿ�δ�������array�������
		array->addObject(pRet);
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}

	return pRet;
}

bool Block::init(CCSize size, ccColor3B color, 
		  CCString str, float strSize, ccColor3B strColor)
{
	CCSprite::init();

	setContentSize(size);
	setTextureRect(CCRectMake(0, 0, size.width, size.height));
	setColor(color);
	setAnchorPoint(ccp(0, 0));

	//block�ϵ��ַ�
	CCLabelTTF *label = CCLabelTTF::create();
	label->setString(str.getCString());
	label->setFontSize(strSize);
	label->setColor(strColor);
	label->setPosition(ccp(size.width/ 2, size.height /2));
	addChild(label);

	return true;
}

CCArray *Block::getBlocksArray()
{
	return array;
}

void Block::moveDownAndCleanUp()
{
	//���Ĳ���
	_lineIndex--;
	CCMoveTo *to = CCMoveTo::create(
			0.001,
			ccp(getPositionX(), getPositionY()-winSize.height/4)
		);

	this->runAction(to);
	if (_lineIndex < 0)
	{
		//˳���ܻ���Ϊһ֡�������ʧ
		array->removeObject(this);
		removeFromParentAndCleanup(true);
	}




}


