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
	normalShowAllState,			 // 显示整个手(最开始的状态)
	begingShowSingleNailState,  // 开始做显示单个指甲的动画
	drawingSingleNailColorState,    // 手的放大动画完了开始涂颜色
	drawingSingleNailPatternState,  // 花纹
	drawingSingleNailDecorState,   // 装饰品
	begingEndSingleNailState,   // 开始恢复显示整个手的动画
	wristbandHandState          // 进入手环装饰
};

class HandLayer : public WJLayer
{
private:
	// 点击了普通的单个指甲
	WJTouchCallback m_clickNormalNailCallBack;
	// 选择了指甲的花环后的回调
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
	// 手环
	WJSprite *m_bandSprite;

	// 当前手上面的指甲
	Vector<NailLayer*> m_nailVector;
	void initNailLayer(WJLayer *oldLayer);

	// 进行指甲的美化
	void onClickNormalNail(Node *node, WJTouchEvent *event);
	void ClickNormalNailEnded(NailLayer *layer);
	void onRestoreSavedPosEnd();
	void onClickBackButton(Node *node, WJTouchEvent *event);
	void changeNailStateAndToolVisible(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void setHandWholeVisible(bool visible);

	// 点击了当前普通展示状态下手上面的指甲
	void doEventClickNormalNailCallBack(Node *node, WJTouchEvent *e);
	// 取得当前选中的指甲
	NailLayer* getSelectNailLayer();

	// 橡皮擦
	bool onEarseTouchAble(Node *node, WJTouchEvent *event);
	bool onEarseWillMoveTo(Node *node, WJTouchEvent *event);
	void onEarseTouchEnded(Node *node, WJTouchEvent *event);

	// 设置其余手指和指甲的显示和隐藏
	void setNailAndFingerVisibleExceptDrawingNail(NailLayer *layer, bool visible);

	// 指甲涂颜色
	void doCanvasTouchBegan(Node *node, WJTouchEvent *touchEvent, int index);

	bool onColorToolTouchAble(Node *node, WJTouchEvent *event);
	bool onColorToolWillMoveTo(Node *node, WJTouchEvent *event);
	void onColorToolTouchEnded(Node *node, WJTouchEvent *event);

	void onClickScrollColorItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	bool onScrollColorTouchAble(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	bool onScrollColorMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void setScrollColorTouchEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollColorDragOutCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	// 花纹
	void onClickScrollPatternItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	bool onScrollPatternMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollPatternCollisonEndedEvent(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	// 点击了应用到所有
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

	// 装饰物
	bool onScrollDecorMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollDecorCollisonEndedEvent(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	// 手环
	void onClickWristStyleScrollItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

public:

	HandLayer();
	~HandLayer();

	static HandLayer* create(WJLayer *layer);
	virtual bool init(WJLayer *layer);
	virtual void onEnterTransitionDidFinish() override;

public:
	// 改变当前指甲的样式(全部改变)
	void changeNewStyleNail(int styleNo, int nailno = -1);
	// 位置复位
	void restoreSavedPositionAndScale();
	// 单击了单个的指甲
	void setClickNormalNailCallBack(const WJTouchCallback &callback);
	// 选择了手环后的回调
	void setChooseWristCallBack(const WJTouchCallback &callback);
	// 是否碰撞到了(选择指甲样式的时候)
	int checkStyleCalculateAABBInWorld(ScrollItem *item);

	// 评星相关
	void checkNailLayerColor();
	void checkBothNailDecorNum();

	// 进入到装修手环
	void gotoWristBandMode();
};

#endif // __HAND_LAYER_H__
