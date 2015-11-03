// Created by wangwenjie on 2013/12

#include "ShowStar.h"
#include "Loading.h"
#include "GameCommand.h"
#include "GameManager.h"

USING_NS_CC;

#define SCALE_ACTION_WITH_STAR_TAG             5536               // 当前星星嵌入进去后整个json都整体向下做个动画

ShowStar* ShowStar::create(ProgressStar progress, bool playStarAniAfter, GameNumber number)
{
    ShowStar *show = new ShowStar();
    show->autorelease();
    show->init(progress, playStarAniAfter, number);

    return show;
}

bool ShowStar::init(ProgressStar progress, bool playStarAniAfter, GameNumber number)
{
	m_progressStar = progress;
	m_returnScenePlayStarAni = playStarAniAfter;
	m_returnGameNumber = number;

	if (!WJLayerJson2x::initFromJsonFile("game/json/stars.json"))
		return false;

	NotificationCenter::getInstance()->postNotification("showcompleted");

	const Size &size = Director::getInstance()->getWinSize();
	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->ignoreAnchorPointForPosition(false);
	this->setPosition(Vec2(size.width / 2, size.height / 2));

	/* 禁止再次操作界面 */
	WJLayer *maskLayer = WJLayer::create();
	maskLayer->noClickMoveEffect();
	maskLayer->setClickAble(true);
	this->addChild(maskLayer, 2);
	
    return true;
}

void ShowStar::onEnter()
{
	WJLayerJson::onEnter();
}

void ShowStar::onExit()
{
	WJLayerJson::onExit();
}

void ShowStar::onEnterTransitionDidFinish()
{
	Common::sound.stopAll();
	WJLayerJson::onEnterTransitionDidFinish();
	Common::sound.play("Common:0046_princess");
	Common::sound.play("Common:0005");
	playStarAni();
}

void ShowStar::gotoHomeScheduce(float delay)
{
	this->scheduleOnce(CC_SCHEDULE_SELECTOR(ShowStar::gotoHomePage), delay);
}

void ShowStar::playStarAni()
{
	if (m_progressStar == ProgressStar::noStar)
		gotoHomeScheduce();
	else
		playStarSpriteAni(this->getSubSpriteByKey("star01"));
}

ShowStar::ShowStar()
{
	m_returnScenePlayStarAni = false;
	m_returnGameNumber = GameNumber::P014_Main;
	m_diySaveKey = "";
}

void ShowStar::gotoHomePage(float time)
{
	// 返回主场景播放星星动画
	if (m_returnScenePlayStarAni && m_progressStar >= ProgressStar::oneStar)
	{
		//　保存星星
		GameStarCommand *command = GameStarCommand::create();
		command->setStarNum(m_progressStar);
		GManager->pushGameCommandIntoGame(command, true);

		// diy
		if (!WJUtils::equals(m_diySaveKey, ""))
		{
			GameShowDiyCommand *diyCommand = GameShowDiyCommand::create();
			diyCommand->setDiySaveKey(m_diySaveKey);
			GManager->pushGameCommandIntoGame(diyCommand);
		}
	}
	Loading::gotoScene(m_returnGameNumber);
}

void ShowStar::playStarSpriteAni(WJSprite *sprite)
{
	sprite->runAction(Sequence::create(
		EaseBounceOut::create(ScaleTo::create(0.3f, 1.8f)),
		DelayTime::create(0.1f),
		Spawn::create(
			ScaleTo::create(0.25f, 1.0f),
			RotateBy::create(0.25f, 360),
			CallFuncN::create(CC_CALLBACK_1(ShowStar::onPlayParticle, this)),
		NULL),
		CCCallFunc::create(CC_CALLBACK_0(ShowStar::playSceneScaleAni, this)),
		DelayTime::create(0.15f),
		CallFuncN::create(CC_CALLBACK_1(ShowStar::onPlayStarEnded, this)),
		NULL));
}

void ShowStar::onPlayStarEnded(Node *node)
{
	WJSprite *sprite  = (WJSprite*)node;
	const char * key = this->getSubKeyByNode(node);

	if (m_progressStar == ProgressStar::oneStar ||
		(m_progressStar == ProgressStar::twoStar && WJUtils::equals(key, "star02")) ||
		(m_progressStar == ProgressStar::threeStar && WJUtils::equals(key, "star03")))
	{
		gotoHomeScheduce();
	}
	else
	{
		int starNo = (atoi)(key + 4);
		playStarSpriteAni(this->getSubSpriteByKey(WJUtils::stringAddInt("star", starNo + 1, 2).c_str()));
	}
}

void ShowStar::playSceneScaleAni()
{
	Common::sound.play("Common:0018");
	this->stopActionByTag(SCALE_ACTION_WITH_STAR_TAG);
	this->runAction(Sequence::create(
		ScaleTo::create(0.1f, 0.8f),
		ScaleTo::create(0.1f, 1.0f),
		NULL))->setTag(SCALE_ACTION_WITH_STAR_TAG);
}

void ShowStar::onPlayParticle(Node *node)
{
	WJSprite *sprite  = (WJSprite*)node;
	// 播放粒子效果
	playStarParticle(sprite, "particles/starEnd001.plist");
	playStarParticle(sprite, "particles/starEnd002.plist");
}

void ShowStar::playStarParticle(WJSprite *sprite, const char * file)
{
	// 播放粒子效果
	ParticleSystemQuad *particle = ParticleSystemQuad::create(file);
	particle->setPosition(sprite->getPosition());
	particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	sprite->getParent()->addChild(particle, sprite->getLocalZOrder() + 10);
}

void ShowStar::show( Node *owner, int zOrder )
{
	LBModalDialog *dialog = LBModalDialog::create(this, true);
	owner->addChild(dialog, zOrder);
	dialog->setKeyEscapeClose(false);
	dialog->setOnDialogShown(CC_CALLBACK_1(ShowStar::onDialogShown, this));
	dialog->show(LBModalDialogAnimationType::Scale);

	// android 下面back返回的屏蔽
	PBase *base = dynamic_cast<PBase*>(owner);
	if (base)
		base->setEnabled(false);

	// 粒子效果
	const Size &size = Director::getInstance()->getWinSize();
	ParticleSystemQuad *particle = ParticleSystemQuad::create("particles/commonFirework.plist");
	particle->setPosition(dialog->convertToNodeSpace(Vec2(size.width / 2, size.height / 2)));
	particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	dialog->addChild(particle, 50);
}

void ShowStar::onDialogShown(LBModalDialog *dialog)
{
	const Size &size = Director::getInstance()->getWinSize();
	ParticleSystemQuad *particle = ParticleSystemQuad::create("particles/opennning.plist");
	particle->setPosition(dialog->convertToNodeSpace(Vec2(size.width / 2, size.height / 2)));
	particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	dialog->addChild(particle, 50);
}

//-----------------------------------------保存图片-----------------------------------------------------------
bool ShowStar::saveItem(DiySaveType type, Node *node)
{
	if (type == DiySaveType::other || !node)
		return false;

	int index = (int)type;
	SaveItemTemplate object = SaveItemTemplateTem(WJUtils::stringAddInt("Save_", index, 3).c_str());
	RenderTexture *renderTexture = visitNode(type, node);
	
	//// test ??
	//node->retain();
	//node->removeFromParentAndCleanup(false);
	//const Size &sizeStar = this->getContentSize();
	//node->setVisible(true);
	//node->setPosition(Vec2(sizeStar.width / 2, sizeStar.height / 2) + Vec2(-300, -600));
	//this->addChild(node, 1000);
	//node->release();

	// 取得当前已经保存过的总数
	std::string totalName = object.savePreKey;
	std::string sumName = object.savePreKey;
	int sum = UserDefault::getInstance()->getIntegerForKey(sumName.append("Sum").c_str(), 0);

	//保存png
	std::string fileName = object.savePreKey;
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

	std::string fullPath = GameManager::getAtlasSavePath() + fileName;
	Image *image = renderTexture->newImage(true);
	WJGraphics::unblendAlpha(image);
	bool result = image->saveToFile(fullPath, false);
	CC_SAFE_DELETE(image);

	// 保存照片成功
	if (result)
	{
		int nowIndex = (sum + 1) % object.count;
		int exitUsed = 0;
		// 需要覆盖以前保存的旧的图片
		if (sum + 1 > object.count)
		{
			// 判断当前覆盖的图片是否在使用？？
			/*if (fileIsUsedFromPrincess(nowIndex))
			{
			exitUsed = 1;
			nowIndex = nowIndex + 1 > object.count ? 1 : nowIndex + 1;
			}*/
			totalName.append(WJUtils::stringAddInt("", nowIndex == 0 ? object.count : nowIndex).c_str());
			std::string oldFileName = UserDefault::getInstance()->getStringForKey(totalName.c_str(), "");
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
			totalName.append(WJUtils::stringAddInt("", nowIndex == 0 ? object.count : nowIndex).c_str());
		}

		// 保存当前diy的序列
		UserDefault::getInstance()->setStringForKey(totalName.c_str(), fileName);
		UserDefault::getInstance()->flush();

		// 记录当前保存的diy的key
		m_diySaveKey = totalName;

		// 保存当前diy类别的总的个数
		UserDefault::getInstance()->setIntegerForKey(sumName.c_str(), sum + 1 + exitUsed);
		UserDefault::getInstance()->flush();
	}

	return result;
}

RenderTexture* ShowStar::visitNode(DiySaveType type, Node *node)
{
	int index = (int)type;
	SaveItemTemplate object = SaveItemTemplateTem(WJUtils::stringAddInt("Save_", index, 3).c_str());

	//定义一个自定义大小的渲染纹理
	float scale = object.scale;
	const Size &size = node->getContentSize() * scale;
	RenderTexture *renderTexture = RenderTexture::create(size.width, size.height);
	renderTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
	renderTexture->ignoreAnchorPointForPosition(false);

	Vec2 ancPoint = node->getAnchorPoint();
	float scaleOld = node->getScale();
	Vec2 point = node->getPosition();

	// 按照高度的比例缩放节点到当前的尺寸
	node->setAnchorPoint(Vec2(0.5f, 0.5f));
	node->ignoreAnchorPointForPosition(false);
	node->setScale(scale);

	renderTexture->beginWithClear(0, 0, 0, 0);
	node->setPosition(Vec2(size.width / 2, size.height / 2));
	node->visit();
	renderTexture->end();
	Director::getInstance()->getRenderer()->render();

	node->setAnchorPoint(ancPoint);
	node->setPosition(point);
	node->setScale(scaleOld);

	return renderTexture;
}

//-----------------------------------------保存图片-----------------------------------------------------------
