#ifndef __NAIL_LAYER_H__
#define __NAIL_LAYER_H__

#include "cocos2d.h"
#include "Common.h"
#include "ScrollLayer.h"
#include "ScrollItem.h"
#include "ScrollDoubleLayer.h"
#include "PUtils.h"

USING_NS_CC;

enum class NailState
{
	NormalState,
	drawingColorState,
	drawingPatternState,
	drawingDecorState,
	waitState
};

#define HAND_LAYER_SCALE_NUM              3            // handlayer�Ŵ�ı���

class NailLayer : public WJLayer
{
private:

	WJTouchCallback m_clickNormalNailCallBack;
	// ָ��ͼƬ
	WJSprite *m_sprite;
	WJSprite *m_coverSprite;
	WJSprite *m_circleSprite1;
	WJSprite *m_circleSprite2;
	WJSprite *m_patternSprite2;
	// װ��layer
	CC_SYNTHESIZE(WJLayer*, m_decorLayer, DecorLayer);
	// ��ǰװ��layerװ�����zorder
	CC_SYNTHESIZE(int, m_maxdecorZorder, MaxdecorZorder);
	// ָ�׶�Ӧ��ͼƬ
	CC_SYNTHESIZE(Image*, m_nailImage, NailImage);
	// ָ�׵ı��(1- n)
	CC_SYNTHESIZE(int, m_nailNo, NailNo);
	// ��ǰָ����ʽ
	CC_SYNTHESIZE(int, m_nailStyleNo, NailStyleNo);
	// ���ʵĴ�С
	CC_SYNTHESIZE(float, m_penSize, PenSize);
	// �Ƿ��Ѿ�����װ��ָ��
	CC_SYNTHESIZE(bool, m_isDrawed, IsDrawed);
	// ���Ƶ�ͼƬ
	CC_SYNTHESIZE(string, m_patternFile, PatternFile);
	// ����
	CC_SYNTHESIZE(WJSprite*, m_patternSprite, PatternSprite);
	// ��ǰ״̬
	NailState m_state;
	// ����
	WJCanvas *m_canvas;
	// ��ʾ��ָ��������
	RenderTexture *m_renderTexture;
	// ��������
	RenderTexture *m_patternTexture;

	void initImage();
	// ָ�׵���ʾ
	void initNailTips();
	// ��ʼ������
	void initCanvasLayer();
	// �����ǰָ�׽���ָ��װ��
	void onClickNailSprite(Node *node, WJTouchEvent *event);
	// ����˵�ǰ��ͨչʾ״̬���������ָ��
	void doEventClickNormalNailCallBack(Node *node, WJTouchEvent *e);

	void onShowSingleNailNotification(Ref *ref);

	// ������touchʱ��ص�
	void onTouchNailBegan(WJCanvas* canvas, Touch* touch);
	void onTouchNailDrawing(WJCanvas* canvas, Touch* touch);
	void onTouchNailDrawEnd(WJCanvas* canvas, Touch* touch);

	virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags) override;

	// ��ʾ���
	void stopplayCircleAni();
	void runCircleScaleAction(WJSprite *sprite);

	// װ��Ʒ�¼�
	bool onDecorSpriteTouchAble(Node *node, WJTouchEvent *event);
	void onDecorSpriteTouchEnded(Node *node, WJTouchEvent *event);

	// ��Ⱦ��ɫ
	void visitColor();
	// ��Ⱦ����
	void visitPattern();

public:

	NailLayer();
	~NailLayer();

	static NailLayer* create(WJSprite *sprite);
	virtual bool init(WJSprite *sprite);
	virtual void onEnterTransitionDidFinish() override;

	virtual void onEnter() override;
	virtual void onExit() override;

	virtual void update(float delta);

public:

	// װ��Ʒ
	ScrollDoubleLayer *_scrollLayer;

	// �ı䵱ǰָ�׵���ʽ
	void changeNewStyleNail(int styleNo);

	void setClickNormalNailCallBack(const WJTouchCallback &callback);

	void changeNailState(NailState state);

	WJCanvas* getNailCanvas();

	NailState getNailState();
	// ��ʾ����
	void startplayCircleAni(float delay);
	// ��Ⱦ����
	void visitPatternTexture(const char * fileName, bool enforce = false);
	// ȡ�õײ��ָ��ͼƬ
	WJSprite *getNailSprite();
	// ��յ�ǰ��װ��Ʒ
	void cleanDecorLayerStack();
	// ��¡װ��Ʒ
	void cloneDecorToNode(NailLayer *toLayer);
	// ���װ��Ʒ
	void addDecorSpriteIntoLayer(WJSprite *sprite);

	// װ�����
	void onClickScrollDecorItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	bool onScrollDecorMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollDecorCollisonEndedEvent(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	// װ��������¼�
	void initDecorSpriteEvent(WJSprite *sprite);

};

#endif // __HAND_LAYER_H__
