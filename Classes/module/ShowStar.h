// Created by wangwenjie on 2013/12

#ifndef __SHOW_STAR_H__
#define __SHOW_STAR_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "ProgressBar.h"
#include "LBModalDialogLayer.h"
#include "Common.h"
#include "PConfig.h"
#include "ConfigFoolHead.h"

class ShowStar: public WJLayerJson2x
{
private:

	// 当前保存的diy的key
	CC_SYNTHESIZE(string, m_diySaveKey, DiySaveKey);

	// 返回到主场景播放星星动画
	bool m_returnScenePlayStarAni;
	void gotoHomePage(float time);
	GameNumber m_returnGameNumber;

	ProgressStar m_progressStar;
	void playStarAni();
	void gotoHomeScheduce(float delay = 3.0f);

	void onPlayParticle(Node *node);
	void playStarParticle(WJSprite *sprite, const char * file);
	void playStarSpriteAni(WJSprite *sprite);
	void onPlayStarEnded(Node *node);
	void playSceneScaleAni();
	void onDialogShown(LBModalDialog *dialog);

private:
	RenderTexture* visitNode(DiySaveType type, Node *node);

public:

	ShowStar();

	// 默认弹出星星完成之后回到主场景播放心心飞的动画
	static ShowStar* create(ProgressStar progress, bool playStarAniAfter = true, GameNumber number = GameNumber::P014_Main);

    virtual bool init(ProgressStar progress, bool playStarAniAfter, GameNumber number);

    virtual void onEnter();
	virtual void onExit();
    virtual void onEnterTransitionDidFinish();

	virtual void show(Node *owner, int zOrder);
public:

	// 保存当前做好的物品(只是保存的图片和记录信息)
	bool saveItem(DiySaveType type, Node *node);
};

#endif // __SHOW_STAR_H__
