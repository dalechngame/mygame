#ifndef __P014_MAIN_H__
#define __P014_MAIN_H__

#include "PBase.h"

USING_NS_CC;

class P014_Main : public PBase
{
protected:

	virtual void initSceneJson(const char* jsonFileName) override;

	virtual GameNumber getGameNumber() override;

	bool onSceneJsonWillMoveTo(Node *node, WJTouchEvent *event);
	void onSceneJsonTouchEnded(Node *node, WJTouchEvent *e);

public:

	P014_Main();
	virtual ~P014_Main();
	virtual bool init() override;

	virtual void onEnterTransitionDidFinish() override;

	virtual void onExit() override;

public:
	CREATE_SCENE_FUNC(P014_Main);
};

#endif // __P008_NAIL_H__
