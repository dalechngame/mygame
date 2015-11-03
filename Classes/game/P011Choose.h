#ifndef __P011_CHOOSE_H__
#define __P011_CHOOSE_H__

#include "PBase.h"

USING_NS_CC;

class P011_Choose : public PBase
{
protected:

	virtual GameNumber getGameNumber() override;

	void onClickPhotoButton(Node *node, WJTouchEvent *event);

public:

	virtual bool init() override;

	virtual void onEnterTransitionDidFinish() override;

public:

	CREATE_SCENE_FUNC(P011_Choose);
};

#endif // __P011_DRAW_H__
