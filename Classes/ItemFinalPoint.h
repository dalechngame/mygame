#ifndef __ItemFinalPoint_H__
#define __ItemFinalPoint_H__

#include "Item.h"
class ItemFinalPoint : public Item
{
public:
	static ItemFinalPoint* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	void collision();
};

#endif