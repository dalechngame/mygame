// Created by yuanzhongji on 2014/04

#include "ShowDiyDialog.h"
#include "GameManager.h"
#include "Common.h"

ShowDiyDialog* ShowDiyDialog::create(const char * jsonFile, const char * key)
{
	ShowDiyDialog *layer = new ShowDiyDialog();
	if (layer && layer->init(jsonFile, key))
	{
		layer->autorelease();
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return NULL;
}

bool ShowDiyDialog::init(const char * jsonFile, const char * key)
{
	if (!TipDialog::init(jsonFile))
		return false;

	m_diySaveKey = key;
	std::string fileName = UserDefault::getInstance()->getStringForKey(key, "");
	std::string fullPath = GameManager::getAtlasSavePath() + fileName;
	if (WJUtils::fileExists(fullPath.c_str()))
	{
		m_diySprite = m_layerJson->getSubSprite("diyplace");
		m_diySprite->loadSpriteTexture(fullPath.c_str());
		m_diySprite->setOnClick(CC_CALLBACK_2(ShowDiyDialog::onClickClose, this));
		m_diySprite->noClickMoveEffect();
	}

	return true;
}

ShowDiyDialog::ShowDiyDialog()
{
	m_diySprite = nullptr;
}

void ShowDiyDialog::onEnterTransitionDidFinish()
{
	TipDialog::onEnterTransitionDidFinish();
	this->runAction(Sequence::create(
		DelayTime::create(5.0f), 
		CCCallFunc::create([&](){
			onClickClose(nullptr, nullptr);
		}),		
	nullptr));
}
