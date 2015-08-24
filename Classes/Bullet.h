#ifndef __Bullet_H__  		
#define __Bullet_H__  		

#include "Item.h"

class Bullet :public Item 
{  					
public: 					
	static Bullet* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	void updateStatus();
	bool canMoveUp(float dt);
	void moveUp(float dt);
	void Up(float);
	void Down(float);
	void bomb();

	void collision();
	void move(float dt);


	int _speedUp;
	
	enum{NORMAL, BOMB} _status;
};  					
#endif  					
