// Created by wangwenjie on 2013/04

#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"

class Splash : public cocos2d::Layer
{
private:
	long long m_startTime;
	cocos2d::Scene *m_nextScene;

	void onCreateNextScene(float delay);
	void onGotoNextScene(float t);
    
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init() override;  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::Scene* createScene();

    virtual void onEnterTransitionDidFinish() override;
	
	// implement the "static node()" method manually
	CREATE_FUNC(Splash);
};

#endif // __SPLASH_SCENE_H__
