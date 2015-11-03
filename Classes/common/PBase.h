#ifndef __PBASE_SCENE_H__
#define __PBASE_SCENE_H__

#include "cocos2d.h"
#include "LBToolbarLayer.h"
#include "LBModalDialogLayer.h"
#include "LBPopupMenu.h"
#include "LBSnapshotLayer.h"
#include "Common.h"
#include "Loading.h"
#include "Store.h"
#include "NoviceFinger.h"
#include "LockGameLayer.h"
#include "Star.h"
#include "ProgressBar.h"
#include "vector"
#include "ShowStar.h"
#include "SavePhotoDialog.h"
#include "PConfig.h"

USING_NS_CC;

#define ZORDER_SCENE_JSON			0
#define ZORDER_TOP_LAYER			1000
#define ZORDER_POPUPMENU			1000
#define ZORDER_ADS_LAYER			8000
#define ZORDER_SNAPSHOT				9000
#define ZORDER_FINGER_LAYER         5000
#define ZORDER_LOCK_LAYER			999
#define ZORDER_NEXT_BUTTON			500
#define ZORDER_PHOTO_BUTTON         600
#define ZORDER_PROGRESS             100

#define SCREEN_CENTER				CENTER_SIZE(m_winSize)

#define VISIBLE_RECT_CENTER			CENTER_RECT(m_visibleRect)

#define VISIBLE_RECT_TOP			Vec2(m_visibleRect.getMidX(), m_visibleRect.getMaxY())
#define VISIBLE_RECT_BOTTOM			Vec2(m_visibleRect.getMidX(), m_visibleRect.getMinY())
#define VISIBLE_RECT_LEFT			Vec2(m_visibleRect.getMinX(), m_visibleRect.getMidY())
#define VISIBLE_RECT_RIGHT			Vec2(m_visibleRect.getMaxX(), m_visibleRect.getMidY())

#define VISIBLE_RECT_LEFT_TOP		Vec2(m_visibleRect.getMinX(), m_visibleRect.getMaxY())
#define VISIBLE_RECT_LEFT_BOTTOM	Vec2(m_visibleRect.getMinX(), m_visibleRect.getMinY())
#define VISIBLE_RECT_RIGHT_TOP		Vec2(m_visibleRect.getMaxX(), m_visibleRect.getMaxY())
#define VISIBLE_RECT_RIGHT_BOTTOM	Vec2(m_visibleRect.getMaxX(), m_visibleRect.getMinY())

#define REMOVE_UNUSE_TEXTURE		Director::getInstance()->getTextureCache()->removeUnusedTextures();
#define DIRECTOR_RENDER				Director::getInstance()->getRenderer()->render();
// �����ؼ�������
#define SCROLL_POSITION_IN_JSON(layer_name) \
	layer_name->setPosition(m_sceneJson->convertToNodeSpace(Vec2(m_winSize.width + 300, m_winSize.height / 2 - 300)));

#define NEXT_BUTTON_SHOW_TAG                   2215              // ��һ����ť��ʾ
#define NEXT_BUTTON_HIDE_TAG                   2216             // ��һ����ť����
#define HOME_AND_PHOTOS_TOP_LENGTH             60               // HOME �� photo ���붥���ľ���

class PBase : public WJLayer
{
protected:
	void* m_userData;

	Size m_winSize;
	Size m_visibleSize;
    Rect m_visibleRect;
	Vec2 m_origin;

	WJLayer* m_topLayer;

	WJLayerJson* m_sceneJson;
	LBPopupMenu *m_popupMenu;
	LBSnapshotLayer *m_snapShot;

	LayerColor* m_adsLayer;
	bool m_adsLayerVisible;

	EventListenerCustom* m_storeEventListener;
	EventListenerCustom* m_getfreeEventListener;

	NoviceFinger *m_noviceFinger;         // ��ָ��ʾ
	LockGameLayer *m_lockGameLayer;       // ����layer
	WJButton *m_photoButton;
	WJButton *m_nextButton;
	Star *m_star;
	ProgressBar *m_progress;
	WJButton *m_homeButton;

	virtual GameNumber getGameNumber() = 0;	// override me.
	std::string getGameName();

	virtual void initSceneJson(const char* jsonFileName);
	virtual void initPopupMenu();
	virtual void initSnapshot();
	virtual void initAdsBanner();
	virtual void showGameAds();
	virtual void initLockGameLayer();                  // ��ʼ����Ϸ�ļ���layer
	virtual void initFingerLayer();                    // ��ʼ��������ʾlayer
	virtual void initPhotoButton(const Vec2 &point = Vec2::ZERO);   // С��Ϸ��������հ�ť
	virtual void initNextButton();                     // С��Ϸ�������һ����ť
	virtual void initStar(const Vec2 &point = Vec2::ZERO);
	virtual void initHomeButton();
	virtual void initProgressBar(const Vec2 &pointWorld = Vec2::ZERO, float scale = 1.0f,
		const char * jsonFile = "game/json/littlestars.json");

	void refreshAdBanner(float duration);
	virtual void onPopupMenuClick(Node* node, WJTouchEvent* event);

    virtual void showSnapshot(const char* fileName = nullptr);
	virtual void onSnapshotClick(Node* node, WJTouchEvent* event);
	virtual void onSnapshotBefore();	// override me.
	virtual void onSnapshotAfter();
	virtual void onSnapshotClosed();	// override me.

	virtual void onStoreClosedAndPurchased();	// override me.
	virtual void onGetFreeVideoClosed(const char* getFreeLockKey, bool isUnlocked);	// override me.
    virtual void onGetFreeVideoUnlocked(const char* getFreeLockKey);                // override me.

	virtual void playGameBgMusic();
	virtual void showInfoDialog();
	void showLockedGameLayer();

	virtual void onClickHomeButton(Node *node, WJTouchEvent *event);
	virtual void onShowNextButtonEnded(Node *node);
	virtual void showNextButton(bool isShowPar = true);                                          // ��ʾ��һ����ť
	virtual void hideNextButton();                                          // ������һ����ť
	virtual void onClickPhotoButton(Node *node, WJTouchEvent *event);       // override me.
	virtual void onClickNextButton(Node *node, WJTouchEvent *event);        // override me.
	virtual void showCameraTipsAction(bool isShowPar = true);

protected:

	/*----------------------------------------------------��ָ��ʾ���-------------------------------------------------------*/
	// ��ָ�ƶ���ʾ
	void startMoveFingerTips(Vec2 &fromPointWorld, Vec2 &toPointWorld, int repeatCount = 2, float interval = 1.0f);
	// ѭ��ÿ��6.0s��ʾ2����ָ�ƶ�����ʾ
	void startMoveFingerTipsForever(Vec2 &fromPointWorld, Vec2 &toPointWorld, float interval = 1.0f);
	// ֹͣ��ָ����ʾ
	void stopMoveFingerTips();
	// 6.0s֮��ʼ����ָ��ʾ�Ķ���
	void delayStartMoveFingerTips(Vec2 &fromPointWorld, Vec2 &toPointWorld, int repeatCount = 2, float interval = 1.0f, float delay = 6.0f);
	// 6.0s֮��ʼѭ������ָ����ʾ����
	void delayStartMoveFingerTipsForever(Vec2 &fromPointWorld, Vec2 &toPointWorld, float interval = 1.0f, float delay = 6.0f);
	/*--------------------------------------------------------------------------------------------------------------------------*/

public:

	// ���õ�ǰ�����Ļ�õ���������
	virtual void setProgressStarNum(ProgressStar star, bool playSound = true);

	ProgressStar getProgressStarNum();

	// ִ������
	virtual void executeCommand(float delay = 0.0f, bool sceneEnable = true);

	// �������ǵĵ���,�����浱ǰ���͵Ľڵ�ΪͼƬ
	void showStarDialog(DiySaveType saveType = DiySaveType::other, Node *node = nullptr);

public:
	Star* getStar();

	WJLayerJson* getLayerJson();

public:
	PBase();
	virtual ~PBase();
	virtual bool init() override;

	virtual void onEnter() override;
	virtual void onEnterTransitionDidFinish() override;
	virtual void onExit() override;
};

#endif // __PBASE_SCENE_H__
