#ifndef __ItemMushroomAddLife_H__
#define __ItemMushroomAddLife_H__

#include "itemaddmushroom.h"
class ItemMushroomAddLife : public ItemAddMushroom
{
public:
	static ItemMushroomAddLife* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

};

#endif