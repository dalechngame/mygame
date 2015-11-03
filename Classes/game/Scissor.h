#ifndef __SCISSOR_H__
#define __SCISSOR_H__

#include "GarlandLayer.h"

enum class CutState
{
	NORMAL,
	CUT,
	SUSPAN
};

class Scissor :public WJLayer
{
protected:
	void initCut(WJLayerJson *layer);

	// cut event
	bool onTouchAble(Node *node, WJTouchEvent *event);
	bool OnWillMoveTo(Node *node, WJTouchEvent *event);
	void onTouchEnded(Node *node, WJTouchEvent *event);
	void onClick(Node *node, WJTouchEvent *event);

	virtual void update(float) override;
	void playAniWithCall(Node*ndde, bool withCall = true);
	void changeDyna(bool change);

protected:
	WJSkeletonAnimation * m_cut = nullptr;	// ¹Ç÷À¼ôµ¶
	WJLayer * m_staticCut = nullptr;	// ¾²Ì¬¼ôµ¶
	WJSprite *m_open = nullptr; // ¼ôµ¶¿ªµÄ×´Ì¬
	WJSprite *m_close = nullptr; // ¼ôµ¶¹ØµÄ×´Ì¬

	CutState m_cutState;		// ¼ôµ¶×´Ì¬

	GarlandLayer *m_garland = nullptr;
	WJLayerJson *m_json = nullptr;

	// ÇÐ»»Ðü¸¡×´Ì¬¼ÆÊ±Æ÷
	long long m_startTime = 0;
	long long m_currTime = 0;

	// Åö×²¼ÆÊ±Æ÷
	long long m_collidStart = 0;
	long long m_collidCurrent = 0;

public:
	void setStaticCutVisible(bool visible, float delayTime = 1.2f);
	bool isCutFinished();

public:
	Scissor(){}
	~Scissor(){}

	static Scissor* create(WJLayerJson *layer, GarlandLayer *garland);
	virtual bool init(WJLayerJson *layer, GarlandLayer *garland);
	virtual void onEnterTransitionDidFinish() override;

};

#endif /* defined(__SCISSOR_H__) */
