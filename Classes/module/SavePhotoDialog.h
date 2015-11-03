// Created by yuanzhongji on 2013/08

#ifndef _SAVE_PHOTO_DIALOG_H__
#define _SAVE_PHOTO_DIALOG_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "LBModalDialogLayer.h"
#include "TipDialog.h"
#include "PConfig.h"

class SavePhotoDialog : public TipDialog
{
private:

	// ��ť����¼�
	WJTouchCallback m_buttonClickCallBack;

	float m_saveWidth;
	float m_saveHeight;

	RenderTexture *m_renderTexture;
	WJSprite *m_photoSprite;

	void initEvent();
	void savePhotos();
	// ����ͼƬ
	virtual void onClickOkButton(Node *node, WJTouchEvent *event) override;
	void onClickButton(Node *node, WJTouchEvent *event);
	void doEventClickButtonEndCallBack(Node *node, WJTouchEvent *e);

public:
	SavePhotoDialog();

	static SavePhotoDialog* create(const char * jsonFile, Texture2D *texture);
	virtual bool init(const char * jsonFile, Texture2D *texture);

	// ��ť����Ļص�
	void setClickButtonEndCallBack(const WJTouchCallback &callback);

};

#endif // _SAVE_PHOTO_DIALOG_H__