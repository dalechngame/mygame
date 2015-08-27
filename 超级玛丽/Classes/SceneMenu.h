
#ifndef __SceneMenu_H__
#define __SceneMenu_H__
#include "Common.h"

class SceneMenu : public CCLayer
{
public:
	CREATE_FUNC(SceneMenu);
	bool init();

	void Back(CCObject*);
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	bool isClick(CCTouch* pTouch);

	CCScrollView* _view;
};

#endif