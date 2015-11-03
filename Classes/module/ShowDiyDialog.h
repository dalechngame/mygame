// Created by yuanzhongji on 2013/08

#ifndef _SHOW_DIY_DIALOG_H__
#define _SHOW_DIY_DIALOG_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "LBModalDialogLayer.h"
#include "TipDialog.h"
#include "PConfig.h"

class ShowDiyDialog : public TipDialog
{
private:

	CC_SYNTHESIZE(string, m_diySaveKey, DiySaveKey);
	CC_SYNTHESIZE(WJSprite*, m_diySprite, DiySprite);

public:

	ShowDiyDialog();

	static ShowDiyDialog* create(const char * jsonFile, const char * key);
	virtual bool init(const char * jsonFile, const char * key);

	virtual void onEnterTransitionDidFinish() override;
};

#endif // _SHOW_DIY_DIALOG_H__