#include "Stone.h"
#include <AppMacros.h>

//! 棋子位置的偏移

int Stone::_d = 0;
int Stone::_offx = 0;
int Stone::_offy =0;


bool Stone::init()
{
	CCSprite::init();

	//! 默认为true 
	_dead = false;
	return true;
}

void Stone::initStone(int id)
{

	_id = id;
	_red = id < 16; ///> id<16就是红色方
	
	//! 设置属性
	setProper(id);

	//! 获得纹理图片
	CCTexture2D* texture;
	switch (_proper._type)
	{
	case ROOK:
		if (_red)
			texture = CCTextureCache::sharedTextureCache()->addImage("rche.png");
		else
			texture = CCTextureCache::sharedTextureCache()->addImage("bche.png");
		break;
	case HORSE:
		if (_red)
			texture = CCTextureCache::sharedTextureCache()->addImage("rma.png");
		else
			texture = CCTextureCache::sharedTextureCache()->addImage("bma.png");
		break;
	case CANNON:
		if (_red)
			texture = CCTextureCache::sharedTextureCache()->addImage("rpao.png");
		else
			texture = CCTextureCache::sharedTextureCache()->addImage("bpao.png");
		break;
	case PAWN:
		if (_red)
			texture = CCTextureCache::sharedTextureCache()->addImage("rbing.png");
		else
			texture = CCTextureCache::sharedTextureCache()->addImage("bzu.png");
		break;
	case KING:
		if (_red)
			texture = CCTextureCache::sharedTextureCache()->addImage("rshuai.png");
		else
			texture = CCTextureCache::sharedTextureCache()->addImage("bjiang.png");
		break;
	case GUARD:
		if (_red)
			texture = CCTextureCache::sharedTextureCache()->addImage("rshi.png");
		else
			texture = CCTextureCache::sharedTextureCache()->addImage("bshi.png");
		break;
	case ELEPH:
		if (_red)
			texture = CCTextureCache::sharedTextureCache()->addImage("rxiang.png");
		else
			texture = CCTextureCache::sharedTextureCache()->addImage("bxiang.png");
		break;
	}

	//! 显示纹理
	setTexture(texture);
	setTextureRect(CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height));

	//! 设置位置
	setPosition(fromPlate());
	setScale(0.6f);
}

CCPoint Stone::fromPlate()
{
	int x = _proper._col *_d + _offx;
	int y = _proper._row *_d + _offy;


	return ccp(x, y);
}

void Stone::setProper(const int &id)
{

	Proper proper[9] = {
		{ ROOK, 0, 0 },	//id = 0
		{ HORSE, 0, 1 },	//1
		{ ELEPH, 0, 2 },	//2
		{ GUARD, 0, 3 },	//3
		{ PAWN, 3, 2 },	//4
		{ PAWN, 3, 0 },	//5
		{ CANNON, 2, 1 },	//6

		{ KING, 0, 4 }	,//7
		{ PAWN, 3, 4 },//8
	};
	
	if (id <= 8)
	{
		this->_proper._type = proper[id]._type;
		this->_proper._col = proper[id]._col;
		this->_proper._row = proper[id]._row;
	}
	else if (id > 8 && id < 16)
	{
		//! y轴对称翻转
		int index = id - 9;
		this->_proper._type = proper[index]._type;
		this->_proper._col  = 8 - proper[index]._col;
		this->_proper._row = proper[index]._row;
	}
	else if (id >= 16)
	{
		//! 圆心旋转180°
		int index = id - 16;
		if (index <= 8)
		{
			this->_proper._type = proper[index]._type;
			this->_proper._col = 8 - proper[index]._col;
			this->_proper._row = 9 - proper[index]._row;
		}
		else
		{
			//! x轴对称翻转
			index -= 9;
			this->_proper._type = proper[index]._type;
			this->_proper._col = proper[index]._col;
			this->_proper._row = 9 - proper[index]._row;
		}
	}
}
