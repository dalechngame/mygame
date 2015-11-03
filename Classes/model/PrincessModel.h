// Created by wangwenjie on 2013/12

#ifndef __PRINCESS_MODEL_H__
#define __PRINCESS_MODEL_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "WJSkeletonAnimation.h"
#include "ModelBase.h"

///* 常态下公主播放的动画 */
//static const char* NORMAL_ANI[] = {
//	"aniCoversmile",
//	"aniLookdown",
//	"aniLookup",
//	"aniSalute",
//	"aniSmile",
//	"aniWave",
//	"aniWow",
//	"aniIdle2"
//};

#define	HAIR_SLOT		        "hair"
#define	CROWN_SLOT				"crown"
#define	CROWN_SLOT_1			"crown001"
#define ACCESSORY_SLOT			"acc"
#define CAPPA_SLOT				"cappa"
#define DRESS_SLOT		        "dress"
#define GLOVE_SLOT              "glove"

#define HAIR_DEFAULT_KEY        "P001_hair_001"
#define DRESS_DEFAULT_KEY       "P001_dress_001"

class PrincessModel : public ModelBase
{
private:
	/* 上一次播放普通动画的索引 */
	int m_perNormalIndex;
	// 播放idle动画
	void playPrincessIdleAni();
public:
	// 公主的状态的还原
	void restoreSavedStatus();
	// 播放动画完成后的回调
	virtual void onCompletedListener(int trackIndex, int loopCount) override;

public:
	// 小游戏和主场景公主的点击事件
	void setPrincessModelClickEvent();
	// 点击公主后播放随机的常态动画
	void onClickPrincessModel(Node *node, WJTouchEvent *event);
	// 设置当前的模特是否发出声音
	void setPrincessPlaySound(bool play);
	// 初始化画脸的render(画脸结束后拍照用)
	void initMakeFaceRender(RenderTexture *render, WJLayer *topLayer);

private:

	// 画脸相关
	spSlot *m_slotHead;
	spBone *m_boneRoot;
	float m_faceBoneDefaultRotation;
	WJLayer *m_modelTopLayerHead;
	RenderTexture *m_faceRenderTexture;

public:

	PrincessModel();
	virtual ~PrincessModel();

    static PrincessModel* create(bool playIdle = true);

    virtual bool init();

    virtual void onEnter() override;
	virtual void onExit() override;
    virtual void onEnterTransitionDidFinish() override;

	virtual void update(float fDelta) override;

	// 播放动画
	virtual void playAnimation( const char* animationName, bool loop, int stateIndex ) override;

	// 手抬起的事件+Food事件
	virtual void onSkeletonHandEvent(int stateIndex, spEvent* pEvent, WJSkeletonAnimation *skeleton);

private:
	void onHairChanged(int index, bool bPlayAnim = true, bool bReload = true);

	void playMakeupSound(const std::string& soundName, float duration = 1.5f);
	void playMakeupParticle(const char* particleName, int posIndex);

	void visitNodeWithMakeFace(Node *node);

private:
	bool m_makeupSoundPlaying;
	float m_makeupSoundPlayingTime;
	float m_makeupSoundDuration;

	// 换物件结束后播放动画和粒子效果
	void playMakeUpEndAniAndParticle(bool save, const char * animalName = "aniSmile");

public:

	// 加载除了头发外的其他部件
	void restoreSavedMakeItem();
	
	// 更换项链
	void onAccessoryChanged(const char *key, bool save = true);
	void cleanupAccessory(const char * key);

	// 更换护肩
	void onCappaChanged(const char * key, bool save = true);
	void cleanupCappa(const char * key);

	// 更换头环
	void onCrownChanged(const char * key, bool save = true);
	void cleanupCrown(const char *key);

	// 更换衣服
	void onDressChanged(const char * key, bool save = true);
	void cleanupDress(const char *key);

	// 更换手套
	std::string getGloveFileNameFromSlotIndex(int gloveIndex, int slot);
	void cleanupChangeGloveSlot(WJSkeletonAnimation *skeleton);
	void onGloveChanged(const char * key, bool save = true);
	void cleanupGlove(const char * key);

	void onHairChanged(const char * key, bool save = true);
};

#endif // __PRINCESS_MODEL_H__
