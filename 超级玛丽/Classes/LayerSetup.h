#ifndef __LayerSetup_H__
#define __LayerSetup_H__

#include "Common.h"
class LayerGame;
class LayerSetup : public CCLayer
{
public:
	CREATE_FUNC(LayerSetup);
	bool init();

	void setBG(CCObject*);
	void setEffect(CCObject*);
	void backGame(CCObject*, CCControlEvent);
	void backHome(CCObject*, CCControlEvent);
	void resumeGame();
	void restartGame();

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	CCControlButton *_back;
	CCControlButton *_home;

	CCMenuItemToggle *_bg;
	CCMenuItemToggle *_effect;
	LayerGame *game;

};

#endif

