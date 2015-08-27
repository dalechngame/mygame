#ifndef __ItemTortoiseRound_H__
#define __ItemTortoiseRound_H__

#include "Item.h"
class ItemTortoiseRound : public Item
{
public:
	static ItemTortoiseRound* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	enum{NORMAL, SLEEP, CRAZY} _status;

	int _roundDis;
	int _leftMost;
	int _rightMost;

	void updateStatus();
	void move(float dt);
	void collision();

	void Recover(float);
};

#endif