#ifndef __ItemMushroomReward_H__
#define __ItemMushroomReward_H__

#include "itemaddmushroom.h"
class ItemMushroomReward : public ItemAddMushroom
{
public:
	static ItemMushroomReward* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	enum{ SLEEP, GROW, MOVE } _status;

	void wakeup();

	void move(float dt);
};

#endif