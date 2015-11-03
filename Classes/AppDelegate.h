// Created by wangwenjie on 2013/04

#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include <string>

USING_NS_CC;

class AppDelegateTimer : public cocos2d::Ref
{
private:
    void onSessionStartTimer(float t);
    void onSessionResumeTimer(float t);

public:
    void sessionStart();
    void sessionResume();
};

/**
@brief	The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by CCDirector.
*/
class  AppDelegate : private cocos2d::Application
{
private:
	long long m_pauseTime;
    AppDelegateTimer m_timer;

public:
	AppDelegate();
	virtual ~AppDelegate();

	virtual void initGLContextAttrs();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
};

#endif // _APP_DELEGATE_H_

