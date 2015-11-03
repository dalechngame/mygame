#include "P012Sugar.h"
#include "PUtils.h"
#include "ScrollItem.h"
#include "GameCommand.h"
#include "GameManager.h"

INIT_SCENE_LOADER_SYNC(P012);

#define ACTIONTAG_ELLIPSE             5566                  // 搅拌动画

#define DIPPER_NOT_PLAY_ANI           5668                 // 勺子没有播放动画
#define DIPPER_PLAYING_ANI            5669                 // 勺子正在播放动画

static bool s_firstStickLayer = true;

static const Vec2 s_BreadPoint[3] = {
	Vec2(-360, 500),
	Vec2(-340, 485),
	Vec2(-360, 505)
};

P012::P012()
{
	m_putBreadNum = 0;
	m_isPauseAction = false;
	m_mixRecord = 0;
	m_basicNum = 0;
	m_tickIndex = 0;
}

P012::~P012()
{

}

bool P012::init()
{
	if ( !PBase::init() )
	{
		return false;
	}

	initSceneJson("game/json/p012.json");

	initEvent();

	return true;
}


GameNumber P012::getGameNumber()
{
	return GameNumber::P012;
}

void P012::onEnterTransitionDidFinish()
{
	PBase::onEnterTransitionDidFinish();
	PUtils::runActionMoveToWithCallBack(m_breadpotSprite,
		PUtils::getPositionInWorldFromPoint(m_breadpotSprite, m_breadpotSprite->getSavedPosition()),
		CC_CALLBACK_1(P012::onBreadMoveInCallBack, this));
}

void P012::initEvent()
{
	// 面包片的锅
	m_breadpotSprite = m_sceneJson->getSubSprite("breadpot");
	m_breadpotSprite->saveCurrentProperties();
	m_breadpotSprite->setPositionX(1700);
	// 装各种调料的锅
	m_bowlSprite = m_sceneJson->getSubSprite("bowl");
	// 奶油瓶子
	m_frostSkeleton = m_sceneJson->getSubSkeleton("frostbowl");
	m_frostSkeleton->saveCurrentProperties();
	m_frostSkeleton->setPositionX(1800);
	m_frostSkeleton->setMoveAble(true);
	m_frostSkeleton->noClickMoveEffect();
	m_frostSkeleton->setOnTouchAble(CC_CALLBACK_2(P012::onFrostTouchAble, this));
	m_frostSkeleton->setOnWillMoveTo(CC_CALLBACK_2(P012::onFrostWillMoveTo, this));
	m_frostSkeleton->setOnTouchEnded(CC_CALLBACK_2(P012::onFrostTouchEnded, this));

	// 事件
	PUtils:: addListenerWithKey(m_sceneJson, "bread", 1, 3,
		CC_CALLBACK_2(P012::onBreadTouchAble, this),
		CC_CALLBACK_2(P012::onBreadWillMoveTo, this),
		CC_CALLBACK_2(P012::onBreadTouchEnded, this),
		nullptr);

	// 搅拌的棒子
	m_mixerSprite = m_sceneJson->getSubSprite("mixericon");
	m_mixerSprite->saveCurrentProperties();
	m_mixerSprite->setPositionX(1800);
	m_mixerSprite->noClickMoveEffect();
	m_mixerSprite->setMoveAble(true);
	m_mixerSprite->setOnTouchAble(CC_CALLBACK_2(P012::onMixerTouchAble, this));
	m_mixerSprite->setOnWillMoveTo(CC_CALLBACK_2(P012::onMixerWillMoveTo, this));
	m_mixerSprite->setOnTouchEnded(CC_CALLBACK_2(P012::onMixerTouchEnded, this));

	// 旋转的配置
	m_ellipseConfig.aLength = 100;
	m_ellipseConfig.cLength = 8;
	m_ellipseConfig.centerPositon = m_bowlSprite->getPositionInNode(m_mixerSprite->getParent()) - Vec2(-80, -240);

	// 装糖球的铁盘
	m_ballsPanel = m_sceneJson->getSubLayer("ballspanel");
	m_ballsPanel->saveCurrentProperties();
	m_ballsPanel->setPositionX(1800);

	// 勺子
	m_dipperSkeleton = m_sceneJson->getSubSkeleton("dipper");
	m_dipperSkeleton->saveCurrentProperties();
	m_dipperSkeleton->setPositionX(1800);
	m_dipperSkeleton->setMoveAble(true);
	m_dipperSkeleton->noClickMoveEffect();
	m_dipperSkeleton->setUserTag(DIPPER_NOT_PLAY_ANI);
	m_dipperSkeleton->setOnTouchAble(CC_CALLBACK_2(P012::onDipperTouchAble, this));
	m_dipperSkeleton->setOnWillMoveTo(CC_CALLBACK_2(P012::onDipperWillMoveTo, this));
	m_dipperSkeleton->setOnTouchEnded(CC_CALLBACK_2(P012::onDipperTouchEnded, this));
}

void P012::onBreadMoveInCallBack(Node *node)
{
	PUtils::playNodeScaleAni(node);
	// 手指提示
	startMoveFingerTips(m_breadpotSprite->getPositionWorld(), m_bowlSprite->getPositionWorld());
	this->runAction(Sequence::create(DelayTime::create(4.0f), 
		CCCallFunc::create([=]() {
			delayStartMoveFingerTipsForever(m_breadpotSprite->getPositionWorld(), m_bowlSprite->getPositionWorld());
		}),
		nullptr));
}

bool P012::onBreadTouchAble(Node *node, WJTouchEvent *event)
{
	WJSkeletonAnimation *sprite = (WJSkeletonAnimation*)node;
	sprite->setLocalZOrder(1000);
	stopMoveFingerTips();
	return true;
}

bool P012::onBreadWillMoveTo(Node *node, WJTouchEvent *event)
{
	WJSkeletonAnimation *skeleton = (WJSkeletonAnimation*)node;
	if (PUtils::isOneCollideWithOne(skeleton->getPositionWorld(), m_bowlSprite) && skeleton->isMoveAble())
	{
		skeleton->setMoveAble(false);
		skeleton->setEnabled(false);
		skeleton->cancelThisTouch(event->touch);
		int tag = skeleton->getUserTag();
		skeleton->stopAllActions();
		skeleton->runAction(Sequence::create(
			MoveTo::create(0.3f, s_BreadPoint[tag - 1]),
			CCCallFunc::create([=]() {

				// 将当前的面包添加到锅里面去
				const Vec2& point = skeleton->getPositionInNode(m_bowlSprite);
				skeleton->retain();
				skeleton->removeFromParentAndCleanup(false);
				skeleton->setPosition(point);
				m_bowlSprite->addChild(skeleton, 5);
				skeleton->release();

				// play ani
				skeleton->playAnimation(WJUtils::stringAddInt("aniRun", tag, 2).c_str(), false);
			}),
			DelayTime::create(1.0f),
			CCCallFunc::create([this]() {
				m_putBreadNum++;
				if (m_putBreadNum >= 3)
				{
					stopMoveFingerTips();
					m_breadpotSprite->stopAllActions();
					m_breadpotSprite->runAction(Sequence::create(
						MoveTo::create(0.5f, Vec2(1700, m_breadpotSprite->getPositionY())),
						CCCallFunc::create([this]() {
						PUtils::runActionMoveToWithCallBack(
							m_frostSkeleton,
							PUtils::getPositionInWorldFromPoint(m_frostSkeleton, m_frostSkeleton->getSavedPosition()),
							CC_CALLBACK_0(P012::onFrostMoveEnded, this));
						}),
					nullptr));
				}
			}),
			nullptr));

		return false;

	}
	return true;
}

void P012::showBreadInTheBowl()
{
	if (m_putBreadNum == 1)
	{
		setBreadVisible(1, 3, true);
	}
	else if (m_putBreadNum == 2)
	{
		setBreadVisible(4, 6, true);
	}
	else
	{
		setBreadVisible(7, 8, true);
	}
}

void P012::setBreadVisible(int startIndex, int endIndex, bool visible)
{
	WJSprite *sprite;
	for (int i = startIndex; i <= endIndex; i++)
	{
		sprite = m_sceneJson->getSubSprite(WJUtils::stringAddInt("breadbowl", i, 3).c_str());
		if (sprite)
			sprite->setVisible(visible);
	}
}

void P012::onBreadTouchEnded(Node *node, WJTouchEvent *event)
{
	restoreSavedPositionAndZorder(node);
	// 面包没有放完的情况开启提示
	if (m_putBreadNum < 3)
		delayStartMoveFingerTipsForever(m_breadpotSprite->getPositionWorld(), m_bowlSprite->getPositionWorld());
}

void P012::onFrostMoveEnded()
{
	PUtils::setNodeShadowVisible(m_sceneJson, m_frostSkeleton, true);

	PUtils::playNodeScaleAni(m_frostSkeleton);
	// 手指提示
	startMoveFingerTips(m_frostSkeleton->getPositionWorld(), m_bowlSprite->getPositionWorld());
	this->runAction(Sequence::create(DelayTime::create(4.0f),
		CCCallFunc::create([=]() {
		delayStartMoveFingerTipsForever(m_frostSkeleton->getPositionWorld(), m_bowlSprite->getPositionWorld());
	}),
	nullptr));
}

bool P012::onFrostTouchAble(Node *node, WJTouchEvent *event)
{
	PUtils::setNodeShadowVisible(m_sceneJson, m_frostSkeleton, false);
	stopMoveFingerTips();
	return true;
}

bool P012::onFrostWillMoveTo(Node *node, WJTouchEvent *event)
{
	WJSkeletonAnimation *skeleton = (WJSkeletonAnimation*)node;
	if (PUtils::isOneCollideWithOne(skeleton->getPositionWorld(), m_bowlSprite) &&
		skeleton->isEnabled())
	{
		skeleton->setEnabled(false);
		skeleton->setMoveAble(false);
		skeleton->stopAllActions();
		skeleton->runAction(Sequence::create(
			MoveTo::create(0.3f, Vec2(660, 700)),
			CCCallFunc::create([=]() {
				skeleton->playAnimation("aniPour", false);
			}),
			DelayTime::create(1.5f),
			CCCallFunc::create([this]() {
				m_sceneJson->getSubSkeleton("mixspine")->runActionFadeIn(0.5f, 0.0f, true);
			}),
			DelayTime::create(1.0f),
			CCCallFunc::create([this]() {
				m_sceneJson->getSubSkeleton("bread001")->runActionFadeOut(0.5f, 0.0f, true);
				m_sceneJson->getSubSkeleton("bread002")->runActionFadeOut(0.5f, 0.0f, true);
				m_sceneJson->getSubSkeleton("bread003")->runActionFadeOut(0.5f, 0.0f, true);
			}),
			CCCallFunc::create([=]() {
				skeleton->runAction(MoveTo::create(0.5f, Vec2(1800, skeleton->getPositionY())));
			}),
			DelayTime::create(1.0f),
			CCCallFunc::create(CC_CALLBACK_0(P012::onFrostPlayAniEnded, this)),
		nullptr));

		return false;
	}
	return true;
}

void P012::onFrostTouchEnded(Node *node, WJTouchEvent *event)
{
	WJSkeletonAnimation *skeleton = (WJSkeletonAnimation*)node;
	if (skeleton->isEnabled())
	{
		delayStartMoveFingerTipsForever(m_frostSkeleton->getPositionWorld(), m_bowlSprite->getPositionWorld());
		skeleton->restoreSavedPosition(0.3f, false, true, true, 0.0f, [this]() {
			PUtils::setNodeShadowVisible(m_sceneJson, m_frostSkeleton, true);
			m_frostSkeleton->restoreSavedZOrder();
		});
	}
}

void P012::onFrostPlayAniEnded()
{
	PUtils::playNodeScaleAni(m_mixerSprite);
	m_mixerSprite->runAction(Sequence::create(
		CCCallFunc::create([this]() {
		PUtils::runActionMoveToWithCallBack(
			m_mixerSprite,
			PUtils::getPositionInWorldFromPoint(m_mixerSprite, m_mixerSprite->getSavedPosition()),
			CC_CALLBACK_0(P012::onMixerSpriteMoveInEnd, this));
		}),
		nullptr));
}

void P012::onMixerSpriteMoveInEnd()
{
	m_mixerSprite->loadSpriteTextureAddSuffix("_", "s");

}

bool P012::onMixerTouchAble(Node *node, WJTouchEvent *event)
{
	WJSprite *sprite = (WJSprite*)node;
	sprite->setLocalZOrder(1000);
	stopMoveFingerTips();
	return true;
}

bool P012::onMixerWillMoveTo(Node *node, WJTouchEvent *event)
{
	WJSprite *sprite = (WJSprite*)node;
	if (!sprite->getActionByTag(ACTIONTAG_ELLIPSE))
	{
		// 搅拌了几圈
		m_mixRecord++;
		if (m_mixRecord >= 4)
		{
			setBreadVisible(1, 8, false);
			setProgressStarNum(ProgressStar::oneStar);
			// 搅拌的消失
			sprite->setEnabled(false);
			sprite->cancelThisTouch(event->touch);
			sprite->runAction(FadeOut::create(0.5f));

			// 盘子移动进来
			PUtils::runActionMoveToWithCallBack(
				m_ballsPanel,
				PUtils::getPositionInWorldFromPoint(m_ballsPanel, m_ballsPanel->getSavedPosition()));
			
			// 勺子
			PUtils::runActionMoveToWithCallBack(
				m_dipperSkeleton,
				PUtils::getPositionInWorldFromPoint(m_dipperSkeleton, m_dipperSkeleton->getSavedPosition()),
				CC_CALLBACK_1(P012::onDipperMoveInEnded, this));

			return false;
		}

	/*	else if (m_mixRecord == 2)
			m_sceneJson->getSubSprite("mix002")->runAction(FadeIn::create(1.5f));
		else if (m_mixRecord == 3)
			m_sceneJson->getSubSprite("mix003")->runAction(FadeIn::create(1.5f));*/

		sprite->stopAllActions();
		sprite->runAction(CircleBy::createWithDuration(1.5f, m_ellipseConfig))->setTag(ACTIONTAG_ELLIPSE);

		// 第一次播放搅拌动画
		if (m_mixRecord == 1)
			m_sceneJson->getSubSkeleton("mixspine")->playAnimation("aniPour", false);
	}
	if (m_isPauseAction)
	{
		m_sceneJson->getSubSkeleton("mixspine")->resumeAllAnimation();
		sprite->getActionManager()->resumeTarget(sprite);
	}

	return false;
}

void P012::onDipperMoveInEnded(Node *node)
{
	PUtils::playNodeScaleAni(node);
	startMoveFingerTips(m_dipperSkeleton->getPositionWorld(), m_bowlSprite->getPositionWorld());
	this->runAction(Sequence::create(DelayTime::create(4.0f),
		CCCallFunc::create([=]() {
		delayStartMoveFingerTipsForever(PUtils::getPositionInWorldFromPoint(m_dipperSkeleton, m_dipperSkeleton->getSavedPosition()), 
			m_bowlSprite->getPositionWorld());
		}),
	nullptr));
}

void P012::onMixerTouchEnded(Node *node, WJTouchEvent *event)
{
	m_isPauseAction = true;
	m_sceneJson->getSubSkeleton("mixspine")->pauseAllAnimation();
	node->getActionManager()->pauseTarget(node);
}

void P012::restoreSavedPositionAndZorder(Node *node)
{
	WJBase *base = dynamic_cast<WJBase*>(node);
	if (base && base->isEnabled())
	{
		base->restoreSavedPosition(0.3f, false, true, true, 0.0f, [=]() {
			base->restoreSavedZOrder();
		});
	}
}

bool P012::onDipperTouchAble(Node *node, WJTouchEvent *event)
{
	stopMoveFingerTips();
	return true;
}

bool P012::onDipperWillMoveTo(Node *node, WJTouchEvent *event)
{
	WJSkeletonAnimation *skeleton = (WJSkeletonAnimation*)node;
	if (PUtils::isOneCollideWithOne(skeleton->getPositionWorld(), m_bowlSprite) &&
		skeleton->getUserTag() == DIPPER_NOT_PLAY_ANI)
	{
		if (m_basicNum >= 5)
			return true;

		m_basicNum++;
		WJSprite *basicSprite = m_sceneJson->getSubSprite(WJUtils::stringAddInt("basic", m_basicNum, 3).c_str());
		skeleton->stopAllActions();
		skeleton->setUserTag(DIPPER_PLAYING_ANI);
		skeleton->runAction(Sequence::create(
			MoveTo::create(0.3f, Vec2(593, 450)),
			CCCallFunc::create([=]() {
				skeleton->playAnimation("aniLadle", false);
			}),
			DelayTime::create(1.0f),
			MoveTo::create(0.5f, m_dipperSkeleton->getParent()->convertToNodeSpace(basicSprite->getPositionWorld() + Vec2(130, 180))),
			DelayTime::create(0.5f),
			CCCallFunc::create([=]() {
				skeleton->playAnimation("aniPour", false);
			}),
			DelayTime::create(0.6f),
			CCCallFunc::create([&]() {
				m_sceneJson->getSubSprite(WJUtils::stringAddInt("basic", m_basicNum, 3).c_str())->setVisible(true);
				if (m_basicNum >= 5)
				{
					m_dipperSkeleton->setMoveAble(false);
					PUtils::setNodeShadowVisible(m_sceneJson, m_bowlSprite, false);
					m_dipperSkeleton->runAction(MoveTo::create(0.5f, Vec2(1800, m_dipperSkeleton->getPositionY())));
					m_bowlSprite->runAction(MoveTo::create(0.5f, Vec2(-300, m_bowlSprite->getPositionY())));
					m_ballsPanel->runAction(Sequence::create(
						Spawn::create(
							MoveTo::create(0.5f, Vec2(790, 514)),
							ScaleTo::create(0.5f, 1.0f),
							nullptr),
						CCCallFunc::create(CC_CALLBACK_0(P012::onBeginStick, this)),
					nullptr));

				}
				else
				{
					m_dipperSkeleton->setUserTag(DIPPER_NOT_PLAY_ANI);
					restoreSavedPositionAndZorder(m_dipperSkeleton);
				}

			}),
			nullptr));
	}
	else if (skeleton->getUserTag() == DIPPER_PLAYING_ANI)
	{
		return false;
	}

	return true;
}

void P012::onDipperTouchEnded(Node *node, WJTouchEvent *event)
{
	if (m_dipperSkeleton->getUserTag() == DIPPER_NOT_PLAY_ANI)
		restoreSavedPositionAndZorder(m_dipperSkeleton);

	// 还未完成的话添加提示
	if (m_basicNum < 5)
		delayStartMoveFingerTipsForever(PUtils::getPositionInWorldFromPoint(m_dipperSkeleton, m_dipperSkeleton->getSavedPosition()), 
		m_bowlSprite->getPositionWorld());
}

void P012::onBeginStick()
{
	// 选择糖球的棒子
	m_scrollLayer = ScrollLayer::create("P012_stick_", ScrollDragDir::DragLeft, 7, true, s_firstStickLayer);
	m_scrollLayer->setClickScrollItemEvent(CC_CALLBACK_3(P012::onClickStickScrollItem, this));
	m_scrollLayer->setScrollItemMovingCollisionEvent(CC_CALLBACK_3(P012::onScrollStickMovingCollision, this));
	m_scrollLayer->setScrollItemCollisonEndedEvent(CC_CALLBACK_3(P012::onScrollStickCollisonEndedEvent, this));
	SCROLL_POSITION_IN_JSON(m_scrollLayer);
	m_sceneJson->addChild(m_scrollLayer, 600);
	m_scrollLayer->initItemSelectFromIndex(0);
	s_firstStickLayer = false;
	m_scrollLayer->show(0.5f);
}

void P012::onClickStickScrollItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	m_tickIndex++;
	if (m_tickIndex >= 6)
		m_tickIndex = 1;
	changeStickTexture(m_tickIndex, item->getItemFilePath().c_str(), item->getItemIndex());
}

bool P012::onScrollStickMovingCollision(Node *node, WJTouchEvent *e, WJScrollLayer *scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	WJSprite *sprite;
	for (int i = 1; i <= 5; i++)
	{
		sprite = m_sceneJson->getSubSprite(WJUtils::stringAddInt("basic", i, 3).c_str());
		if (PUtils::isOneCollideWithOne(sprite, item->getMovingSprite(), false))
		{
			item->setItemUserData((void*)i);
			return true;
		}
	}


	return false;
}

void P012::onScrollStickCollisonEndedEvent(Node *node, WJTouchEvent *e, WJScrollLayer *scroll)
{
	ScrollItem *item = dynamic_cast<ScrollItem*>(node);
	int index = (int)item->getItemUserData();
	changeStickTexture(index, item->getItemFilePath().c_str(), item->getItemIndex());

	// 移除掉拖动的糖球棒
	WJSprite *sprite = item->getMovingSprite();
	WJSprite* newSprite = dynamic_cast<WJSprite*>(sprite->getUserObject());
	newSprite->followNode(nullptr);
	sprite->removeFromParentAndCleanup(true);
	sprite = nullptr;
	newSprite->removeFromParentAndCleanup(true);
	newSprite = nullptr;
}

void P012::changeStickTexture(int index, const char * file, int itemIndex)
{
	if (index <= 0 || index >= 6)
		return;

	WJSprite *sprite = m_sceneJson->getSubSprite(WJUtils::stringAddInt("stick", index, 3).c_str());
	sprite->loadSpriteTexture(file);
	sprite->setVisible(true);
	sprite->setUserTag(itemIndex + 1);

	setProgressStarNum(ProgressStar::twoStar);

	if (checkBothChangeStickTexture())
		showNextButton();
}

bool P012::checkBothChangeStickTexture()
{
	for (int i = 1; i <= 5; i++)
	{
		WJSprite *sprite = m_sceneJson->getSubSprite(WJUtils::stringAddInt("stick", i, 3).c_str());
		if (!sprite->isVisible())
			return false;
	}

	return true;
}

void P012::onClickNextButton(Node *node, WJTouchEvent *event)
{
	PBase::onClickNextButton(node, event);
	WJButton *button = (WJButton*)node;
	button->setEnabled(false);

	GameSugarShowCommand *command = GameSugarShowCommand::create();
	for (int i = 1; i <= 5; i++)
	{
		int index = m_sceneJson->getSubSprite(WJUtils::stringAddInt("stick", i, 3).c_str())->getUserTag();
		command->sugarVector.pushBack(__Integer::create(index));
	}
	GManager->pushGameCommandIntoGame(command, true);
	Loading::gotoScene(GameNumber::P012_show);
}