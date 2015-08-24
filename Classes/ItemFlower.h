#ifndef __ItemFlower_H__
#define __ItemFlower_H__

#include "Item.h"
class ItemFlower : public Item
{
public:
	static ItemFlower* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	bool _bShow; // 表现状态，就是花全部显示出来之后的状态
	int _yTop;
	int _yButtom;

	void updateStatus();
	void move(float dt);
	void collision();

	void unsetShow(float dt)
	{
		_bShow = false;
		_dir = Common::DOWN;
	}
};

#endif