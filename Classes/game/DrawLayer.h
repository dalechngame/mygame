#ifndef __DRAW_LAYER_H__
#define __DRAW_LAYER_H__

#include "cocos2d.h"
#include "Common.h"
#include "PUtils.h"

USING_NS_CC;


class DrawLayer : public WJLayer
{
private:

	RenderTexture *m_renderTexture;
	// 是否可以绘制
	CC_SYNTHESIZE(bool, m_isDrawedAble, IsDrawedAble);
	// 是否完成了
	CC_SYNTHESIZE(bool, m_isCompleted, IsCompleted);
	// 底层用来做遮罩的
	CC_SYNTHESIZE(WJSprite*, m_spriteCover, SpriteCover);
	// 最高层用来显示的剪影
	CC_SYNTHESIZE(WJSprite*, m_spriteHigher, SpriteHigher);
	// 画布
	CC_SYNTHESIZE(WJCanvas*, m_canvas, Canvas);

public:

	// 绘图
	void doDrawTouchAble(Node *node, WJTouchEvent *event, Vec2 &offSet = Vec2(-20, 140));
	void doDrawTouchMoved(Node *node, WJTouchEvent *event);
	void doDrawTouchEnded(Node *node, WJTouchEvent *event);

public:

	DrawLayer();
	~DrawLayer();

	static DrawLayer* create(WJSprite *sprite1, WJSprite *sprite2);
	virtual bool init(WJSprite *sprite1, WJSprite *sprite2);

	virtual void onEnterTransitionDidFinish() override;

	virtual void onEnter() override;
	virtual void onExit() override;

	virtual void update(float delta) override;

};

#endif // __HAND_LAYER_H__
