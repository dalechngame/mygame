#ifndef __P007_MAKEUP_H__
#define __P007_MAKEUP_H__

#include "PBase.h"
#include "ScrollLayer.h"
#include "ScrollDoubleLayer.h"

USING_NS_CC;

class P007_MakeUp : public PBase
{
protected:
	virtual GameNumber getGameNumber() override;\

private:
	enum makeUpStep
	{
		step_none,
		step1_baes,
		step2_eyebrow,
		step2_eyeshadow,
		step2_cheek,
		step2_mouth,
		step2_paster,
		step3_camera,
		step_max,
	};
	makeUpStep m_step;
	WJSkeletonAnimation* m_skeleton;

	WJCanvas* m_canvas_freckle;
	RenderTexture* m_renderTexture_freckle;
	WJSprite* m_freckle;
	WJSprite* m_step1Base;
	bool m_endThisTouch;

	void initStep1();
	void update_freckle(float delay);
	bool isFreckleClear();
	void endStep1();


	ScrollDoubleLayer* m_scrollDoubleLayer;
	WJSprite* m_currentTool;
	RenderTexture* m_step2RenderTexture[step2_mouth];
	WJCanvas* m_step2Canvas;
	int m_itemIndex;
	WJLayer* m_pasterLayer;

	void initStep2();
	void initCanvas();
	void initScrollLayer();
	void initMakeUpRenderTexture();
	void initPasterLayer();

	void onRightScrollItemClick(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollItemClick(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	bool onScrollItemTouchBegin(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	bool onScrollItemTouchMove(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onScrollItemTouchEnd(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);
	void onClickCancelItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll);

	void changePen(WJSprite* pen_png);
	void updateSlotTexetureFromRender(float delay);
	void drawAndUpdateSlotTexture(RenderTexture* rt, string pngPath, string slotName);

	bool isInFaceBox(Vec2 pos);
	Vec2 getRandomPostion();
	void addPaster(Node* node, bool isRandPos = true, Vec2 randPos = Vec2::ZERO);
	void ereasePaster(WJSprite* sp_old = nullptr);
	bool onPasterTouchAble(Node* node, WJTouchEvent* event);
	bool onPasterTouchMove(Node* node, WJTouchEvent* event);
	void onPasterTouchEnd(Node* node, WJTouchEvent* event);
	void endStep2();

	ScrollLayer* m_scrollLayer;
	void initStep3();
	void changeBG(int index);

	int m_changeBGNum;
	bool m_isPlayed[step2_paster - step1_baes];
	float playAnimation(int type);
	void playAnimation(const char* aniName);
	void refreshStar(int num);
	void updateTips1(float dt);
	void updateTips2(float dt);
	void updateTips3(float dt);
public:

	P007_MakeUp();
	virtual ~P007_MakeUp();
	virtual bool init() override;
	virtual void initSceneJson(const char* jsonFileName) override;
	virtual void onEnterTransitionDidFinish() override;
	virtual void onClickNextButton(Node *node, WJTouchEvent *event) override;
	virtual void initProgressBar(const Vec2 &pointWorld = Vec2::ZERO, float scale = 1.0f, const char * jsonFile = "game/json/littlestars.json") override;
	virtual void onSnapshotClosed() override;
public:
	CREATE_SCENE_FUNC(P007_MakeUp);
};

#endif // __P007_MakeUp_H__
