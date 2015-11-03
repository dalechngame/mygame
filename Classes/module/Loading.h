#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "Common.h"

class Loading : public WJLayer
{
private:
	enum class Phase
	{
		kBeforeLoad, kLoad, kAfterLoad
	};

	static bool s_isLoading;
	static int s_loadingType;
    
	static Loading* gotoPhase(GameNumber gameNumber, void* userData, Phase phase);
	
    static Node* createNextSceneNode(GameNumber gameNumber, void* userData);
    static Scene* createNextScene(GameNumber gameNumber, void* userData);
	
protected:
	GameNumber m_gameNumber;
	void* m_userData;
	Phase m_phase;

	long long m_startShowTime;
	int m_asyncLoadTaskCount;

	EventListenerCustom* m_listenerStart;
	EventListenerCustom* m_listenerComplete;

	WJLayerJson *m_loadingJson;

    // 实现你的Loading的动画效果
    void doLoading(bool isDoNothing = false);

	void releaseResources();

	void loadNextSceneResources();
	void loadNextSceneSound();

	void createAndShowNextScene();
	void showNextScene(Node *nextSceneNode);

	void onLoadScene3StepCompleted();

public:
	bool init(GameNumber gameNumber, void* userData, Phase phase);
	bool init();
	virtual void onEnterTransitionDidFinish() override;
	virtual void onExit() override;

public:
	static Loading* gotoScene(GameNumber gameNumber, void* userData = nullptr);
	static void showLoading();
};

#endif // __LOADING_SCENE_H__
