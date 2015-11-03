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
	std::string leafName;				// slot ����
	char weight = DEFAULT_WEIGHT; // Ȩ��
	Vec2 position;						// λ��
	bool isVisible = true;		// �Ƿ�ɼ�

	// ��¼��ײʱ��
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
	bool isPlaced = false; // �Ƿ�����˻���
	int index = -1;				// �����Ŀ�ʽ
	Node * tip = nullptr; 	// ��ײ����
	Node *flower = nullptr;	// ����

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
	void storeMarkLeaf(const std::string &leafName, const char weight = DEFAULT_WEIGHT); // Ĭ�ϼ�����

protected:
	WJLayerJson *m_json = nullptr;
	WJLayer *m_flower = nullptr;														// flower
	CC_SYNTHESIZE(WJSkeletonAnimation *  , m_garland , Garland); // ����
	CC_SYNTHESIZE(GestureSprite *, m_arrow, Arrow);			 // ����
	CC_SYNTHESIZE(Scissor *, m_scissor, Scissor);					 // scissor����

	CC_SYNTHESIZE(PBase*, m_parent, PBase);						// p006����
	CC_PROPERTY(ProgressStar, m_progressState, ProgressState); 	// ����

public:
	// �������ֶ���
	void startplayCircleAni(float delay= 0.f); // unused param
	void stopplayCircleAni();

	MarkFlower *isFlowerCutWithRect(Node *flower);
	MarkLeaf* isCutCollidedWithGarland(Node *cut);
	void playGarlandFinished();
	void changeFlower(Node * node, MarkFlower *mark, int index);

	std::vector< MarkLeaf *> m_leafVec;				// Ҷ�ӵ����information
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
