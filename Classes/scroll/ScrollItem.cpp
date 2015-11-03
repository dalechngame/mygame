// Created by yuanzhongji on 2015/09

#include "ScrollItem.h"
#include "GameManager.h"
#include "ScrollItemTemplate.h"

USING_NS_CC;

#define SCROLL_ITEM_SPRITE_TAG            5668                // 可以拖动的精灵tag
#define PARTICLE_TAG                      3679                // 粒子效果tag

bool ScrollItem::init(ScrollLayer *scrollLayer, const char *  key, int index, ScrollDragDir drag, bool clickAble)
{
	if (!WJLayer::init())
	{
		return false;
	}
	m_pScrollLayer = scrollLayer;
	m_dragDir = drag;
	m_itemIndex = index;
	m_itemClickAble = clickAble;
	// 读取配置文件里的数据
	m_primievalKey = key;
	m_scrollItemKey = key;
	WJUtils::stringAddInt(m_scrollItemKey, index + 1, 3);
	ScrollItemTemplate object = ScrollItemTemplateTem(m_scrollItemKey);
	m_type = (DragoutType)object.dragType;
	m_itemScaleTo = object.itemScale;
	m_itemRotateTo = object.itemRotate;
	m_itemFilePath = object.itemFile;
	m_cancelItem = object.cancleFlag;
	m_itemMoveAble = object.moveAble;
	m_itemBgScale = object.itemBgScale;
	m_itemIconFile = object.itemIconFile;
	m_itemIconScale = object.itemIconScale;

	// 背景
	m_bgSprite = WJSprite::create("game/03_minigame/common/ui/frame005.png");
	const Size &size = m_bgSprite->getContentSize();
	m_bgSprite->setPosition(Vec2(size.width / 2, size.height / 2));
	m_bgSprite->setScale(m_itemBgScale);
	this->addChild(m_bgSprite);

	// 滚动控件
	m_itemSprite = WJSprite::create(m_cancelItem ? "game/03_minigame/common/ui/cancel.png" : m_itemIconFile.c_str());
	m_itemSprite->setPosition(Vec2(size.width / 2, size.height / 2));
	m_itemSprite->setScale(m_itemIconScale);
	m_itemSprite->saveCurrentProperties();
	m_itemSprite->setTag(SCROLL_ITEM_SPRITE_TAG);
	this->addChild(m_itemSprite);

	this->setContentSize(size);
	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->ignoreAnchorPointForPosition(false);

	m_itemSprite->setClickAble(clickAble);
	m_itemSprite->noClickMoveEffect();
	m_itemSprite->setTouchSwallowsTouches(false);
	m_itemSprite->setOnTouchAble(CC_CALLBACK_2(ScrollItem::_onScrollItemTouchAble, this));
	m_itemSprite->setOnWillMoveTo(CC_CALLBACK_2(ScrollItem::_onScrollItemWillMoveTo, this));
	m_itemSprite->setOnTouchEnded(CC_CALLBACK_2(ScrollItem::_onScrollItemTouchEnded, this));
	m_itemSprite->setMoveAble(m_itemMoveAble);

	// 取消按钮
	if (m_cancelItem)
	{
		m_itemSprite->setClickAble(true);
		m_itemSprite->setOnClick(CC_CALLBACK_2(ScrollItem::_onClickCancelItem, this));
	}

	// lock init
	initLockedScrollItem();

	return true;
}

void ScrollItem::onEnterTransitionDidFinish()
{
	WJLayer::onEnterTransitionDidFinish();
}

void ScrollItem::onEnter()
{
	// store notification
	m_storeEventListener = _eventDispatcher->addCustomEventListener(NOTIFI_STORE_IAP_PURCHASED,
		[&](EventCustom* ec)
	{
		onStoreIapPurchased();
	});

	// getfree notification
	m_getfreeEventListener = _eventDispatcher->addCustomEventListener(NOTIFI_GETFREE_REWARD_VIDEO_CLOSED,
		[&](EventCustom* ec)
	{
		if (WJUtils::isGetFreeUnlocked(WJUtils::getLastRewardVideoLockKey()))
		{
			WJUtils::delayExecute(0.1f, [this](float delay)
			{
				onGetFreeUnlocked(WJUtils::getLastRewardVideoLockKey(), 0);
			});
		}

	});

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ScrollItem::onSelectScrollItemNotification),
		"selectItem", NULL);
	WJLayer::onEnter();
}

void ScrollItem::onStoreIapPurchased()
{
	// 重新刷新
	initLockedScrollItem();
}

void ScrollItem::onGetFreeUnlocked(const char* getFreeLockKey, int coins)
{
	// 重新刷新
	initLockedScrollItem();
}

void ScrollItem::initLockedScrollItem()
{
	LockHelper::initLockOnScrollLayer(this, m_scrollItemKey.c_str());
}

void ScrollItem::onExit()
{
	_eventDispatcher->removeEventListener(m_storeEventListener);
	_eventDispatcher->removeEventListener(m_getfreeEventListener);
	NotificationCenter::getInstance()->removeObserver(this, "selectItem");
	WJLayer::onExit();
}

void ScrollItem::onSelectScrollItemNotification(Ref *ref)
{
	__Integer *inter = (__Integer*)ref;
	// 当前的滚动条显示在屏幕里面的时候，接受到通知后当前的item变为选中的状态
	if (!m_pScrollLayer)
		setItemSelect(inter->getValue() == m_itemIndex);
	else if (m_pScrollLayer && m_pScrollLayer->getScrollIsShowed())
		setItemSelect(inter->getValue() == m_itemIndex);
}

bool ScrollItem::_onScrollItemTouchAble(Node *node, WJTouchEvent *event)
{
	WJScrollLayer *scroll = (WJScrollLayer*)this->getParent();

	// 禁用滚动区域外的Touch
	if (!scroll->containsTouchLocation(event->touch) || scroll->isScrollPaused())
		return false;

	m_movingSprite = (WJSprite *)node;
	m_movingCollide = false;

	return doEventScrollItemTouchAble(this, event, scroll);
}

bool ScrollItem::_onScrollItemWillMoveTo(Node *node, WJTouchEvent *event)
{
	// 是否可以移动的判断
	if (m_cancelItem || !checkItemMovingAbleWhenMoving(node, event))
		return false;

	WJSprite* sprite = static_cast<WJSprite*>(node);
	WJScrollLayer *scroll = (WJScrollLayer*)this->getParent();

	if (!scroll->isScrollPaused() && isScrollDragOut(event, m_dragDir))
	{
		// 其他逻辑的判断
		if (!checkItemUsedAbleBeforeCheckLocked(this, event, scroll))
		{
			sprite->cancelThisTouch(event->touch);
			return false;
		}

		// 加锁的判断
		if (!LockHelper::getInstance()->onLockItemClick(this, event, m_scrollItemKey.c_str(), true,
			CC_CALLBACK_2(ScrollItem::onMovingLockItemCallBack, this, scroll)))
		{
			sprite->cancelThisTouch(event->touch);
			return false;
		}

		scroll->pauseScroll();
		scroll->updateScrollPosition(0, 0);	  // 刷新滚动控件坐标，限制在正常范围内

		sprite->runAction(MoveTo::create(0.1f, sprite->getParent()->convertTouchToNodeSpace(event->touch) +
			movePointWhenDragOut(m_dragDir) + m_offsetWhenDragOut));

		// 拖出精灵效果
		// 计算得到将精灵移到m_sceneJson后的坐标（保证精灵在屏幕上看到的位置不变）
		//Vec2 posScene = sprite->getPositionInNode(m_scrollLayer->getSceneJson());
		// 计算得到将精灵移到m_sceneJson后的缩放比（保证精灵在屏幕上看到的大小不变）
		//Vec2 scaleScene = sprite->getScaleInNode(m_scrollLayer->getSceneJson());

		// 创建出一个新的精灵，而不是将之前的精灵拖出
		// 先调用自定义的init事件，返回null时在创建默认的sprite
		Node *newNode = _doEventWhenDragOutItemFromScrollLayer(sprite, event, scroll);
		CCASSERT(newNode != nullptr, "NODE IS NULL");

		newNode->setScale(0.01f);
		newNode->setRotation(m_itemRotateTo);
		newNode->runAction(ScaleTo::create(0.2f, m_itemScaleTo));
		m_pScrollLayer->getSceneJson()->addChild(newNode, 2000);

		// 跟随移动
		WJBase *base = dynamic_cast <WJBase*>(newNode);
		if (base)
		{
			base->followNode(sprite, Vec2::ZERO, false, false);
		}

		sprite->setVisible(false);
		sprite->setUserObject(newNode);

		// 创建一个新的精灵到滚动控件里面
		creatNewSpriteOnScrollSide(sprite, scroll, m_dragDir, m_itemScaleTo);

		// 物品被拖出来之后的回调(每次拖出来只会调用一次)
		doEventScrollItemDragOutCallBack(this, event, scroll);

		// 当前的item被选中了
		doEventSelectWhenDragOutFromScrollLayer(this);

		return false;
	}

	// 当前的滚动条暂停了
	if (scroll->isScrollPaused())
	{
		bool found = false;
		// 碰撞检测、如果符合条件返回true并且保存当前的信息到UserData里面
		found = doEventScrollItemMovingCollision(this, event, scroll);

		// 当前没有碰撞到
		if (!found && this->getUserData())
		{
			this->setUserData(nullptr);
		}

		// 保存当前的操作是否碰撞到了
		m_movingCollide = found;
	}

	return scroll->isScrollPaused();
}

bool ScrollItem::isScrollDragOut(WJTouchEvent *event, ScrollDragDir drag, float minDistance, float maxDistance)
{
	switch (drag)
	{
	case ScrollDragDir::DragLeft:
		return event->isDragOutLeft(minDistance, maxDistance);
		break;
	case ScrollDragDir::DragRight:
		return event->isDragOutRight(minDistance, maxDistance);
		break;
	case ScrollDragDir::DragUp:
		return event->isDragOutUp(minDistance, maxDistance);
		break;
	case ScrollDragDir::DragDown:
		return event->isDragOutDown(minDistance, maxDistance);
		break;
	default:
		return false;
	}
}

Vec2 ScrollItem::movePointWhenDragOut(ScrollDragDir drag)
{
	switch (drag)
	{
	case ScrollDragDir::DragLeft:
		return Vec2(-30, 0);
		break;
	case ScrollDragDir::DragRight:
		return Vec2(30, 0);
		break;
	case ScrollDragDir::DragUp:
		return Vec2(0, 30);
		break;
	case ScrollDragDir::DragDown:
		return Vec2(0, -30);
		break;
	default:
		return Vec2(0, 0);
	}
}

void ScrollItem::_onScrollItemTouchEnded(Node *node, WJTouchEvent *event)
{
	// 取消按钮
	if (m_cancelItem)
		return;

	WJSprite* sprite = dynamic_cast<WJSprite*>(node);
	WJScrollLayer *scroll = (WJScrollLayer*)this->getParent();
	Node* newNode = dynamic_cast<Node*>(sprite->getUserObject());

	// 点击了滚动控件
	if (sprite->isClickAble() && sprite->isMaybeClick() &&
		scroll->containsTouchLocation(event->touch) && !scroll->isScrollPaused())
	{
		playItemBounceEffect(node);

		// 其他逻辑的判断
		if (!checkItemUsedAbleBeforeCheckLocked(this, event, scroll))
		{
			return;
		}

		// 加锁判断
		if (LockHelper::getInstance()->onLockItemClick(this, event, m_scrollItemKey.c_str(), true,
			CC_CALLBACK_2(ScrollItem::onClickLockItemCallBack, this, scroll)))
		{
			Common::sound.play("Common:button");
			doEventSelectScrollItem(this, event, scroll);
			doEventScrollItemClick(this, event, scroll);
		}

		GameManager::removeUnusedDataAndTexture();
		return;
	}

	if (newNode)
	{
		scroll->resumeScroll();
		scroll->updateScrollPosition(0.0f, 0.0f);

		// 检测到碰撞后松手的事件处理
		if (m_movingCollide)
		{
			doEventSelectScrollItem(this, event, scroll);
			doEventScrollItemCollisonEnded(this, event, scroll);
		}
		else
		{
			doEventFailedScrollItem(this, event, scroll);
			// 当前移动的滚动精灵复位
			sprite->setEnabled(false, false);
			sprite->stopAllActions();
			sprite->runAction(Sequence::createWithTwoActions(
				MoveTo::create(0.2f, sprite->getSavedPosition()),
				CallFunc::create([=]() {
					Node* newNode2 = dynamic_cast<Node*>(sprite->getUserObject());
					if (newNode2)
					{
						// 更随状态的取消 
						WJBase *base = dynamic_cast<WJBase*>(newNode2);
						if (base)
							base->followNode(nullptr);

						// 移除
						if (newNode2->getParent())
						{
							newNode2->removeFromParentAndCleanup(false);
							newNode2 = nullptr;
						}
					}
			})));
		}
	}

	doEventScrollItemTouchEnded(this, event, scroll);
	GameManager::removeUnusedDataAndTexture();
	// 复位
	m_movingSprite = nullptr;
	m_movingCollide = false;
	m_itemUserData = nullptr;
}

void ScrollItem::creatNewSpriteOnScrollSide(WJSprite *oldsprite, WJScrollLayer *scroll, ScrollDragDir drag, float scaleTo)
{
	WJSprite *sprite = (WJSprite*)oldsprite->clone(true);
	sprite->noClickMoveEffect();
	sprite->setTouchSwallowsTouches(false);
	sprite->setOnTouchAble(CC_CALLBACK_2(ScrollItem::_onScrollItemTouchAble, this));
	sprite->setOnWillMoveTo(CC_CALLBACK_2(ScrollItem::_onScrollItemWillMoveTo, this));
	sprite->setOnTouchEnded(CC_CALLBACK_2(ScrollItem::_onScrollItemTouchEnded, this));
	sprite->setPosition(oldsprite->getSavedPosition());
	sprite->saveCurrentPosition();
	sprite->saveCurrentZOrder();
	sprite->setTag(oldsprite->getTag());
	sprite->setUserTag(oldsprite->getUserTag());
	sprite->setScale(oldsprite->getSavedScale());
	sprite->saveCurrentScale();
	sprite->setVisible(true);
	oldsprite->getParent()->addChild(sprite, oldsprite->getLocalZOrder());

	m_itemSprite = sprite;
}

ScrollItem::ScrollItem() : WJLayer(), ScrollBase()
{
	m_cancelItem = false;
	m_bgSprite = nullptr;
	m_pScrollLayer = nullptr;
	m_type = DragoutType::WJSPRITE;
	m_itemScaleTo = 1.0f;
	m_itemIndex = -1;
	m_movingSprite = nullptr;
	m_movingCollide = false;
	m_itemMoveAble = true;
	m_itemSprite = nullptr;
	m_particleNum = 0;
	m_offsetWhenDragOut = Vec2::ZERO;
	m_itemUserData = nullptr;
	m_itemRotateTo = 0.0f;
	m_itemClickAble = false;
	m_primievalKey = "";
	m_itemFilePath = "";
	m_scrollItemKey = "";
	m_itemIconFile = "";
}

void ScrollItem::setItemSelect(bool select)
{
	if (!select)
		m_bgSprite->loadSpriteTextureRemoveSuffix("_");
	else
		m_bgSprite->loadSpriteTextureAddSuffix("_", "l");
}

float ScrollItem::playItemBounceEffect(Node* pNode, float time)
{
	WJBase *base = WJBase::convertToWJBase(pNode);
	if (base)
	{
		float scale = base->getSavedScale();
		pNode->runAction(Sequence::create(ScaleTo::create(time, scale * 1.2f),
			ScaleTo::create(time, scale * 0.8f),
			ScaleTo::create(time, scale * 1.1f),
			ScaleTo::create(time, scale * 0.9f),
			ScaleTo::create(time * 0.667f, scale),
			nullptr));
	}

	return time * 4.667f;
}

std::string ScrollItem::getItemFilePath()
{
	return  m_itemFilePath;
}

void ScrollItem::_onClickCancelItem(Node *node, WJTouchEvent *event)
{
	Common::sound.play("Common:button");
	playItemBounceEffect(node);
	doEventClickCancelItem(this, event, (WJScrollLayer*)this->getParent());
}

WJSprite* ScrollItem::getMovingSprite()
{
	return m_movingSprite;
}

void ScrollItem::onClickLockItemCallBack(Node *node, WJTouchEvent *event, WJScrollLayer *scroll)
{
	doEventSelectScrollItem(this, event, scroll);
	doEventScrollItemClick(this, event, scroll);
}

void ScrollItem::onMovingLockItemCallBack(Node *node, WJTouchEvent *event, WJScrollLayer *scroll)
{
	doEventSelectScrollItem(this, event, scroll);
	doEventScrollItemClick(this, event, scroll);
}

WJSprite* ScrollItem::getItemIconSprite()
{
	return m_itemSprite;
}

void ScrollItem::addParticleIntoItem(int sum)
{
	m_particleNum = sum;
	ParticleSystemQuad *particle = ParticleSystemQuad::create("particles/warn02.plist");
	particle->setPosition(getItemIconSprite()->getPosition());
	particle->setTag(PARTICLE_TAG);
	particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	this->addChild(particle, 1000);
}

void ScrollItem::removeParticle()
{
	m_particleNum--;
	if (m_particleNum <= 0 && this->getChildByTag(PARTICLE_TAG))
	{
		this->removeChildByTag(PARTICLE_TAG);
	}
}

Node* ScrollItem::getItem(WJSprite *spr)
{
	WJBase *base = nullptr;
	switch (m_type)
	{
	case DragoutType::WJSPRITE:
		base = WJSprite::create(m_itemFilePath.c_str());
		return dynamic_cast<WJSprite *>(base);

		break;
	case DragoutType::WJSKELETON:
		base = WJSkeletonAnimation::createWithFile(m_itemFilePath.c_str());

		return dynamic_cast<WJSkeletonAnimation *>(base);

		break;
	default:
		break;
	}

	return nullptr;
}

Node* ScrollItem::_doEventWhenDragOutItemFromScrollLayer(WJSprite *sprite, WJTouchEvent *event, WJScrollLayer *scroll)
{
	do
	{
		// 自定义的拖拽出来初始化Node
		Node *node = doEventScrollDragOutInitNodeCallBack(this, event, scroll);
		if (node)
			return node;

		return getItem(sprite);

	} while (false);
}

void ScrollItem::cloneEventTo(ScrollItem *toItem)
{
	// clone event
	toItem->setScrollItemTouchAbleEvent(m_scrollTouchAble);
	toItem->setScrollItemMovingCollisionEvent(m_scrollMovingCollision);
	toItem->setScrollItemDragOutCallBack(m_scrollItemDragOutCallBack);
	toItem->setScrollItemCollisonEndedEvent(m_scrollCollisonEnded);
	toItem->setFailedSelectItemEvent(m_failedCallBack);
	toItem->setClickScrollItemEvent(m_clickItem);
	toItem->setSelectScrollItemEvent(m_selectCallBack);
	toItem->setClickCancelItemEvent(m_clickCancel);
	toItem->setScrollItemTouchEndedEvent(m_scrollTouchEndedCallBack);
	toItem->setScrollDragOutInitNodeCallBack(m_scrollDragOutInitNodeCallBack);

	// clone property
	toItem->setPosition(this->getPosition());
	toItem->setScale(this->getScale());
	toItem->setRotation(this->getRotation());
	toItem->setLocalZOrder(this->getLocalZOrder());
	toItem->setAnchorPoint(this->getAnchorPoint());
	toItem->setName(this->getName());
}

bool ScrollItem::checkItemUsedAbleBeforeCheckLocked(Node *node, WJTouchEvent *event, WJScrollLayer *scroll)
{
	return true;
}

bool ScrollItem::checkItemMovingAbleWhenMoving(Node *node, WJTouchEvent *event)
{
	return true;
}
