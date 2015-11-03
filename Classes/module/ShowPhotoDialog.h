// Created by yuanzhongji on 2013/08

#ifndef _SHOW_PHOTO_DIALOG_H__
#define _SHOW_PHOTO_DIALOG_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "LBModalDialogLayer.h"
#include "WJLayerJson.h"

class ShowPhotoDialog : public LBModalDialog
{
protected:

	WJLayerJson *m_layerJson;
	bool m_adsIsVisible;

	void initButtonEvent();
	void initPhotosFromSaved(int photoIndex);
	void onClickClose( Node *node, WJTouchEvent *event );
	
public:
	ShowPhotoDialog();

	static ShowPhotoDialog* create(const char * jsonFile, int photoIndex);
	virtual bool init(const char * jsonFile, int photoIndex);

	virtual bool initFromJsonFile(const char *jsonFileName);
	virtual void onEnterTransitionDidFinish();

	virtual void onEnter();
};

class ZoomPhotoLayer : public WJLayer
{
private:
	
	Size m_winSize;
	Point m_origion;
	// ��Ƭ
	WJSprite *m_sprite;
	// �Ƿ񳬳��߽�
	bool m_moveOutSide;
    
    cocos2d::Rect m_moveRect;

	// ����֮��ľ���
	double m_twoTouchPointLen;
	// ��㴥��
	virtual void touchesBegan(const std::vector<Touch*>& pTouches, Event *pEvent) override;
	virtual void touchesMoved(const std::vector<Touch*>& pTouches, Event *pEvent) override;
	virtual void touchesEnded(const std::vector<Touch*>& pTouches, Event *pEvent) override;

	bool checkSceneAllIn(float scale);

	// ���㴥���¼�
	virtual bool doEventTouchAble(Node* node, WJTouchEvent* e) override;
	virtual bool doEventWillMoveTo(Node* node, WJTouchEvent* e) override;
	virtual void doEventTouchEnded(Node* node, WJTouchEvent* e) override;

	// ��ǰlayer�Ƿ�����Ļ����
	bool checkLayerInView(const Point &point, float scale);
	bool scaleNodeFromZoom(float len);

public:

	ZoomPhotoLayer();

	static ZoomPhotoLayer* create(int photoIndex);
	virtual bool init(int photoIndex);
    void setMoveArea( const Rect &moveRect);
};

#endif // _SHOW_PHOTO_DIALOG_H__