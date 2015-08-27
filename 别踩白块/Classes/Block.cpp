#include "Block.h"
#include "AppMacros.h"

CCArray * Block::array = NULL;

Block *Block::create(CCSize size, ccColor3B color, 
					 CCString str, float strSize, ccColor3B strColor)
{
	if (array == NULL)
	{
		array = CCArray::create();
		//不走渲染树这条路需要手动retain
		array->retain();
	}
	Block *pRet = new Block;
	if (pRet && pRet->init(size, color, str, strSize, strColor))
	{
		pRet->autorelease();
		//每次创建都往array里面添加
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

	//block上的字符
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
	//核心步骤
	_lineIndex--;
	CCMoveTo *to = CCMoveTo::create(
			0.001,
			ccp(getPositionX(), getPositionY()-winSize.height/4)
		);

	this->runAction(to);
	if (_lineIndex < 0)
	{
		//顺序不能换因为一帧后对象消失
		array->removeObject(this);
		removeFromParentAndCleanup(true);
	}




}


