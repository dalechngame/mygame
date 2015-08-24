#ifndef __ItemTortoise_H__
#define __ItemTortoise_H__

#include "Item.h"

class ItemTortoise : public Item
{
public:

	static ItemTortoise* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	void move(float dt);
	void collision();

	void revive(float dt);
	bool marioUpHit(CCRect rcMario, CCRect rcItem, bool coll);
	void die();

	enum { NORMAL, SLEEP, CRAZY, DEAD} _status;

};

#endif