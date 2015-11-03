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
// 滚动控件的坐标
#define SCROLL_POSITION_IN_JSON(layer_name) \
	layer_name->setPosition(m_sceneJson->convertToNodeSpace(Vec2(m_winSize.width + 300, m_winSize.height / 2 - 300)));

#define NEXT_BUTTON_SHOW_TAG                   2215              // 下一步按钮显示
#define NEXT_BUTTON_HIDE_TAG                   2216             // 下一步按钮隐藏
#define HOME_AND_PHOTOS_TOP_LENGTH             60               // HOME 和 photo 距离顶部的距离

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

	NoviceFinger *m_noviceFinger;         // 手指提示
	LockGameLayer *m_lockGameLayer;       // 加锁layer
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
	virtual void initLockGameLayer();                  // 初始化游戏的加锁layer
	virtual void initFingerLayer();                    // 初始化新手提示layer
	virtual void initPhotoButton(const Vec2 &point = Vec2::ZERO);   // 小游戏里面的拍照按钮
	virtual void initNextButton();                     // 小游戏里面的下一步按钮
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
	virtual void showNextButton(bool isShowPar = true);                                          // 显示下一步按钮
	virtual void hideNextButton();                                          // 隐藏下一步按钮
	virtual void onClickPhotoButton(Node *node, WJTouchEvent *event);       // override me.
	virtual void onClickNextButton(Node *node, WJTouchEvent *event);        // override me.
	virtual void showCameraTipsAction(bool isShowPar = true);

protected:

	/*----------------------------------------------------手指提示相关-------------------------------------------------------*/
	// 手指移动提示
	void startMoveFingerTips(Vec2 &fromPointWorld, Vec2 &toPointWorld, int repeatCount = 2, float interval = 1.0f);
	// 循环每隔6.0s提示2次手指移动的提示
	void startMoveFingerTipsForever(Vec2 &fromPointWorld, Vec2 &toPointWorld, float interval = 1.0f);
	// 停止手指的提示
	void stopMoveFingerTips();
	// 6.0s之后开始做手指提示的动画
	void delayStartMoveFingerTips(Vec2 &fromPointWorld, Vec2 &toPointWorld, int repeatCount = 2, float interval = 1.0f, float delay = 6.0f);
	// 6.0s之后开始循环做手指的提示动画
	void delayStartMoveFingerTipsForever(Vec2 &fromPointWorld, Vec2 &toPointWorld, float interval = 1.0f, float delay = 6.0f);
	/*--------------------------------------------------------------------------------------------------------------------------*/

public:

	// 设置当前场景的获得的星星数量
	virtual void setProgressStarNum(ProgressStar star, bool playSound = true);

	ProgressStar getProgressStarNum();

	// 执行命令
	virtual void executeCommand(float delay = 0.0f, bool sceneEnable = true);

	// 弹出星星的弹窗,并保存当前类型的节点为图片
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
