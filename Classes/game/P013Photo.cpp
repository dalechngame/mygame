#include "P013Photo.h"
#include "GameManager.h"
#include "ZoomLayer.h"
#include "ShowPhotoDialog.h"

static const float PHOTOS_SCALE_NUM  = 0.175f;

INIT_SCENE_LOADER_SYNC(P013);

static bool s_firstIntoP013 = true;

P013::P013()
{

}

P013::~P013()
{

}

bool P013::init()
{
	if ( !PBase::init())
	{
		return false;
	}

	initSceneJson("game/json/p013.json");

	setNodeVisible();

	initPhotos();

	return true;
}

void P013::setNodeVisible()
{
	m_sceneJson->setOnClick(CC_CALLBACK_2(P013::onClickScene, this));
	m_sceneJson->noClickMoveEffect();
}

void P013::onClickScene(Node *node, WJTouchEvent *event)
{
	PUtils::playParticle("particles/puzzledrag.plist", m_sceneJson, m_sceneJson->convertTouchToNodeSpace(event->touch), 1000);
}

GameNumber P013::getGameNumber()
{
	return GameNumber::P013;
}

void P013::onEnterTransitionDidFinish()
{
	PBase::onEnterTransitionDidFinish();
	this->runAction(RepeatForever::create(Sequence::create(
		DelayTime::create(8.0f),
		CCCallFunc::create([](){
			Common::sound.play("Common:0053_princess");
		}),
		nullptr)));
	this->runAction(Sequence::create(
		DelayTime::create(1.0f),
		CCCallFunc::create([&](){
			if (s_firstIntoP013)
			{
				s_firstIntoP013 = false;
				Common::sound.play("Common:0051_princess");
			}
		}),
		nullptr));
	// 
	this->runAction(Sequence::create(
		DelayTime::create(3.0f),
		CCCallFunc::create([&]() {
			showPhotosTips(3.0f);
			this->schedule(CC_SCHEDULE_SELECTOR(P013::showPhotosTips), 6.0f);
		}),
		nullptr));
}

void P013::showPhotosTips(float time)
{
	// 一进入游戏就延迟3s q弹。待机超过6s后q弹
	if (m_photoVector.size() >= 1)
	{
		// 最上面的一个照片提示
		PUtils::playNodeScaleAni(m_photoVector.at(m_photoVector.size() - 1));

		// 一进入游戏就延迟3s出手指，从照片处，指向任意没有照片的方框处。
		DottedSprite *sprite;
		for (int i = 0; i < m_lineVector.size(); i++)
		{
			sprite = m_lineVector.at(i);
			if (!sprite->getHavingPhoto())
			{
				startMoveFingerTips(m_photoVector.at(m_photoVector.size() - 1)->getPositionWorld(), sprite->getPositionWorld());
				return;
			}
		}

		// 全部都有照片显示则随机一个框的位置
		sprite = m_lineVector.at(WJUtils::randomInt(m_lineVector.size()));
		startMoveFingerTips(m_photoVector.at(m_photoVector.size() - 1)->getPositionWorld(), sprite->getPositionWorld(), 1000, 1);
	}
}

void P013::initPhotos()
{
	// 初始化已经添加进展示的图片
	m_lineVector.clear();
	for (int i = 1; i <= 6; i++)
	{
		std::string key = WJUtils::stringAddInt("line", i, 3);
		WJSprite *sprite = m_sceneJson->getSubSprite(key.c_str());

		DottedSprite *dotted = DottedSprite::create(sprite->getSpriteFileName(), i);
		m_sceneJson->replaceSubNode(key.c_str(), dotted, true);
		dotted->autoAddPhotosInto(i);
		m_lineVector.push_back(dotted);
	}

	m_sceneJson->setMoveAble(true);
	m_sceneJson->setTouchSwallowsTouches(false);
	m_sceneJson->setOnTouchAble(CC_CALLBACK_2(P013::onSceneTouchAble, this));
	m_sceneJson->setOnWillMoveTo(CC_CALLBACK_2(P013::onSceneWillMoveTo, this));

	m_photoVector.clear();
	// 初始化右下角保存的图片(先从旧到新创建出相片)
	int num = 0;
	for (int j = 1; j <= 6; j++)
	{
		std::string fileName = GameManager::getSavePhotosFileFromIndex(j);
		std::string path = GameManager::getAtlasSavePath() + fileName;
		if (!WJUtils::equals(fileName.c_str(), "") &&
			WJUtils::fileExists(path.c_str()))
		{
			WJSprite *sprite = WJSprite::create(path.c_str());
			sprite->setScale(PHOTOS_SCALE_NUM * (m_winSize.height / sprite->getContentSize().height));
			sprite->setMoveAble(true);
			sprite->setUserTag(j);
			sprite->noClickMoveEffect();
			sprite->setTouchSwallowsTouches(true);
			sprite->setOnTouchAble(CC_CALLBACK_2(P013::onPhotosTouchAble, this));
			sprite->setOnTouchEnded(CC_CALLBACK_2(P013::onPhotosTouchEnded, this));
			m_sceneJson->addChild(sprite, 100 + num);
			sprite->saveCurrentProperties();
			num++;
			m_photoVector.push_back(sprite);
		}
	}

	// 重新设置其位置
	int maxNum = m_photoVector.size();
	switch (maxNum)
	{
		case 1:
			copySavedSpritePos(m_photoVector.at(0), "plan001_1");
			break;
		case 2:
			copySavedSpritePos(m_photoVector.at(0), "plan002_1");
			copySavedSpritePos(m_photoVector.at(1), "plan002_2");
			break;
		case 3:
			copySavedSpritePos(m_photoVector.at(0), "plan003_1");
			copySavedSpritePos(m_photoVector.at(1), "plan003_2");
			copySavedSpritePos(m_photoVector.at(2), "plan003_3");
			break;
		case 4:
			copySavedSpritePos(m_photoVector.at(0), "plan004_1");
			copySavedSpritePos(m_photoVector.at(1), "plan004_2");
			copySavedSpritePos(m_photoVector.at(2), "plan004_3");
			copySavedSpritePos(m_photoVector.at(3), "plan004_4");
			break;
		case 5:
			copySavedSpritePos(m_photoVector.at(0), "plan005_1");
			copySavedSpritePos(m_photoVector.at(1), "plan005_2");
			copySavedSpritePos(m_photoVector.at(2), "plan005_3");
			copySavedSpritePos(m_photoVector.at(3), "plan005_4");
			copySavedSpritePos(m_photoVector.at(4), "plan005_5");
			break;
		case 6:
			copySavedSpritePos(m_photoVector.at(0), "plan006_1");
			copySavedSpritePos(m_photoVector.at(1), "plan006_2");
			copySavedSpritePos(m_photoVector.at(2), "plan006_3");
			copySavedSpritePos(m_photoVector.at(3), "plan006_4");
			copySavedSpritePos(m_photoVector.at(4), "plan006_5");
			copySavedSpritePos(m_photoVector.at(5), "plan006_6");
			break;
		default:
			break;
	}
}

void P013::copySavedSpritePos(WJSprite *sprite, const char * key)
{
	Node *node = m_sceneJson->getSubNode(key);
	Point pointWorld = node->getParent()->convertToWorldSpace(node->getPosition());
	Point point = m_sceneJson->convertToNodeSpace(pointWorld);
	sprite->setRotation(node->getRotation());
	sprite->setPosition(point);
	sprite->saveCurrentProperties();
}

bool P013::onPhotosTouchAble(Node *node, WJTouchEvent *event)
{
	WJSprite *sprite = (WJSprite*)node;
	stopMoveFingerTips();
	sprite->setLocalZOrder(5000);
	sprite->stopAllActions();
	sprite->restoreSavedScale(0.3f);
	sprite->setPosition(sprite->getParent()->convertTouchToNodeSpace(event->touch));
	sprite->runAction(RotateTo::create(0.3f, 0));
	NotificationCenter::getInstance()->postNotification("touchAblephotos");
	
	return true;
}

bool P013::onSceneTouchAble(Node *node, WJTouchEvent *event)
{
	PUtils::playParticle("particles/click.plist", m_sceneJson, m_sceneJson->convertTouchToNodeSpace(event->touch), 1000);
	return true;
}

bool P013::onSceneWillMoveTo(Node *node, WJTouchEvent *event)
{
	PUtils::playParticle("particles/click.plist", m_sceneJson, m_sceneJson->convertTouchToNodeSpace(event->touch), 1000);
	return false;
}


void P013::onPhotosTouchEnded(Node *node, WJTouchEvent *event)
{
	WJSprite *sprite  = (WJSprite*)node;
	NotificationCenter::getInstance()->postNotification("touchEndedphotos");

	// check 
	DottedSprite *lineSprite;
	for ( int i = 1; i <= 6; i++)
	{
		std::string key = WJUtils::stringAddInt("line", i, 3);
		lineSprite = (DottedSprite*)m_sceneJson->getSubSprite(key.c_str());

		const Size& size = lineSprite->getContentScaleSize();
		Rect rect = Rect(0, 0, size.width, size.height);
		Point point = lineSprite->convertToNodeSpace(sprite->getPositionWorld());
		if (rect.containsPoint(point))
		{
			sprite->stopAllActions();
			sprite->setMoveAble(false);
			sprite->runAction(Spawn::create(
				MoveTo::create(0.2f, lineSprite->getPosition()),
				RotateTo::create(0.2f,lineSprite->getRotation()),
				ScaleTo::create(0.2f, PHOTOS_SCALE_NUM),
				nullptr));
			sprite->setLocalZOrder(lineSprite->getLocalZOrder() + 1);
			lineSprite->removeLastPhotos(sprite);
			lineSprite->setHavingPhoto(true);
			lineSprite->resetIndexDotted(i);
			lineSprite->setHavingPhotos(sprite);
			GManager->putSavePhotosIntoShowPhotos(sprite->getUserTag(), i);
			// 粒子效果
			PUtils::playParticle("particles/place.plist", m_sceneJson, sprite->getPositionInNode(m_sceneJson), 1000);

			removePhotosFromVector(sprite);

			return;
		}
	}

	sprite->setMoveAble(false);
	sprite->restoreSavedPosition(0.3f);
	sprite->restoreSavedRotation(0.3f);
	sprite->runAction(Sequence::create(
		DelayTime::create(0.3f),
		CCCallFunc::create([=]{
			sprite->restoreSavedZOrder();
			sprite->setMoveAble(true);
		}),
		NULL));
}

void P013::removePhotosFromVector(WJSprite *sprite)
{
	if (m_photoVector.size() <= 0)
		return;

	WJSprite *_sprite;
	for (int i = 0; i < m_photoVector.size(); i++)
	{
		_sprite = m_photoVector.at(i);
		if (_sprite == sprite)
		{
			m_photoVector.erase(i + m_photoVector.begin());
			break;
		}
	}
}

void P013::onExit()
{
	if (this->isScheduled(CC_SCHEDULE_SELECTOR(P013::showPhotosTips)))
		this->unschedule(CC_SCHEDULE_SELECTOR(P013::showPhotosTips));
	PBase::onExit();
}

DottedSprite* DottedSprite::create( const char *pszFileName, int index )
{
	DottedSprite *pobSprite = new DottedSprite();
	if (pobSprite && pobSprite->initWithFile(pszFileName, index))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}

bool DottedSprite::initWithFile( const char *pszFilename, int index )
{
	if (!WJSprite::initWithFile(pszFilename, true))
		return false;
	m_itemIndex = index;
	// 取得当前展示的图片名称
	std::string fileName = GameManager::getShowPhotosFileFromIndex(m_itemIndex);
	m_havingPhoto = !WJUtils::equals(fileName, "");

	// 点击后展示大图
	this->setClickAble(true);
	this->setInvisibleTouchAble(true);
	this->setTouchSwallowsTouches(true);
	this->setOnClick(CC_CALLBACK_2(DottedSprite::onClickDottedSprite, this));

	return true;
}

void DottedSprite::onClickDottedSprite(Node *node, WJTouchEvent *event)
{
	if (!m_havingPhoto)
		return;

	Common::sound.play("Common:0052_princess");
	Common::sound.play("Common:popup");
	// 显示大图
	ShowPhotoDialog *dialog = ShowPhotoDialog::create("game/json/p013photoeframe.json", m_itemIndex);
	Director::getInstance()->getRunningScene()->addChild(dialog, 1000);
	dialog->show(LBModalDialogAnimationType::ScaleEaseBack);

	// 播放点击了展示的照片粒子特效
	PUtils::playParticle("particles/tapphotos.plist", this->getParent(), this->getPosition(), 1000);
}

void DottedSprite::onClickPhotoDialog(Node *node, WJTouchEvent *event, LBModalDialog *dialog)
{
	dialog->close();
	// 显示广告
	if (m_adsIsVisible && !WJUtils::isAdsVisible())
		Common::showAds();
}

void DottedSprite::update( float delta )
{
	WJSprite::update(delta);
}

void DottedSprite::autoAddPhotosInto(int index)
{
	if (m_havingPhoto)
	{
		std::string fileName = GameManager::getShowPhotosFileFromIndex(m_itemIndex);
		std::string fullpath = GameManager::getAtlasSavePath() + fileName;
		// 对应的照片
		WJSprite *photoSprite = WJSprite::create(fullpath.c_str());
		WJBase::copyProperties(this, photoSprite);
		photoSprite->getTexture()->setAntiAliasTexParameters();
		photoSprite->setScale(PHOTOS_SCALE_NUM);
		this->getParent()->addChild(photoSprite, this->getLocalZOrder() + 1);
		m_havingPhotoSprite = photoSprite;
	}
	else
	{
		this->setVisible(false);
	}
	resetIndexDotted(index);
}

void DottedSprite::resetIndexDotted(int index)
{
	this->setVisible(false);
	WJLayerJson *layerjson = (WJLayerJson*)this->getParent();
	//WJSprite *edgeSprite = layerjson->getSubSprite(WJUtils::stringAddInt("edges", index, 3).c_str());
	WJSprite *pinaSprite = layerjson->getSubSprite(WJUtils::stringAddInt("pin", index, 3).append("_a").c_str());
	WJSprite *pinbSprite = layerjson->getSubSprite(WJUtils::stringAddInt("pin", index, 3).append("_b").c_str());
	if (m_havingPhoto)
	{
		WJSprite *edgeSprite = layerjson->getSubSprite(WJUtils::stringAddInt("edges", index, 3).c_str());
		//if (edgeSprite)
		//	edgeSprite->setVisible(true);
		// 别针
		if (pinaSprite)
			pinaSprite->setVisible(true);
		if (pinbSprite)
			pinbSprite->setVisible(true);
	}
	else
	{
		//if (edgeSprite)
		//	edgeSprite->setVisible(false);
		// 别针
		if (pinaSprite)
			pinaSprite->setVisible(false);
		if (pinbSprite)
			pinbSprite->setVisible(false);
	}
}

void DottedSprite::onEnter()
{
	WJSprite::onEnter();

	NotificationCenter::getInstance()->addObserver(this,
		CC_CALLFUNCO_SELECTOR(DottedSprite::onPhotosTouchAbleSelector), "touchAblephotos", NULL);

	NotificationCenter::getInstance()->addObserver(this,
		CC_CALLFUNCO_SELECTOR(DottedSprite::onPhotosTouchEndedSelector), "touchEndedphotos", NULL);
}

void DottedSprite::onExit()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
	WJSprite::onExit();
}

void DottedSprite::onPhotosTouchAbleSelector(Ref *ref)
{
	if (m_havingPhoto)
		return;

	this->stopAllActions();
	this->runAction(RepeatForever::create(
		Sequence::create(
			Show::create(),
			DelayTime::create(0.5f),
			Hide::create(),
			DelayTime::create(0.5f),
			NULL)));
}

void DottedSprite::onPhotosTouchEndedSelector(Ref *ref)
{
	this->stopAllActions();
	this->setVisible(false);
}

void DottedSprite::removeLastPhotos(WJSprite *newSprite)
{
	if (!m_havingPhoto)
		return;

	// 存在旧的图片
	if (m_havingPhotoSprite)
	{
		Point toPoint = m_havingPhotoSprite->getPosition() + Vec2(1500, 0);
		if (m_itemIndex <= 3)
			toPoint = m_havingPhotoSprite->getPosition() - Vec2(1500, 0);
		m_havingPhotoSprite->stopAllActions();
		m_havingPhotoSprite->runAction(Sequence::create(
			Spawn::create(
				MoveTo::create(1.2f, toPoint),
				RotateBy::create(1.2f, 360),
				ScaleTo::create(1.2f, 0.3f),
				NULL),
			Hide::create(),
			RemoveSelf::create(),
			NULL));
	}
	m_havingPhotoSprite = newSprite;
}

DottedSprite::DottedSprite()
{
	m_havingPhotoSprite = nullptr;
	m_adsIsVisible = false;
}

void DottedSprite::setHavingPhotos( WJSprite *sprite )
{
	m_havingPhotoSprite = sprite;
}

