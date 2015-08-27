#ifndef __ItemMushroom_H__
#define __ItemMushroom_H__

#include "Item.h"
class ItemMushroom : public Item
{
public:
	static ItemMushroom* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	bool _isDead;

	void die();
	void move(float dt);
	void collision();
};

#endif