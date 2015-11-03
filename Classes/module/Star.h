// Created by wangwenjie on 2014/01

#ifndef __STAR_H__
#define __STAR_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"

USING_NS_CC;

class Star : public WJLayerJson2x
{
private:

	Label *m_label;
	void starChangedNotification(Ref *object);

public:

    // 增加星星
    static void addStar(int n);

    // 减少星星
    static void subStar(int n);

    // 取得当前总的星星数量
    static int getTotalStar();
	
	// 播放增加星星的粒子效果
	void playParticle();


public:

	static Star* create();

    virtual bool initFromJsonFile(const char *jsonFileName);

    virtual void onEnter();
    virtual void onExit();

	virtual void onEnterTransitionDidFinish();
};

#endif  // __STAR_H__
