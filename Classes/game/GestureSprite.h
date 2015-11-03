#ifndef __GESTURESPRITE_H__
#define __GESTURESPRITE_H__

#include "cocos2d.h"
#include "PUtils.h"

USING_NS_CC;

class GarlandLayer;

typedef std::function<void(float dt)> touchListener;

class GestureSprite : public Sprite
{
protected:
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	void playAnimation(bool play);
	void funCallback(float dt);
	void doEventTouchCallBack(float dt);

protected:
	EventListenerTouchOneByOne * m_listener = nullptr;
	CC_SYNTHESIZE(touchListener , m_touchListener, TouchListener);

	GarlandLayer *m_garland = nullptr;
	Sprite *m_gues = nullptr;

	Point prePos;   //起点位置
	Point curPos;   //当前点位置
	float xMove;
	float yMove;

public:
	static GestureSprite* create(GarlandLayer *garland, Node *spr);
	virtual bool init(GarlandLayer *garland, Node *spr);

	void setGuestureVisible(bool visible);
};

#endif // __GESTURESPRITE_H__
