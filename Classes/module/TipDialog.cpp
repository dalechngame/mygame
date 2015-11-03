// Created by yuanzhongji on 2014/04

#include "TipDialog.h"
#include "GameManager.h"
#include "Common.h"

TipDialog* TipDialog::create(const char * jsonFile)
{
	TipDialog *layer = new TipDialog();
	if (layer && layer->init(jsonFile))
	{
		layer->autorelease();
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return NULL;
}

bool TipDialog::init(const char * jsonFile)
{
	m_okSelector = nullptr;
	m_closeSelector = nullptr;

	if (!TipDialog::initFromJsonFile(jsonFile))
		return false;

	initButtonEvent();

	return true;
}

bool TipDialog::initFromJsonFile(const char * fileName)
{
	const Size &size = Director::getInstance()->getWinSize();
	m_layerJson = WJLayerJson::create(fileName);
	m_layerJson->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_layerJson->ignoreAnchorPointForPosition(false);
	m_layerJson->setPosition(Vec2(size.width / 2, size.height / 2));

	// init dialog
	if (!LBModalDialog::init(m_layerJson, true))
	{
		CC_SAFE_DELETE(m_layerJson);
		return false;
	}

	return true;
}

void TipDialog::initButtonEvent()
{
	// close
	WJSprite *close = m_layerJson->getSubSpriteByKey("no");
	if (close)
	{
		close->setTag(MENU_TAG_CLOSE);
		close->setOnClick(CC_CALLBACK_2(TipDialog::onClickClose, this));
	}

	WJSprite *ok = m_layerJson->getSubSpriteByKey("yes");
	if (ok)
	{
		ok->setOnClick(CC_CALLBACK_2(TipDialog::onClickOkButton, this));
	}
}

void TipDialog::onEnterTransitionDidFinish()
{
	LBModalDialog::onEnterTransitionDidFinish();
}

void TipDialog::onClickClose( Node *node, WJTouchEvent *event )
{
	Common::sound.play("Common:close");
	closeMySelf((WJSprite*)node);
	doEventClickClose();
}

void TipDialog::onClickOkButton(Node *node, WJTouchEvent *event)
{
	Common::sound.play("Common:button");
	doEventClickOk();
	closeMySelf((WJSprite*)node);
}

TipDialog::TipDialog()
{
	m_adsIsVisible = false;
	m_layerJson = nullptr;
}

void TipDialog::setOnCloseCallBack( const TipClickCallback &selector )
{
	m_closeSelector = selector;
}

void TipDialog::closeMySelf(WJSprite *sprite)
{
	this->close();
	// 显示广告
	if (m_adsIsVisible && !WJUtils::isAdsVisible())
		Common::showAds();
}

void TipDialog::doEventClickClose()
{
	if (m_closeSelector)
		m_closeSelector(this);
}

void TipDialog::doEventClickOk()
{
	if (m_okSelector)
		m_okSelector(this);
}

void TipDialog::setOnOkCallBack( const TipClickCallback &selector )
{
	m_okSelector = selector;
}

void TipDialog::onEnter()
{
	LBModalDialog::onEnter();

	// 隐藏广告
	m_adsIsVisible = WJUtils::isAdsVisible();
	if (m_adsIsVisible)
		WJUtils::hideAds();
}
