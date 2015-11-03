// Created by wangwenjie on 2013/12

#ifndef __Model_BASE_H__
#define __Model_BASE_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "WJSkeletonAnimation.h"

// 人物的类型
enum modelType
{
	pricess = 1,
	man,
	woman,
	cat,
	dog
};

enum class ModelNo
{
	princess = 1,
	whitesKing,
	blackKing,
	asiaPrince,
	blackPrince,
	whitesPrince,
	dog,
	cat
};

class ModelBase : public WJLayer
{
protected:
	
	// 模特的编号
    int  m_modelNo;
	// 模特的类型
	modelType m_modelType;
	// 人物骨骼
    WJSkeletonAnimation *m_skeleton;
	// 当前播放的指定的动画
	vector<string> m_curAggregateStr;
	// 当前播放指定动画对应的声音
	vector<string> m_curSoundVector;

	WJTouchCallback m_modelTouchEndCallBack;

	// 是否在播放idle动画
	CC_SYNTHESIZE(bool, m_playIdle, playIdle);
	// 在通道一常态下播放的动画
	CC_SYNTHESIZE(string, m_playNameWithChannel1, PlayNameWithChannel1);
	// 中心点坐标
	CC_SYNTHESIZE(Point, m_middlePoint, MiddlePoint);
	// 播放动画的时候是否播放声音
	CC_SYNTHESIZE(bool, m_playSound, playSound);
	// 定时播放动画的时候是否播放声音
	CC_SYNTHESIZE(bool, m_timerPlaySound, TimerPlaySound);
	// 点击的时候是否播放声音
	CC_SYNTHESIZE(bool, m_clickPlaySound, ClickPlaySound);
	// 上一次播放指定动画的index
	CC_SYNTHESIZE(int, m_standbyIndex, StandbyIndex);
	// 上一次播放随机的声音的key
	CC_SYNTHESIZE(string, m_lastPlayRanAni, LastPlayRanAni);

	// 动画完成后的回调
	virtual void onCompletedListener(int trackIndex, int loopCount);
	// 点击事件
	virtual void onClickSkeleton(Node *node, WJTouchEvent *event);
	// 播放声音
	void playSoundWithPlayAni(const char * key);
	// 播放指定的动画序列("start"-----"end")
	virtual void setStandbyAnimation(const char *a, ...);
	// 播放指定声音("start"-----"end")
	virtual void setStandbySound(const char* a, ...);
	// 开始播放指定的动画序列和声音(点击默认也是播放这个)
	virtual void startPlayStandByAnimationSchedule(float IntervalTime = 6.0f);
	// 停止定时器
	virtual void stopStandbyAnimationSchedule();
	// 随机播放声音
	virtual void playStandbyAnimation(float time);
	// 播放当前指定的序列动画和声音
	virtual void playCurAggreagteAniAndSound(int index, bool loop, int stateIndex, bool playSound);
	// 播放动画的同时播放公主的动画对应的声音
	virtual void playModelAniBindSound(const char * animationName);
	// 播放点击的随机声音
	void startPlayClickStandbyAnimation();
	// 播放动画是随机找到当前播放的动画
	bool randomFindStandByAniIndex();
	// 播放指定的一个随机声音声音
	void playStandbyOneSoundEach();

protected:
	// 当前的模特是否加入到会场中去了
	CC_SYNTHESIZE(bool, m_modelIsMoveScene, ModelIsMoveScene);

private:

	void showCompletedNotification(Ref *object);

	// 模特触摸事件
	bool onModelTouchAbleWithPhoto(Node *node, WJTouchEvent *event);
	void onModelTouchEndedWithPhoto(Node *node, WJTouchEvent *event);
	void doEventModelTouchEndCallBack(Node *node, WJTouchEvent *event);

public:
    
    // 取得模特号 1~n
    int getModelNo();
	// 播放动画
	virtual void playAnimation( const char* animationName, bool loop, int stateIndex );
    // 取得骨骼
    WJSkeletonAnimation *getSkeleton();
	// 是否是宠物
	CC_SYNTHESIZE(bool, m_isAnimal, IsAnimal);
	// 声音是否绑定动画(只是用作公主的声音和动画绑定)
	CC_SYNTHESIZE(bool, m_soundIsBindAni, SoundIsBindAni);

	// 将客人包括宠物放到场景中去的时候播放声音
	virtual void playSoundWhenPutIntoScene();

public:
	// 从一个序列中播放随机的声音
	std::string playRandomAnimal( const char *a,... );
	// 移动事件
	void initMoveEvent();
	// 模特触摸后的回调
	void setModelTouchEndCallBack(const WJTouchCallback& callback);

public:

	ModelBase();

    virtual bool init();

    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();

	virtual void onExit();
};

#endif // __Model_BASE_H__
