#ifndef __ItemAddMushroom_H__
#define __ItemAddMushroom_H__

#include "Item.h"
class ItemAddMushroom : public Item
{
public:
	static ItemAddMushroom* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	enum{ SLEEP, GROW, MOVE } _status;

	void wakeup();
	void afterGrowFuncCall();
	void updateStatus();

	void move(float dt);
	void collision();
};

#endif