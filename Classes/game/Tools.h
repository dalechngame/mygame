#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "Snowman.h"

enum class ShovelState
{
	NORMAL, 
	SHOVELING,
};

class Tools :public WJLayer
{
protected:
	void initShovel(WJLayerJson *layer);

	// cut event
	bool onTouchAble(Node *node, WJTouchEvent *event);
	bool OnWillMoveTo(Node *node, WJTouchEvent *event);
	void onTouchEnded(Node *node, WJTouchEvent *event);
	void onClick(Node *node, WJTouchEvent *event);

	void playAniWithCall(Node*ndde, bool withCall = true);
	void changeDyna(bool change);
	void setShovelVisible(bool visible, float delayTime = 1.2f);

protected:
	WJLayerJson *m_json = nullptr;
	Snowman *m_snowman = nullptr;
	bool isRunactionBack = true;
	CC_SYNTHESIZE(ShovelState, m_shovelState, ShovelState);

	CC_SYNTHESIZE(WJSprite * ,m_staticShovel ,StaticShovel);
	CC_SYNTHESIZE(WJSkeletonAnimation *, m_shovel, Shovel);
	WJSprite *m_box = nullptr;

public:
	void setBoxVisible(bool visible, float delayTime = 1.2f);

	bool isShovelFinished();

public:
	Tools(){
		ShovelState::NORMAL;
		m_staticShovel = nullptr;
	}
	~Tools(){}

	static Tools* create(WJLayerJson *layer, Snowman *snowman);
	virtual bool init(WJLayerJson *layer, Snowman *snowman);
	virtual void onEnterTransitionDidFinish() override;

};

#endif /* defined(__TOOLS_H__) */
