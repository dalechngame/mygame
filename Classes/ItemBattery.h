#ifndef __ItemBattery_H__
#define __ItemBattery_H__

#include "Item.h"
class ItemBattery : public Item
{
public:
	static ItemBattery* create(CCDictionary* dict);
	bool init(CCDictionary* dict);
};

#endif