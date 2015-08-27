#ifndef __ItemFlagPoint_H__
#define __ItemFlagPoint_H__

#include "Item.h"
class ItemFlagPoint : public Item
{
public:
	static ItemFlagPoint* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	void collision();
};

#endif