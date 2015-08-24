#ifndef __ItemBoss_H__
#define __ItemBoss_H__

#include "Item.h"
class ItemBoss : public Item
{
public:
	static ItemBoss* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	Common::DIRECTION _dir;

	enum{ NORMAL, DROPPING } _status;
	int _left;
	int _right;

	bool _bCanJump;
	void setAccJumpTrue(){ _bCanJump = true; }

	void updateStatus();
	void move(float dt);
};

#endif