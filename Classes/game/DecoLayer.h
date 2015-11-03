#ifndef __DECO_LAYER_H__
#define __DECO_LAYER_H__

#include "PBase.h"

struct Mark
{
	std::string name;				// 骨骼名字
	Vec2 bonePos;				// 骨骼世界坐标

	bool isPlaced = false; // 是否放置了吊坠
	int index = -1;				// 吊坠的款式
	Node *drop = nullptr;	// 吊坠

	// 新手动画
	WJSprite *tip1= nullptr;
	WJSprite *tip2 = nullptr;
};

struct PosCalc
{
	Node *currentDeco = nullptr;
	Vec2 startPos;
	Vec2 currentPos;
};

class DecoLayer : public WJLayer
{
protected:
	void initDeco(WJLayer *layer);
	void initCircleTips();
	
	void onClickCircle(Node* node, WJTouchEvent* touchEvent);
	void doEventClickCircleCallBack(Node *node, WJTouchEvent *e);
	bool onTouchDeco(Node *node, WJTouchEvent *touchEvent);

	void playScaleAniWithCall(Node *spr, bool withCall = true); // unused method
	void runCircleScaleAction(WJSprite *sprite);
	void playSkeAni(int trackIndex, bool playParticle); 
	void playDropAni(); 	// 播放吊坠的动画
	void calc(float);

protected:
	Mark *m_currentMark = nullptr;							// 当前的吊坠
	CC_SYNTHESIZE(WJSprite *, m_originPic, OriginPic); // 原始json图片
	CC_SYNTHESIZE(Node *, m_circle, Circle);					// 款式
	CC_SYNTHESIZE(WJLayer *, m_layer, Layer);	
	CC_SYNTHESIZE(int, m_index, Index);					// 款式index
	CC_SYNTHESIZE(int, m_colorIndex, ColorIndex);	// 颜色index
	CC_SYNTHESIZE(int, m_originColor, OriginColor);	// 初始颜色

	CC_SYNTHESIZE(WJTouchCallback, m_clickCircleCallBack, ClickCircleCallBack);
	CC_SYNTHESIZE(bool, m_usingSke, UsingSke);
	
	Counter<long long> m_parabolaCalc;
	PosCalc m_posCalc;

public:
	// 播放新手动画
	void startplayCircleAni(float delay);
	void stopplayCircleAni();

	void seeThis(bool canSee); 				// 隐藏/显示当前款式
	void changeCircle(int index, Node *newSpr); 	// 改变款式
	void changeDrop(Node *newSpr, Mark*mark,int index); 	// 改变吊坠
	void changeDeco(int index, Node* newSpr);	// 改变装饰
	Mark * isCollisioned(Node *destSpr); // 碰撞检测
	Mark * getRandomMark(int index); // 返回随机mark

	std::vector<Mark*> m_drop;
	std::vector<int> m_randomInt; // 随机数

public:
	DecoLayer();
	~DecoLayer();

	static DecoLayer* create(WJLayer *layer, int index, bool usingSke);
	virtual bool init(WJLayer *layer, int index, bool usingSke);
	virtual void onEnterTransitionDidFinish() override;
};

#endif // __DECO_LAYER_H__
