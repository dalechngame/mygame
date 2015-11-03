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
	// �Ƿ���Ի���
	CC_SYNTHESIZE(bool, m_isDrawedAble, IsDrawedAble);
	// �Ƿ������
	CC_SYNTHESIZE(bool, m_isCompleted, IsCompleted);
	// �ײ����������ֵ�
	CC_SYNTHESIZE(WJSprite*, m_spriteCover, SpriteCover);
	// ��߲�������ʾ�ļ�Ӱ
	CC_SYNTHESIZE(WJSprite*, m_spriteHigher, SpriteHigher);
	// ����
	CC_SYNTHESIZE(WJCanvas*, m_canvas, Canvas);

public:

	// ��ͼ
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
