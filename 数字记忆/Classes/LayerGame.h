#ifndef __LayerGame_H__
#define __LayerGame_H__

#include "cocos2d.h"
#include "vector"
using namespace  std;

USING_NS_CC;
class LayerGame:public CCLayer
{
public:
	static CCScene * scene(int degree);
	static LayerGame * create(int degree);
	bool init(int degree);

	void getAllPoint();
	void getAvailablePoint();
	void addCard();
	void showCardAllWhite();

	CCPointArray * allPoints;
	CCArray      * allCards;
	vector<int> vec;

	CC_SYNTHESIZE(int, _degere, Degree);
	CC_SYNTHESIZE(int, _clickStart, ClickStart);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);



};

#endif




