#ifndef __LOCK_HELPER_H__
#define __LOCK_HELPER_H__

#include "cocos2d.h"
#include "lblibrarybase.h"
#include "PBase.h"
#include "TipDialog.h"

enum class LockStatus
{
	Reserved = 0,
	Available,			/* 已解锁或免费物品 */
	PriceLocked,		/* 需要星星购买解锁 */
	IAPLocked,			/* 需要IAP购买解锁 */
	GetfreeLocked		/* 需要观看视频广告解锁 */
};

enum class IAPLockType
{
	Reserved = 0,
	Dress,				/* 公主服装锁 */
	Item,				/* 物品锁 */
	HairPack,			/* 头发游戏包 */
	TreatPack,			/* 医生游戏包 */
	CookPack			/* 料理游戏包 */
};

class LockItemClickInfo
{
public:
	Node* m_node;
	WJTouchEvent* m_event;
	int m_index;
	PBase* m_owner;
	bool m_bCallback;
	std::string m_itemKey;
	WJTouchCallback m_callback;

public:
	LockItemClickInfo()
		: m_node(nullptr), m_event(nullptr), m_index(-1), 
		m_owner(nullptr), m_bCallback(false), m_callback(nullptr)
	{}

	~LockItemClickInfo()
	{
		if (m_event)
		{
			CC_SAFE_DELETE(m_event->touch);
			CC_SAFE_DELETE(m_event);
		}
	}

	void cloneTouchEvent(const WJTouchEvent* e)
	{
		if (e == nullptr)
		{
			m_event = nullptr;
			return;
		}

		m_event = new WJTouchEvent();
		
		m_event->x = e->x; 
		m_event->y = e->y; 
	
		m_event->width = e->width;    
		m_event->height = e->height;   
    
		m_event->widthInWorld = e->widthInWorld;     
		m_event->heightInWorld = e->heightInWorld;    

		m_event->maxWidthInWorld = e->maxWidthInWorld;  
		m_event->maxHeightInWorld = e->maxHeightInWorld;
    
		m_event->lastMoveWidthInWorld = e->lastMoveWidthInWorld;
		m_event->lastMoveHeightInWorld = e->lastMoveHeightInWorld;
    
		m_event->lastChangeDirMoveWidthInWorld = e->lastChangeDirMoveWidthInWorld;    
		m_event->lastChangeDirMoveHeightInWorld = e->lastChangeDirMoveHeightInWorld;   
    
		m_event->time = e->time;
    
		m_event->touch = cloneTouch(e->touch);
	}

	Touch* cloneTouch(const Touch* pTouch)
	{
		Touch* pNewTouch = new Touch();
		pNewTouch->setTouchInfo(0, pTouch->getLocationInView().x, pTouch->getLocationInView().y);

		return pNewTouch;
	}
};

class LockHelper : public Ref
{
private:

	static void saveItemPurchased(const std::string &itemKey);
	static bool isItemPurchased(const std::string& itemKey);
  
	//static void onUseStarDialogOKClick(TipDialog* pTipDialog);                                      
    static void updateMenuPositionInUseStarDialog(Node* pNode, TipDialog* pTipDialog);              /* 调整菜单在屏幕中的显示位置 */

private:
	static void addPriceSpriteAndLabel(WJBase* pItemBase, int price, float scaleFactor = 1.0f);
	static void addIAPLockSprite(WJBase* pItemBase, float scaleFactor = 1.0f);
	static void addGetfreeSprite(WJBase* pItemBase, float scaleFactor = 1.0f);

	// 播放完视频广告后相关
	static void playLookGetfreeVideoAfterAni(Node* pItemNode);
	void playGetfreeVidelEndSpriteAni(Node* pItemNode);
	void playGetfreeVideoEndParticles(Node* pItemNode);

private:

	// 播放getfree
	void showGetfreeVideo(const char* id);
	// 弹出星星的弹窗
	void showStarTipDialog(bool dressUp = false);
	void onStarTipDialogOkClick(TipDialog* pDialog);
	void onUseStarDialogOKClick(TipDialog *pTipDialog);                        /* 点击使用星星提示框YES按钮后的回调 */
	void onUseStarDialogCancelClick(TipDialog* pTipDialog);					  /* 点击使用星星提示框NO按钮后的回调 */

public:
	// 点击事件
	bool onLockItemClick(Node* pNode, WJTouchEvent* e, const char * key, bool bCallback, WJTouchCallback callback);

public:
	/** returns a shared instance of the LockHelper */
	static LockHelper* getInstance(void);
	virtual bool init();

	// 初始化
	static void initLockOnScrollLayer(WJBase *itemBase, const char* key);
	// 通过key和type取得当前节点的状态
	static LockStatus getItemLockStatusFromScrollItem(const char * key);
	// 初始化之前清理之前的状态
	static void clearLockStatus(WJBase* pItemBase, const char * key, LockStatus status);
	/* 显示使用星星的提示对话框 */
	void showUseStarDialog(Node* pNode, WJTouchEvent* e, const char * key, bool bCallback, WJTouchCallback callback);
	// 星星解锁
	static bool purchaseItemByStar(const char * key);
	// 通过当前的类型取得iapkey
	static const char* getIAPKeyByLockType(IAPLockType type);


};

#endif // __LOCK_HELPER_H__