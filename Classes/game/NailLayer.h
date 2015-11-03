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

#define HAND_LAYER_SCALE_NUM              3            // handlayer放大的比例

class NailLayer : public WJLayer
{
private:

	WJTouchCallback m_clickNormalNailCallBack;
	// 指甲图片
	WJSprite *m_sprite;
	WJSprite *m_coverSprite;
	WJSprite *m_circleSprite1;
	WJSprite *m_circleSprite2;
	WJSprite *m_patternSprite2;
	// 装修layer
	CC_SYNTHESIZE(WJLayer*, m_decorLayer, DecorLayer);
	// 当前装修layer装饰物的zorder
	CC_SYNTHESIZE(int, m_maxdecorZorder, MaxdecorZorder);
	// 指甲对应的图片
	CC_SYNTHESIZE(Image*, m_nailImage, NailImage);
	// 指甲的编号(1- n)
	CC_SYNTHESIZE(int, m_nailNo, NailNo);
	// 当前指甲样式
	CC_SYNTHESIZE(int, m_nailStyleNo, NailStyleNo);
	// 画笔的大小
	CC_SYNTHESIZE(float, m_penSize, PenSize);
	// 是否已经进入装饰指甲
	CC_SYNTHESIZE(bool, m_isDrawed, IsDrawed);
	// 花纹的图片
	CC_SYNTHESIZE(string, m_patternFile, PatternFile);
	// 花纹
	CC_SYNTHESIZE(WJSprite*, m_patternSprite, PatternSprite);
	// 当前状态
	NailState m_state;
	// 画布
	WJCanvas *m_canvas;
	// 显示的指甲油纹理
	RenderTexture *m_renderTexture;
	// 花纹纹理
	RenderTexture *m_patternTexture;

	void initImage();
	// 指甲的提示
	void initNailTips();
	// 初始化画布
	void initCanvasLayer();
	// 点击当前指甲进入指甲装饰
	void onClickNailSprite(Node *node, WJTouchEvent *event);
	// 点击了当前普通展示状态下手上面的指甲
	void doEventClickNormalNailCallBack(Node *node, WJTouchEvent *e);

	void onShowSingleNailNotification(Ref *ref);

	// 画布的touch时间回调
	void onTouchNailBegan(WJCanvas* canvas, Touch* touch);
	void onTouchNailDrawing(WJCanvas* canvas, Touch* touch);
	void onTouchNailDrawEnd(WJCanvas* canvas, Touch* touch);

	virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags) override;

	// 提示相关
	void stopplayCircleAni();
	void runCircleScaleAction(WJSprite *sprite);

	// 装饰品事件
	bool onDecorSpriteTouchAble(Node *node, WJTouchEvent *event);
	void onDecorSpriteTouchEnded(Node *node, WJTouchEvent *event);

	// 渲染颜色
	void visitColor();
	// 渲染花纹
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

	// 装饰品
	ScrollDoubleLayer *_scrollLayer;

	// 改变当前指甲的样式
	void changeNewStyleNail(int styleNo);

	void setClickNormalNailCallBack(const WJTouchCallback &callback);

	void changeNailState(NailState state);

	WJCanvas* getNailCanvas();

	NailState getNailState();
	// 提示动画
	void startplayCircleAni(float delay);
	// 渲染花纹
	void visitPatternTexture(const char * fileName, bool enforce = false);
	// 取得底层的指甲图片
	WJSprite *getNailSprite();
	// 清空当前的装饰品
	void cleanDecorLayerStack();
	// 克隆装饰品
	void cloneDecorToNode(NailLayer *toLayer);
	// 添加装饰品
	void addDecorSpriteIntoLayer(WJSprite *sprite);

	// 装饰相关
	void onClickScrollDecorItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	bool onScrollDecorMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollDecorCollisonEndedEvent(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	// 装饰物添加事件
	void initDecorSpriteEvent(WJSprite *sprite);

};

#endif // __HAND_LAYER_H__
