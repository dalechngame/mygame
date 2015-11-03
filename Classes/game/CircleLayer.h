#ifndef __CIRCLE_LAYER_H__
#define __CIRCLE_LAYER_H__

#include "DecoLayer.h"

class P002;

class CircleLayer : public WJLayer
{
protected:
	void initDecoLayer(WJLayerJson *layer, bool usingSke);

protected:
	std::vector<DecoLayer*> m_decoVec;
	CC_SYNTHESIZE(int, m_currentIndex, CurrentIndex);
	CC_PROPERTY(int, m_originColor, OriginColor);	 // unused param
	CC_PROPERTY(int, m_currentColor, CurrentColor);  // unused param

	CC_PROPERTY(ProgressStar,  m_progressState, ProgressState);
	CC_SYNTHESIZE(PBase*, m_parent, PBase);

public:
	// ���ֶ���
	void startPlayCircleAni(float delay);
	void stopPlayCircleAni();

	void changeCircle(int index ,Node *newSpr); 			// �ı��ʽ
	void changeColor(int index, Node *newSpr); 			// �ı���ɫ
	void changeDrop(Node *newSpr, Mark*mark, int index); 	// �ı��׹
	void changeDeco(int index, Node *newNode);			// �ı�װ��

	DecoLayer*getCurrentDeco();
	Node *getCurrentCircle();
	WJLayer *getCurrentLayer();

	Mark* isCollided(Node *destSpr);
	Mark*getRandomMark(int index);

public:
	CircleLayer(){
		m_parent = nullptr;
		m_progressState = ProgressStar::noStar;
		m_decoVec.clear();
	}
	~CircleLayer(){}

	static CircleLayer* create(WJLayerJson *layer, bool usingSke, PBase *parent);
	virtual bool init(WJLayerJson *layer, bool usingSke, PBase *parent);
	virtual void onEnterTransitionDidFinish() override;

};

#endif // __CIRCLE_LAYER_H__
