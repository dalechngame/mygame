// Created by yuanzhongji on 2013/08

#ifndef _TIP_DIALOG_H__
#define _TIP_DIALOG_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "LBModalDialogLayer.h"
#include "WJLayerJson1x.h"

using namespace cocos2d;
using namespace std;

class TipDialog;

typedef std::function<void (TipDialog *dialog)> TipClickCallback;

class TipDialog : public LBModalDialog
{
private:

	TipClickCallback m_okSelector;
	TipClickCallback m_closeSelector;

	void onDelayPushCallBackIntoStack();
protected:
	bool m_adsIsVisible;
	virtual void initButtonEvent();
	virtual void onClickClose(Node *node, WJTouchEvent *event);
	virtual void onClickOkButton(Node *node, WJTouchEvent *event);
	void doEventClickClose();
	void doEventClickOk();

	void closeMySelf(WJSprite *sprite);

public:
	CC_SYNTHESIZE(WJLayerJson*, m_layerJson, LayerJson);

public:
	TipDialog();

	static TipDialog* create(const char * jsonFile);
	virtual bool init(const char * jsonFile);

	virtual bool initFromJsonFile(const char *jsonFileName);
	virtual void onEnterTransitionDidFinish();

	virtual void onEnter();

	// 关闭后的回调
	void setOnCloseCallBack(const TipClickCallback &selector );
	// 点击ok后的回调
	void setOnOkCallBack(const TipClickCallback &selector );
};

#endif // _TIP_DIALOG_H__