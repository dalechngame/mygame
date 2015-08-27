#ifndef __ItemFlyFish_H__
#define __ItemFlyFish_H__

#include "Item.h"
class ItemFlyFish : public Item
{
public:
	static ItemFlyFish* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	int _duration;
	int _offsetH;
	int _offsetV;

	bool _alreadyFly;

	void updateStatus();

	void move(float dt);
	void collision();
};

#endif