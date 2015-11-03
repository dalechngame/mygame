// Created by yuanzhongji on 2014/04

#include "SavePhotoDialog.h"
#include "GameManager.h"
#include "Common.h"


static bool s_isFirstSavePhoto = true;

SavePhotoDialog* SavePhotoDialog::create(const char * jsonFile, Texture2D *texture)
{
	SavePhotoDialog *layer = new SavePhotoDialog();
	if (layer && layer->init(jsonFile, texture))
	{
		layer->autorelease();
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return NULL;
}

bool SavePhotoDialog::init( const char * jsonFile, Texture2D *texture)
{
	if (!TipDialog::init(jsonFile))
		return false;

	// 展示图片
	WJSprite *sprite = m_layerJson->getSubSprite("photo");
	m_photoSprite = WJSprite::createWithTexture(texture);
	WJBase::copyProperties(sprite, m_photoSprite);
	sprite->getParent()->addChild(m_photoSprite, sprite->getLocalZOrder() + 1);
	m_photoSprite->setScale(0.5f);
	sprite->setVisible(false);
	
	initEvent();

	return true;
}

void SavePhotoDialog::initEvent()
{
	WJButton *close = m_layerJson->getSubButton("cancle");
	close->setTag(MENU_TAG_CLOSE);
	close->setOnClick(CC_CALLBACK_2(SavePhotoDialog::onClickButton, this));

	// facebook share
	WJButton*facebook = m_layerJson->getSubButton("share");
	facebook->setTag(MENU_TAG_FACEBOOK);
	facebook->setOnClick(CC_CALLBACK_2(SavePhotoDialog::onClickButton, this));
	// email
	WJButton* email = m_layerJson->getSubButton("email");
	email->setTag(MENU_TAG_EMAIL);
	email->setOnClick(CC_CALLBACK_2(SavePhotoDialog::onClickButton, this));

	// common photos
	WJButton* photos = m_layerJson->getSubButton("toipad");
	photos->setTag(MENU_TAG_PHOTOS);
	photos->setOnClick(CC_CALLBACK_2(SavePhotoDialog::onClickButton, this));

	// game photos
	WJButton* togamePhoto = m_layerJson->getSubButton("togame");
	togamePhoto->setTag(5599);
	togamePhoto->setOnClick(CC_CALLBACK_2(SavePhotoDialog::onClickButton, this));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (WJUtils::getDeviceOSVersion() < 6.0f)
	{
		facebook->setVisible(false);
		email->setPositionX(email->getPositionX() - 90);
	}
#endif
	
}

void SavePhotoDialog::savePhotos()
{
	// 取得当前已经保存过的总数
	std::string totalName = SAVE_PHOTO_PREFIX;
	std::string sumName = "photosSum";
	int sum = UserDefault::getInstance()->getIntegerForKey(sumName.c_str(), 0);

	//保存png
	std::string fileName = SAVE_PHOTO_PREFIX;
	fileName.append("_");
	fileName.append(WJUtils::stringAddInt("", sum + 1).c_str());
	fileName.append("_");
	for (int i = 0; i < 6; i++)
	{
		int c = WJUtils::randomInt(2);
		int num = WJUtils::randomInt(26);
		char m;
		if (c == 0)
		{
			m = num + 'A';
		}
		else
		{
			m = num + 'a';
		}
		fileName += m;
	}
	fileName.append(".png");

	// 保存当前的照片
	std::string fullPath = GameManager::getAtlasSavePath() + fileName;
	
	const Size &size = Size(1024, 768);
	const Size &winSize = Director::getInstance()->getWinSize();
	const Size &sSize = m_photoSprite->getContentSize();

	RenderTexture *render = RenderTexture::create(size.width, size.height);
	render->setAnchorPoint(Vec2(0.5f, 0.5f));
	render->ignoreAnchorPointForPosition(false);
	render->getSprite()->getTexture()->setAntiAliasTexParameters();

	auto renderer = Director::getInstance()->getRenderer();
	auto& parentTransform = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Vec2 pointSave = m_photoSprite->getPosition();
	float scaleSave = m_photoSprite->getScale();
	render->beginWithClear(0, 0, 0, 0);
	m_photoSprite->setVisible(true);
	m_photoSprite->setScale(1.0f * 768 / sSize.height);
	m_photoSprite->setPosition(Vec2(size.width / 2, size.height / 2));
	m_photoSprite->visit(renderer, parentTransform, true);
	//m_photoSprite->setVisible(false);
	render->end();
	m_photoSprite->setPosition(pointSave);
	m_photoSprite->setScale(scaleSave);
	Director::getInstance()->getRenderer()->render();

	// 保存图片
	Image *image = render->newImage(true);
	//WJGraphics::unblendAlpha(image);
	bool result = image->saveToFile(fullPath, true);
	CC_SAFE_DELETE(image);
	int maxNum = SAVE_PHOTO_MAX;

	if (result)
	{
		int nowIndex = (sum + 1) % maxNum;

		// 需要覆盖以前保存的旧的图片
		if (sum + 1 > maxNum)
		{
			// 检查是否有一个序列图片不存在了
			std::string oldFileName;
			std::string key = GameManager::findNoneSavePhotosKey();
			if (!WJUtils::equals(key, ""))
			{
				totalName = key;
			}
			// 都保存有图片、取得当前最旧的一个
			else
			{
				totalName.append(WJUtils::stringAddInt("", nowIndex == 0 ? maxNum : nowIndex).c_str());
			}

			oldFileName = UserDefault::getInstance()->getStringForKey(totalName.c_str(), "");
			// 当前存在并且没有被使用则删除
			if (!WJUtils::equals(oldFileName.c_str(), ""))
			{
				std::string fullpath = GameManager::getAtlasSavePath() + oldFileName;
				if (WJUtils::fileExists(fullpath.c_str()))
					FileUtils::getInstance()->removeFile(fullpath);
			}
		}
		else
		{
			totalName.append(WJUtils::stringAddInt("", nowIndex).c_str());
		}

		// 保存当前的图片
		UserDefault::getInstance()->setStringForKey(totalName.c_str(), fileName);
		UserDefault::getInstance()->flush();

		// 保存当前照片的总数
		UserDefault::getInstance()->setIntegerForKey(sumName.c_str(), sum + 1);
		UserDefault::getInstance()->flush();
	}
}

void SavePhotoDialog::onClickOkButton( Node *node, WJTouchEvent *event )
{
	if (s_isFirstSavePhoto)
	{
		s_isFirstSavePhoto = false;
	}
	TipDialog::onClickOkButton(node, event);
	//savePhotos();
}

void SavePhotoDialog::onClickButton(Node *node, WJTouchEvent *event)
{
	// to game photos
	if (node->getTag() == 5599)
	{
		savePhotos();
	}
	else
	{
		doEventClickButtonEndCallBack(node, event);
		if (node->getTag() == MENU_TAG_CLOSE)
			closeMySelf((WJSprite*)node);
	}
}

SavePhotoDialog::SavePhotoDialog()
{
	m_renderTexture = nullptr;
	m_photoSprite = nullptr;
	m_buttonClickCallBack = nullptr;
}

void SavePhotoDialog::setClickButtonEndCallBack(const WJTouchCallback &callback)
{
	m_buttonClickCallBack = callback;
}

void SavePhotoDialog::doEventClickButtonEndCallBack(Node *node, WJTouchEvent *e)
{
	if (m_buttonClickCallBack)
		m_buttonClickCallBack(node, e);
}
