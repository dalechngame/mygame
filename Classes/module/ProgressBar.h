// Created by wangwenjie on 2014/01

#ifndef __PR0GRESS_BAR_H__
#define __PR0GRESS_BAR_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"

enum class ProgressStar
{
	noStar = 0,
	oneStar,
	twoStar,
	threeStar
};

class ProgressBar : public WJLayerJson2x
{
private:
	/* 当前星星的数量 */
	ProgressStar m_progressStar;

	void resetProgressStar();
	void loadStarSpriteTexture(const char * key, bool light);

	void creatParticleFromKey(const char *key);
	void creatParticleFromStar(ProgressStar last, ProgressStar now, bool playSound);
	void playSoundWithStar(const char * key, bool playSound);

public:
	/* 取得当前的星星数量 */
	ProgressStar getStarNum();
	/* 星星清零 */
	void setProgressNoneStar();
	/* 设置当前的星星的数量 */
	void setProgressStarNum(ProgressStar star, bool playSound = true);

public:
	ProgressBar();

	static ProgressBar* create(const char * jsonFile = "game/json/littlestars.json");

	virtual bool initFromJsonFile(const char * jsonFile);

    virtual void onEnterTransitionDidFinish();
};

#endif // __PR0GRESS_BAR_H__
