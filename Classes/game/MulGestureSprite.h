#ifndef __MULGESTURESPRITE_H__
#define __MULGESTURESPRITE_H__

#include "cocos2d.h"
#include "PUtils.h"

USING_NS_CC;

class Snowman;

typedef std::function<void(float dt)> touchListener;

struct MovePos
{
	Point startPos;   //起点位置
	Point curPos;   //当前点位置
	float xMove;
	float yMove;
	Rect touchRect;
};

class MulGestureSprite : public Sprite
{
protected:
	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);

	void runAnimationRM(Node *node);
	void playAnimation(bool play);
	void funCallback(float dt);
	void doEventTouchCallBack(float dt);

protected:
	EventListenerTouchAllAtOnce * m_listener = nullptr;
	CC_SYNTHESIZE(touchListener, m_touchListener, TouchListener);

	Snowman *m_snowman = nullptr;
	Sprite *m_guesLeft = nullptr;
	Sprite *m_guesRight = nullptr;

	MovePos m_leftPos;
	MovePos m_rightPos;

public:
	static MulGestureSprite* create(Snowman *garland, Node *left, Node *right);
	virtual bool init(Snowman *garland, Node *left, Node *right);

	void setGuestureVisible(bool visible);
};

#endif // __MULGESTURESPRITE_H__
