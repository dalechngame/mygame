#ifndef __ItemLadderUD_H__
#define __ItemLadderUD_H__

#include "Item.h"
class ItemLadderUD : public Item
{
public:
	static ItemLadderUD* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	int _UorD;
	int _ladderDis;
	float _dis;
	bool _marioOnThisLadder;

	void move(float dt);
	void collision();
};

#endif