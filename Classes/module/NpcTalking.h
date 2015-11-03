// Created by wangwenjie on 2013/12

#ifndef __NPC_TALKING_H__
#define __NPC_TALKING_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "PrincessModel.h"
#include "GuestModel.h"
#include "Common.h"

typedef std::function<void ()> NpcTalkEndCallBack;

template <typename T>
T Edge(T i, T x, T w)
{  
	T i_k = x + i;  
	if      (i_k < 0)  i_k = 0;
	else if (i_k >= w) i_k = w - 1 - x;
	else               i_k = i;
	return i_k;
}

class NpcTalking: public WJLayer
{
private:

	NpcTalkEndCallBack m_talkEndCallBack;
	Texture2D *m_texture2D;
	RenderTexture *m_texture;
	RenderTexture *m_finalTexture;
	GameNumber m_gameNumber;
	WJButton *m_skipButton;
	// 当前字体label
	Label *m_nowLabel;
	// 播放的key
	std::string m_playKey;
	// 公主
	PrincessModel *m_princess;
	// 客人
	GuestModel *m_guest;
	// 说话的json
	WJLayerJson *m_fontsJson;
	// 是否已经结束
	bool m_isEnd;
	// 整个模糊的背景
	Sprite *m_sprite;
	// 医生医病的游戏开场动画指定的人物
	CC_SYNTHESIZE(int, m_healthModelIndex, HealthModelIndex);
	// 结束谈话
	void onEndPlayNpcAni(float time);
	void doEventNpcTalkEndCallBack();
	void playNpcTalkAndAni();
	void initSpeakFonts();

	void onPrincessCompleted();
	void onGuestCompleted();

	// 播放动画和配音相关
	void getAnimalKeyFromGameNumber();
	void playPrincessAniAndSound(float delay = 0.0f);
	void playGuestAniAndSound(float delay = 0.0f);
	void endPlayAniAndSound();
	void onDestorySelf();
	void changeTalk();
	void resetTalk(bool first);

	// 跳过
	void onClickSkipButton(Node *node, WJTouchEvent *event);

	void shaderBlur(WJLayerJson *json);
	void cupBlur(WJLayerJson *json);

	void runActionFadeIn(Node *node, float delay, float time);
	void actionFadeOut(Node *node, float delay, float time);

	// 声音相关
	void stopPlaySoundAndAni();
	void gohomeNotification(Ref *object);

	void fadeInNpcAndFonts();

	void onSkeletonCompleted(int trackIndex, int loopCount, WJSkeletonAnimation *skeleton);

public:
	// 对话完成后的回调
	void setNpcTalkEndCallBack(const NpcTalkEndCallBack& callback);

public:

	NpcTalking();
	~NpcTalking();

	 static NpcTalking* create(WJLayerJson *json, GameNumber game);
    virtual bool init(WJLayerJson *json, GameNumber game);

    virtual void onEnter();
	virtual void onExit();
    virtual void onEnterTransitionDidFinish();

	void setSkipButtonPostition(const Vec2 &pointWorld);

	WJButton* getSkipButton();
};

#endif // __NPC_TALKING_H__
