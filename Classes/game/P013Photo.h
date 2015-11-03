#ifndef __P013_PHOTO_H__
#define __P013_PHOTO_H__

#include "PBase.h"
#include "ScrollLayer.h"

USING_NS_CC;

class DottedSprite : public WJSprite
{
private:
	bool m_adsIsVisible;
	CC_SYNTHESIZE(int, m_itemIndex, ItemIndex);
	CC_SYNTHESIZE(bool, m_havingPhoto, HavingPhoto);
	WJSprite *m_havingPhotoSprite;

	virtual void update(float delta);
	void onPhotosTouchAbleSelector(Ref *ref);
	void onPhotosTouchEndedSelector(Ref *ref);

	void onClickDottedSprite(Node *node, WJTouchEvent *event);
	void onClickPhotoDialog(Node *node, WJTouchEvent *event, LBModalDialog *dialog);
	
public:

	DottedSprite();

	bool initWithFile(const char *pszFilename, int index);

	static DottedSprite* create(const char *pszFileName, int index);

	void autoAddPhotosInto(int index);

	void resetIndexDotted(int index);

	void removeLastPhotos(WJSprite *newSprite);
	void setHavingPhotos(WJSprite *sprite);

	virtual void onEnter();
	virtual void onExit();
};

class P013 : public PBase
{
protected:
	
	vector<WJSprite*> m_photoVector;
	vector<DottedSprite*> m_lineVector;
	void initPhotos();
	void setNodeVisible();
	virtual GameNumber getGameNumber() override;

	bool onPhotosTouchAble(Node *node, WJTouchEvent *event);
	void onPhotosTouchEnded(Node *node, WJTouchEvent *event);
	void copySavedSpritePos(WJSprite *sprite, const char * key);
	void onClickScene(Node *node, WJTouchEvent *event);

	bool onSceneTouchAble(Node *node, WJTouchEvent *event);
	bool onSceneWillMoveTo(Node *node, WJTouchEvent *event);

	void removePhotosFromVector(WJSprite *sprite);
	void showPhotosTips(float time);

public:
	virtual bool init() override;  

	virtual void onEnterTransitionDidFinish() override;

	virtual void onExit() override;

public:
	P013();
	~P013();

	CREATE_SCENE_FUNC(P013);

};

#endif // __P011_H__