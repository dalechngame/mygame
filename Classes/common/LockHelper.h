#ifndef __LOCK_HELPER_H__
#define __LOCK_HELPER_H__

#include "cocos2d.h"
#include "lblibrarybase.h"
#include "PBase.h"
#include "TipDialog.h"

enum class LockStatus
{
	Reserved = 0,
	Available,			/* �ѽ����������Ʒ */
	PriceLocked,		/* ��Ҫ���ǹ������ */
	IAPLocked,			/* ��ҪIAP������� */
	GetfreeLocked		/* ��Ҫ�ۿ���Ƶ������ */
};

enum class IAPLockType
{
	Reserved = 0,
	Dress,				/* ������װ�� */
	Item,				/* ��Ʒ�� */
	HairPack,			/* ͷ����Ϸ�� */
	TreatPack,			/* ҽ����Ϸ�� */
	CookPack			/* ������Ϸ�� */
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
    static void updateMenuPositionInUseStarDialog(Node* pNode, TipDialog* pTipDialog);              /* �����˵�����Ļ�е���ʾλ�� */

private:
	static void addPriceSpriteAndLabel(WJBase* pItemBase, int price, float scaleFactor = 1.0f);
	static void addIAPLockSprite(WJBase* pItemBase, float scaleFactor = 1.0f);
	static void addGetfreeSprite(WJBase* pItemBase, float scaleFactor = 1.0f);

	// ��������Ƶ�������
	static void playLookGetfreeVideoAfterAni(Node* pItemNode);
	void playGetfreeVidelEndSpriteAni(Node* pItemNode);
	void playGetfreeVideoEndParticles(Node* pItemNode);

private:

	// ����getfree
	void showGetfreeVideo(const char* id);
	// �������ǵĵ���
	void showStarTipDialog(bool dressUp = false);
	void onStarTipDialogOkClick(TipDialog* pDialog);
	void onUseStarDialogOKClick(TipDialog *pTipDialog);                        /* ���ʹ��������ʾ��YES��ť��Ļص� */
	void onUseStarDialogCancelClick(TipDialog* pTipDialog);					  /* ���ʹ��������ʾ��NO��ť��Ļص� */

public:
	// ����¼�
	bool onLockItemClick(Node* pNode, WJTouchEvent* e, const char * key, bool bCallback, WJTouchCallback callback);

public:
	/** returns a shared instance of the LockHelper */
	static LockHelper* getInstance(void);
	virtual bool init();

	// ��ʼ��
	static void initLockOnScrollLayer(WJBase *itemBase, const char* key);
	// ͨ��key��typeȡ�õ�ǰ�ڵ��״̬
	static LockStatus getItemLockStatusFromScrollItem(const char * key);
	// ��ʼ��֮ǰ����֮ǰ��״̬
	static void clearLockStatus(WJBase* pItemBase, const char * key, LockStatus status);
	/* ��ʾʹ�����ǵ���ʾ�Ի��� */
	void showUseStarDialog(Node* pNode, WJTouchEvent* e, const char * key, bool bCallback, WJTouchCallback callback);
	// ���ǽ���
	static bool purchaseItemByStar(const char * key);
	// ͨ����ǰ������ȡ��iapkey
	static const char* getIAPKeyByLockType(IAPLockType type);


};

#endif // __LOCK_HELPER_H__