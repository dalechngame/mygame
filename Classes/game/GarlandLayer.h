#ifndef __GARLAND_LAYER_H__
#define __GARLAND_LAYER_H__

#include "PBase.h"

class GestureSprite;
class Scissor;
class P006;

#define DEFAULT_WEIGHT 1
#define HEIGHTER_WEIGHT 1

// slot infomation
struct MarkLeaf
{
	std::string leafName;				// slot 名字
	char weight = DEFAULT_WEIGHT; // 权重
	Vec2 position;						// 位置
	bool isVisible = true;		// 是否可见

	// 记录碰撞时间
	long long currentTime = 0; 
	long long startTime = 0; 

	MarkLeaf(){}
	MarkLeaf(const std::string &name, const Vec2 &pos, const char weight)
	{
		leafName = name;
		position = pos; 
		this->weight = weight;
	}

};

// flower infomation
struct MarkFlower
{
	bool isPlaced = false; // 是否放置了花环
	int index = -1;				// 花环的款式
	Node * tip = nullptr; 	// 碰撞区域
	Node *flower = nullptr;	// 花环

	MarkFlower(Node *flower, Node *tip)
	{
		this->flower = flower;
		this->tip = tip;
	}
	MarkFlower(){}
};

class GarlandLayer : public WJLayer
{
protected:
	void initDecoLayer(WJLayerJson *layer);
	void initFlower(WJLayerJson *layer);
	void initArrow();

	void onClick(Node *node, WJTouchEvent *event);
	void storeMarkLeaf(const std::string &leafName, const char weight = DEFAULT_WEIGHT); // 默认剪两次

protected:
	WJLayerJson *m_json = nullptr;
	WJLayer *m_flower = nullptr;														// flower
	CC_SYNTHESIZE(WJSkeletonAnimation *  , m_garland , Garland); // 花环
	CC_SYNTHESIZE(GestureSprite *, m_arrow, Arrow);			 // 手势
	CC_SYNTHESIZE(Scissor *, m_scissor, Scissor);					 // scissor引用

	CC_SYNTHESIZE(PBase*, m_parent, PBase);						// p006引用
	CC_PROPERTY(ProgressStar, m_progressState, ProgressState); 	// 星星

public:
	// 播放新手动画
	void startplayCircleAni(float delay= 0.f); // unused param
	void stopplayCircleAni();

	MarkFlower *isFlowerCutWithRect(Node *flower);
	MarkLeaf* isCutCollidedWithGarland(Node *cut);
	void playGarlandFinished();
	void changeFlower(Node * node, MarkFlower *mark, int index);

	std::vector< MarkLeaf *> m_leafVec;				// 叶子的相关information
	std::vector<MarkFlower *> m_flowerVec;			 // flower infomation

public:
	GarlandLayer() {
		m_garland = nullptr;
		m_arrow = nullptr;
		m_scissor = nullptr;
		m_parent = nullptr;
		m_progressState = ProgressStar::noStar;
	}
	~GarlandLayer(){}

	static GarlandLayer* create(WJLayerJson *layer, PBase *parent);
	virtual bool init(WJLayerJson *layer, PBase *parent);
	virtual void onEnterTransitionDidFinish() override;

};

#endif // __GARLAND_LAYER_H__
