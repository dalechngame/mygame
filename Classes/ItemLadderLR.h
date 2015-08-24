#ifndef __ItemLadderLR_H__
#define __ItemLadderLR_H__

#include "Item.h"
class ItemLadderLR : public Item
{
public:
	static ItemLadderLR* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	int _LorR;
	int _ladderDis;
	float _dis;
	bool _marioOnThisLadder;

	void move(float dt);
	void collision();
};

#endif