#ifndef __P009_LINE_H__
#define __P009_LINE_H__

#include "PBase.h"
#include "ScrollLayer.h"

USING_NS_CC;

class P009 : public PBase
{
protected:

	void initEvent();
	void initLineLayer();

	virtual GameNumber getGameNumber() override;


	bool m_startDrawing;
	bool onSceneLayerTouchAble(Node *node, WJTouchEvent *event);
	bool onSceneLayerWillMoveTo(Node *node, WJTouchEvent *event);
	void onSceneLayerTouchEnded(Node *node, WJTouchEvent *event);

public:

	P009();
	virtual ~P009();
	virtual bool init() override;

	virtual void onEnterTransitionDidFinish() override;

public:
	CREATE_SCENE_FUNC(P009);
};

#endif // __P009_LINE_H__
