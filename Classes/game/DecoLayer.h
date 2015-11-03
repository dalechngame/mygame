#ifndef __DECO_LAYER_H__
#define __DECO_LAYER_H__

#include "PBase.h"

struct Mark
{
	std::string name;				// ��������
	Vec2 bonePos;				// ������������

	bool isPlaced = false; // �Ƿ�����˵�׹
	int index = -1;				// ��׹�Ŀ�ʽ
	Node *drop = nullptr;	// ��׹

	// ���ֶ���
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
	void playDropAni(); 	// ���ŵ�׹�Ķ���
	void calc(float);

protected:
	Mark *m_currentMark = nullptr;							// ��ǰ�ĵ�׹
	CC_SYNTHESIZE(WJSprite *, m_originPic, OriginPic); // ԭʼjsonͼƬ
	CC_SYNTHESIZE(Node *, m_circle, Circle);					// ��ʽ
	CC_SYNTHESIZE(WJLayer *, m_layer, Layer);	
	CC_SYNTHESIZE(int, m_index, Index);					// ��ʽindex
	CC_SYNTHESIZE(int, m_colorIndex, ColorIndex);	// ��ɫindex
	CC_SYNTHESIZE(int, m_originColor, OriginColor);	// ��ʼ��ɫ

	CC_SYNTHESIZE(WJTouchCallback, m_clickCircleCallBack, ClickCircleCallBack);
	CC_SYNTHESIZE(bool, m_usingSke, UsingSke);
	
	Counter<long long> m_parabolaCalc;
	PosCalc m_posCalc;

public:
	// �������ֶ���
	void startplayCircleAni(float delay);
	void stopplayCircleAni();

	void seeThis(bool canSee); 				// ����/��ʾ��ǰ��ʽ
	void changeCircle(int index, Node *newSpr); 	// �ı��ʽ
	void changeDrop(Node *newSpr, Mark*mark,int index); 	// �ı��׹
	void changeDeco(int index, Node* newSpr);	// �ı�װ��
	Mark * isCollisioned(Node *destSpr); // ��ײ���
	Mark * getRandomMark(int index); // �������mark

	std::vector<Mark*> m_drop;
	std::vector<int> m_randomInt; // �����

public:
	DecoLayer();
	~DecoLayer();

	static DecoLayer* create(WJLayer *layer, int index, bool usingSke);
	virtual bool init(WJLayer *layer, int index, bool usingSke);
	virtual void onEnterTransitionDidFinish() override;
};

#endif // __DECO_LAYER_H__
