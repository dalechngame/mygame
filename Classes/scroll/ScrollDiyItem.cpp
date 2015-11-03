// Created by yuanzhongji on 2015/09

#include "ScrollDiyItem.h"
#include "GameManager.h"
#include "ScrollItemTemplate.h"

USING_NS_CC;

bool ScrollDiyItem::init(ScrollLayer *scrollLayer, const char *  key, int index, ScrollDragDir drag, bool clickAble)
{
	if (!ScrollItem::init(scrollLayer, key, index, drag, clickAble))
	{
		return false;
	}

	return true;
}

void ScrollDiyItem::onEnterTransitionDidFinish()
{
	ScrollItem::onEnterTransitionDidFinish();
}

void ScrollDiyItem::onEnter()
{
	ScrollItem::onEnter();
}

void ScrollDiyItem::onExit()
{
	ScrollItem::onExit();
}

ScrollDiyItem::ScrollDiyItem()
{
	m_needPlayGame = true;
	m_fileSaveType = DiySaveType::other;
	m_modelIsMoveScene = false;
}

bool ScrollDiyItem::checkItemUsedAbleBeforeCheckLocked(Node *node, WJTouchEvent *event, WJScrollLayer *scroll)
{
	if (m_needPlayGame && m_fileSaveType != DiySaveType::other)
	{
		showDiyPopDialog();
		return false;
	}

	return true;
}

void ScrollDiyItem::showDiyPopDialog()
{
	// diy pop
	TipDialog* pTipDialog = TipDialog::create("game/json/diypop.json");
	pTipDialog->setOnOkCallBack(CC_CALLBACK_1(ScrollDiyItem::onClickShowDiyPopOkButton, this));

	Common::sound.play("Common:popup");
	Director::getInstance()->getRunningScene()->addChild(pTipDialog, 9999);
	pTipDialog->show(LBModalDialogAnimationType::ScaleEaseBack);
}

void ScrollDiyItem::onClickShowDiyPopOkButton(TipDialog *dialog)
{
	if (m_needPlayGame)
	{
		switch (m_fileSaveType)
		{
		case DiySaveType::P012_Sugar:
			Loading::gotoScene(GameNumber::P012);
			break;
		case DiySaveType::P002_Dreamcatcher:
			Loading::gotoScene(GameNumber::P002);
			break;
		default:
			break;
		}
	}
}

bool ScrollDiyItem::checkItemMovingAbleWhenMoving(Node *node, WJTouchEvent *event)
{
	return !m_modelIsMoveScene;
}

void ScrollDiyItem::setModelIntoMoveScene(bool move)
{
	if (move)
		this->getItemIconSprite()->loadSpriteTextureAddSuffix("_", "s");
	else
		this->getItemIconSprite()->loadSpriteTextureRemoveSuffix("_");

	m_modelIsMoveScene = move;
}
