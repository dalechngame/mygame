#ifndef __HAND_LAYER_H__
#define __HAND_LAYER_H__

#include "cocos2d.h"
#include "Common.h"
#include "NailLayer.h"
#include "ScrollLayer.h"
#include "ScrollDoubleLayer.h"

USING_NS_CC;

enum class HandLayerState
{
	normalShowAllState,			 // ��ʾ������(�ʼ��״̬)
	begingShowSingleNailState,  // ��ʼ����ʾ����ָ�׵Ķ���
	drawingSingleNailColorState,    // �ֵķŴ󶯻����˿�ʼͿ��ɫ
	drawingSingleNailPatternState,  // ����
	drawingSingleNailDecorState,   // װ��Ʒ
	begingEndSingleNailState,   // ��ʼ�ָ���ʾ�����ֵĶ���
	wristbandHandState          // �����ֻ�װ��
};

class HandLayer : public WJLayer
{
private:
	// �������ͨ�ĵ���ָ��
	WJTouchCallback m_clickNormalNailCallBack;
	// ѡ����ָ�׵Ļ�����Ļص�
	WJTouchCallback m_chooseWristCallBack;

	// hand layer
	WJLayer *m_handLayer;
	CC_SYNTHESIZE(HandLayerState, m_Handstate, Handstate);
	// scroll layer
	ScrollDoubleLayer *m_scrollLayer;
	ScrollLayer *m_wristScrollLayer;

	WJButton *m_back;
	WJSprite *m_earseSprite;
	WJSprite *m_colorTool;
	WJButton *m_usedWhole;
	// �ֻ�
	WJSprite *m_bandSprite;

	// ��ǰ�������ָ��
	Vector<NailLayer*> m_nailVector;
	void initNailLayer(WJLayer *oldLayer);

	// ����ָ�׵�����
	void onClickNormalNail(Node *node, WJTouchEvent *event);
	void ClickNormalNailEnded(NailLayer *layer);
	void onRestoreSavedPosEnd();
	void onClickBackButton(Node *node, WJTouchEvent *event);
	void changeNailStateAndToolVisible(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void setHandWholeVisible(bool visible);

	// ����˵�ǰ��ͨչʾ״̬���������ָ��
	void doEventClickNormalNailCallBack(Node *node, WJTouchEvent *e);
	// ȡ�õ�ǰѡ�е�ָ��
	NailLayer* getSelectNailLayer();

	// ��Ƥ��
	bool onEarseTouchAble(Node *node, WJTouchEvent *event);
	bool onEarseWillMoveTo(Node *node, WJTouchEvent *event);
	void onEarseTouchEnded(Node *node, WJTouchEvent *event);

	// ����������ָ��ָ�׵���ʾ������
	void setNailAndFingerVisibleExceptDrawingNail(NailLayer *layer, bool visible);

	// ָ��Ϳ��ɫ
	void doCanvasTouchBegan(Node *node, WJTouchEvent *touchEvent, int index);

	bool onColorToolTouchAble(Node *node, WJTouchEvent *event);
	bool onColorToolWillMoveTo(Node *node, WJTouchEvent *event);
	void onColorToolTouchEnded(Node *node, WJTouchEvent *event);

	void onClickScrollColorItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	bool onScrollColorTouchAble(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	bool onScrollColorMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void setScrollColorTouchEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollColorDragOutCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	// ����
	void onClickScrollPatternItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	bool onScrollPatternMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollPatternCollisonEndedEvent(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	// �����Ӧ�õ�����
	void onClickUsedWholeButton(Node *node, WJTouchEvent *event);

	// double scroll layer
	void onClickScrollItemCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onClickCancelItemCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onClickMenuScrollItemCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	bool onScrollItemTouchAble(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollItemDragOutCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	bool onScrollItemMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollItemTouchEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollItemCollisonEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	// װ����
	bool onScrollDecorMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollDecorCollisonEndedEvent(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	// �ֻ�
	void onClickWristStyleScrollItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

public:

	HandLayer();
	~HandLayer();

	static HandLayer* create(WJLayer *layer);
	virtual bool init(WJLayer *layer);
	virtual void onEnterTransitionDidFinish() override;

public:
	// �ı䵱ǰָ�׵���ʽ(ȫ���ı�)
	void changeNewStyleNail(int styleNo, int nailno = -1);
	// λ�ø�λ
	void restoreSavedPositionAndScale();
	// �����˵�����ָ��
	void setClickNormalNailCallBack(const WJTouchCallback &callback);
	// ѡ�����ֻ���Ļص�
	void setChooseWristCallBack(const WJTouchCallback &callback);
	// �Ƿ���ײ����(ѡ��ָ����ʽ��ʱ��)
	int checkStyleCalculateAABBInWorld(ScrollItem *item);

	// �������
	void checkNailLayerColor();
	void checkBothNailDecorNum();

	// ���뵽װ���ֻ�
	void gotoWristBandMode();
};

#endif // __HAND_LAYER_H__
