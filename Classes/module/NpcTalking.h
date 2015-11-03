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
	// ��ǰ����label
	Label *m_nowLabel;
	// ���ŵ�key
	std::string m_playKey;
	// ����
	PrincessModel *m_princess;
	// ����
	GuestModel *m_guest;
	// ˵����json
	WJLayerJson *m_fontsJson;
	// �Ƿ��Ѿ�����
	bool m_isEnd;
	// ����ģ���ı���
	Sprite *m_sprite;
	// ҽ��ҽ������Ϸ��������ָ��������
	CC_SYNTHESIZE(int, m_healthModelIndex, HealthModelIndex);
	// ����̸��
	void onEndPlayNpcAni(float time);
	void doEventNpcTalkEndCallBack();
	void playNpcTalkAndAni();
	void initSpeakFonts();

	void onPrincessCompleted();
	void onGuestCompleted();

	// ���Ŷ������������
	void getAnimalKeyFromGameNumber();
	void playPrincessAniAndSound(float delay = 0.0f);
	void playGuestAniAndSound(float delay = 0.0f);
	void endPlayAniAndSound();
	void onDestorySelf();
	void changeTalk();
	void resetTalk(bool first);

	// ����
	void onClickSkipButton(Node *node, WJTouchEvent *event);

	void shaderBlur(WJLayerJson *json);
	void cupBlur(WJLayerJson *json);

	void runActionFadeIn(Node *node, float delay, float time);
	void actionFadeOut(Node *node, float delay, float time);

	// �������
	void stopPlaySoundAndAni();
	void gohomeNotification(Ref *object);

	void fadeInNpcAndFonts();

	void onSkeletonCompleted(int trackIndex, int loopCount, WJSkeletonAnimation *skeleton);

public:
	// �Ի���ɺ�Ļص�
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
