// Created by wangwenjie on 2013/12

#include "PrincessModel.h"
#include "Common.h"
#include "GameManager.h"

USING_NS_CC;

#define MAKEUP_SOUND_TIME		1.5f
#define GLOVE_ALTER_BASE_SLOT	"glove003"
#define GLOVE_ALTER_BASE_SLOT_2	"glove004"

#define ANIM_HAIR				"aniSmile"
#define ANIM_SMILE				"aniSmile"
#define ANIM_COVER_SMILE		"aniCoversmile"
#define ANIM_LOOKUP				"aniLookup"
#define ANIM_LOOKDOWN			"aniLookdown"
#define ANIM_NECKLACE			"aniSalute"
#define ANIM_WAVE				"aniWave"

#define MAKEUP_PARTICLE			"particles/change02.plist"
#define CLOTHES_BASE			"game/03_minigame/p001/clothes/"

#define BASE_NONE_PNG           "base/none.png"

#define PRINCESS_MODEL_NORMAL_ANI        "aniIdle"     // 公主播放的常态动画

PrincessModel* PrincessModel::create(bool playIdle)
{
    PrincessModel* model = new PrincessModel();
	model->m_modelNo = 1;
	model->m_playIdle = playIdle;
    model->autorelease();
    model->init();

    return model;
}

bool PrincessModel::init()
{
	WJUtils::randomize();
	if (!ModelBase::init())
        return false;

	// 默认在通道一播放的常态动画
	setPlayNameWithChannel1(PRINCESS_MODEL_NORMAL_ANI);

	/* 加载保存的状态 */
	restoreSavedStatus();

	/* 设置动画的EventTrigger */
	m_skeleton->setEventListener(CC_CALLBACK_2(PrincessModel::onSkeletonHandEvent, this, m_skeleton));

	scheduleUpdate();

    return true;
}

PrincessModel::PrincessModel()
	: m_makeupSoundPlaying(false), m_makeupSoundPlayingTime(0.0f), m_makeupSoundDuration(0.0f)
{
	m_curAggregateStr.clear();
	m_curSoundVector.clear();
	m_perNormalIndex = -1;
	m_standbyIndex = -1;
	m_playSound = false;
	m_slotHead = nullptr;
	m_boneRoot = nullptr;
	m_faceBoneDefaultRotation = 0.0f;
	m_modelTopLayerHead = nullptr;
	m_faceRenderTexture = nullptr;
}

void PrincessModel::onEnter()
{
    ModelBase::onEnter();
}

void PrincessModel::onEnterTransitionDidFinish()
{
    ModelBase::onEnterTransitionDidFinish();
	
	playPrincessIdleAni();
}

void PrincessModel::onExit()
{
	this->unscheduleUpdate();
	ModelBase::onExit();
}

void PrincessModel::update(float fDelta)
{
	ModelBase::update(fDelta);

	if (m_makeupSoundPlaying)
	{
		m_makeupSoundPlayingTime += fDelta;
		if (m_makeupSoundPlayingTime > m_makeupSoundDuration)
		{
			m_makeupSoundPlaying = false;
			m_makeupSoundPlayingTime = 0.0f;
			m_makeupSoundDuration = 0.0f;
		}
	}
}

void PrincessModel::playPrincessIdleAni()
{
	if (m_playIdle)
		m_skeleton->playAnimation(PRINCESS_MODEL_NORMAL_ANI, true, 1);
}

void PrincessModel::restoreSavedStatus()
{
	onHairChanged(GManager->getMakeUpItemKeyFromName(HAIR_SLOT).c_str(), false);
}

void PrincessModel::restoreSavedMakeItem()
{
	onAccessoryChanged(GManager->getMakeUpItemKeyFromName(ACCESSORY_SLOT).c_str(), false);
	onCappaChanged(GManager->getMakeUpItemKeyFromName(CAPPA_SLOT).c_str(), false);
	onCrownChanged(GManager->getMakeUpItemKeyFromName(CROWN_SLOT).c_str(), false);
	onDressChanged(GManager->getMakeUpItemKeyFromName(DRESS_SLOT).c_str(), false);
	onGloveChanged(GManager->getMakeUpItemKeyFromName(GLOVE_SLOT).c_str(), false);
}

void PrincessModel::playMakeupSound(const std::string& soundName, float duration)
{
	if (m_makeupSoundPlaying)
		return;

	playSoundWithPlayAni(soundName.c_str());
	m_makeupSoundPlaying = true;
	m_makeupSoundDuration = duration;
}

void PrincessModel::playMakeupParticle(const char* particleName, int index)
{
	Rect rect = m_skeleton->getContentRectInWorld();

	Vec2 pos = Vec2::ZERO;
	if (index == 0)
		pos = Vec2(rect.origin.x + rect.size.width * 0.5f, rect.origin.y + rect.size.height * 0.85f);
	else if (index == 1)
		pos = Vec2(rect.origin.x + rect.size.width * 0.5f, rect.origin.y + rect.size.height * 0.75f);
	else if (index == 2)
		pos = Vec2(rect.origin.x + rect.size.width * 0.5f, rect.origin.y + rect.size.height * 0.6f);
	pos = m_skeleton->convertToNodeSpace(pos);

	ParticleSystemQuad *particle = ParticleSystemQuad::create(particleName);
	particle->setPosition(pos + Vec2(0, -180));
	particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	particle->setAutoRemoveOnFinish(true);
	m_skeleton->addChild(particle, 2000);
}

void PrincessModel::onSkeletonHandEvent(int stateIndex, spEvent* pEvent, WJSkeletonAnimation *skeleton)
{
	if (!pEvent || !pEvent->stringValue)
		return;

	if (strcmp(pEvent->data->name, "handy") != 0 && strcmp(pEvent->data->name, "handz") != 0)
		return;
	if (strlen(pEvent->stringValue) <= 0)
		return;

	 std::string key = GManager->getMakeUpItemKeyFromName(GLOVE_SLOT);
	// 当前戴了手套
	if (!WJUtils::equals("" , key.c_str()))
	{
		GloveTemplate object = GloveTemplateTem(key);
		// 清空右手(glove03)
        skeleton->setSlotTexture(GLOVE_ALTER_BASE_SLOT, BASE_NONE_PNG);

		if (strcmp(pEvent->stringValue, "glove011") == 0 || strcmp(pEvent->stringValue, "glove012") == 0)
		{
			/* 这两个事件需要同时操作左右手、清空 */
			skeleton->setSlotTexture(GLOVE_ALTER_BASE_SLOT_2, BASE_NONE_PNG);
		}

		// 清空变换用的手套点
		if (strcmp(pEvent->stringValue, "glove013") != 0)
		{
			cleanupChangeGloveSlot(skeleton);
		}
		
		if (strcmp(pEvent->stringValue, "glove011") == 0)
		{
			int gloveIndex = atoi(object.Key.c_str()+ 11);
			/* glove011事件中，需要加载glove09和glove011两个slot的图片 */
			skeleton->setSlotTexture("glove009", getGloveFileNameFromSlotIndex(gloveIndex, 9).c_str());
			skeleton->setSlotTexture("glove011", getGloveFileNameFromSlotIndex(gloveIndex, 11).c_str());
		}
		else if (strcmp(pEvent->stringValue, "glove012") == 0)
		{
			/* glove012事件中，需要加载glove03和glove04两个slot的图片 */
			skeleton->setSlotTexture("glove003", (object.filepath + object.slot3File).c_str());
			skeleton->setSlotTexture("glove004", (object.filepath + object.slot4File).c_str());
		}
		else
		{
			/* 通常情况 */
			int alterSlot = atoi(pEvent->stringValue + 5);
			int gloveIndex = atoi(object.Key.c_str()+ 11);
			std::string alterSlotFileName = getGloveFileNameFromSlotIndex(gloveIndex, alterSlot);

			/* aniChops01hand 02hand 03hand 这三个动画，都不加载左手的手套 */
			const char* aniName = skeleton->getPlayingAnimationName(stateIndex);
			if (strcmp(aniName, "aniChops01hand") == 0 ||
				strcmp(aniName, "aniChops02hand") == 0 ||
				strcmp(aniName, "aniChops03hand") == 0)
			{
				if (alterSlot == 4)
				{
					return;
				}
			}

			skeleton->setSlotTexture(WJUtils::stringAddInt("glove", alterSlot, 3).c_str(), alterSlotFileName.c_str());
		}
    }
}

std::string PrincessModel::getGloveFileNameFromSlotIndex(int gloveIndex, int slot)
{
	std::string directoryName(CLOTHES_BASE);
	directoryName.append("glove");


	WJUtils::stringAddInt(directoryName, slot, 3);
	directoryName.append("/");


	std::string fileName = WJUtils::stringAddInt("cloth", gloveIndex, 3).append(WJUtils::stringAddInt("_glove", slot, 3).c_str());
	fileName.append(".png");

	std::string fullFileName = directoryName + fileName;
	std::string fullPath = FileUtils::getInstance()->fullPathForFilename(fullFileName);
	if (fullPath.length() == 0)
		return BASE_NONE_PNG;
	else
		return directoryName.append(fileName);
	
}

void PrincessModel::setPrincessModelClickEvent()
{
	m_skeleton->setClickAble(true);
	m_skeleton->setOnClick(CC_CALLBACK_2(PrincessModel::onClickPrincessModel, this));
	m_skeleton->setTouchSwallowsTouches(false);
	m_skeleton->setCustomizeTouchRect(Rect(-135, 70, 280, 500), true);
	m_skeleton->noClickMoveEffect();

	//for test
	//LayerColor *colorLayerHead = LayerColor::create(Color4B(0, 255, 255, 80));
	//colorLayerHead->setContentSize(Size(280, 500));
	//colorLayerHead->setPosition(Vec2(-135, 70));
	//m_skeleton->addChild(colorLayerHead);
}

void PrincessModel::onClickPrincessModel(Node *node, WJTouchEvent *event)
{
	WJSkeletonAnimation *skeleton = (WJSkeletonAnimation*)node;
	// 当前已经指定了公主点击播放的动画和声音
	if (m_curAggregateStr.size() > 1)
	{
		startPlayClickStandbyAnimation();
	}
}

void PrincessModel::setPrincessPlaySound( bool play )
{
	m_playSound = play;
}

void PrincessModel::onAccessoryChanged( const char *key, bool save)
{
	if (WJUtils::equals(key, ""))
		return;

	// 更换项链
	ScrollItemTemplate object = ScrollItemTemplateTem(key);
	m_skeleton->setSlotTexture(ACCESSORY_SLOT, object.itemFile.c_str());
	// 保存
	if (save)
	{
		Common::sound.play("Common:0067");
		GManager->setMakeUpItemKey(ACCESSORY_SLOT, key);
	}
	playMakeUpEndAniAndParticle(save, playRandomAnimal("start", "aniLookup", "aniCoversmile", "aniLookdown2", "end").c_str());
}

void PrincessModel::cleanupAccessory( const char * key )
{
	m_skeleton->setSlotTexture(ACCESSORY_SLOT, BASE_NONE_PNG);
	GManager->setMakeUpItemKey(ACCESSORY_SLOT, "");
}

void PrincessModel::onCappaChanged(const char * key, bool save)
{
	if (WJUtils::equals(key, ""))
		return;

	// glove05 glove06(左右护肩)
	CappaTemplate object = CappaTemplateTem(key);
	m_skeleton->setSlotTexture(object.leftSlotName.c_str(), object.leftFile.c_str());
	m_skeleton->setSlotTexture(object.rightSlotName.c_str(), object.rightFile.c_str());
	m_skeleton->setSlotTexture(object.specilSlotName.c_str(), object.specilFile.c_str());
	if (save)
	{
		Common::sound.play("Common:0066");
		GManager->setMakeUpItemKey(CAPPA_SLOT, key);
	}
	playMakeUpEndAniAndParticle(save, playRandomAnimal("start", "aniWow", "aniWave", "aniCoversmile", "aniSmile", "end").c_str());
}

void PrincessModel::cleanupCappa( const char * key )
{
	// glove05 glove06(左右护肩)
	CappaTemplate object = CappaTemplateTem(key);
	m_skeleton->setSlotTexture(object.leftSlotName.c_str(), BASE_NONE_PNG);
	m_skeleton->setSlotTexture(object.rightSlotName.c_str(), BASE_NONE_PNG);
	m_skeleton->setSlotTexture(object.specilSlotName.c_str(), BASE_NONE_PNG);
	GManager->setMakeUpItemKey(CAPPA_SLOT, "");
}

void PrincessModel::onCrownChanged( const char * key, bool save /*= true*/ )
{
	if (WJUtils::equals(key, ""))
		return;

	CrownTemplate object = CrownTemplateTem(key);
	m_skeleton->setSlotTexture(object.firstSlotName.c_str(), object.firstFile.c_str());
	m_skeleton->setSlotTexture(object.SecondSlotName.c_str(), object.secondFile.c_str());

	if (save)
	{
		Common::sound.play("Common:0066");
		GManager->setMakeUpItemKey(CROWN_SLOT, key);
	}
	playMakeUpEndAniAndParticle(save, playRandomAnimal("start", "aniLookup", "aniCoversmile", "end").c_str());
}

void PrincessModel::cleanupCrown( const char *key )
{
	m_skeleton->setSlotTexture(CROWN_SLOT, BASE_NONE_PNG);
	m_skeleton->setSlotTexture(CROWN_SLOT_1, BASE_NONE_PNG);
	GManager->setMakeUpItemKey(CROWN_SLOT, "");
}

void PrincessModel::onDressChanged( const char * key, bool save /*= true*/ )
{
	// 默认的衣服
	if (WJUtils::equals(key, ""))
		key = DRESS_DEFAULT_KEY;

	// 替换衣服
	ClothTemplate object = ClothTemplateTem(key);
	m_skeleton->setSlotTexture(object.slot1Name.c_str(), (object.filepath + object.slot1File).c_str());
	m_skeleton->setSlotTexture(object.slot2Name.c_str(), (object.filepath + object.slot2File).c_str());
	m_skeleton->setSlotTexture(object.slot3Name.c_str(), (object.filepath + object.slot3File).c_str());
	m_skeleton->setSlotTexture(object.slot4Name.c_str(), (object.filepath + object.slot4File).c_str());
	m_skeleton->setSlotTexture(object.slot5Name.c_str(), (object.filepath + object.slot5File).c_str());
	m_skeleton->setSlotTexture(object.slot6Name.c_str(), (object.filepath + object.slot6File).c_str());
	if (save)
	{
		Common::sound.play("Common:0066");
		GManager->setMakeUpItemKey(DRESS_SLOT, key);
	}
	playMakeUpEndAniAndParticle(save, playRandomAnimal("start", "aniSmile", "aniCoversmile", "aniNod", "aniShoke", "end").c_str());
}

void PrincessModel::onGloveChanged(const char * key, bool save)
{
	if (WJUtils::equals(key, ""))
		return;

	// 替换手套
	GloveTemplate object = GloveTemplateTem(key);
	m_skeleton->setSlotTexture(object.slot1Name.c_str(), (object.filepath + object.slot1File).c_str());
	m_skeleton->setSlotTexture(object.slot2Name.c_str(), (object.filepath + object.slot2File).c_str());
	m_skeleton->setSlotTexture(object.slot3Name.c_str(), (object.filepath + object.slot3File).c_str());
	m_skeleton->setSlotTexture(object.slot4Name.c_str(), (object.filepath + object.slot4File).c_str());
	if (save)
	{
		Common::sound.play("Common:0066");
		GManager->setMakeUpItemKey(GLOVE_SLOT, key);
	}
	playMakeUpEndAniAndParticle(save, playRandomAnimal("start", "aniWave", "aniLookdown", "aniCoversmile", "end").c_str());
}

void PrincessModel::cleanupChangeGloveSlot(WJSkeletonAnimation *skeleton)
{
	/* 将手套的所有的变换用Slot清空 */
	// glove07、glove08、glove09、glove010、glove011、glove012、glove013
	for (int iSlot = 6; iSlot < 13; ++iSlot)
	{
		std::string slotName(GLOVE_SLOT);
		WJUtils::stringAddInt(slotName, iSlot + 1, 3);
		skeleton->setSlotTexture(slotName.c_str(), BASE_NONE_PNG);
	}
}

void PrincessModel::playMakeUpEndAniAndParticle(bool save, const char * animalName)
{
	if (!save)
		return;

	playAnimation(animalName, false, 1);
	playMakeupParticle(MAKEUP_PARTICLE, 1);
}

void PrincessModel::cleanupGlove( const char * key )
{
	// 清空变换用的手套slot和基本的手套slot
	GloveTemplate object = GloveTemplateTem(key);
	m_skeleton->setSlotTexture(object.slot1Name.c_str(), BASE_NONE_PNG);
	m_skeleton->setSlotTexture(object.slot2Name.c_str(), BASE_NONE_PNG);
	m_skeleton->setSlotTexture(object.slot3Name.c_str(), BASE_NONE_PNG);
	m_skeleton->setSlotTexture(object.slot4Name.c_str(), BASE_NONE_PNG);
	cleanupChangeGloveSlot(m_skeleton);
	GManager->setMakeUpItemKey(GLOVE_SLOT, "");
}

void PrincessModel::onHairChanged( const char * key, bool save /*= true*/ )
{
	if (WJUtils::equals(key, ""))
		key = HAIR_DEFAULT_KEY;

	// 还原后重新加载
	m_skeleton->setSkinToNull();
	m_skeleton->setToSetupPose();
	restoreSavedMakeItem();

	// 设置皮肤
	ScrollItemTemplate object = ScrollItemTemplateTem(key);
	int index = atoi(key + 10);
	m_skeleton->setSkin(WJUtils::stringAddInt("hair", index, 3).c_str());
	if (save)
	{
		Common::sound.play("Common:0066");
		GManager->setMakeUpItemKey(HAIR_SLOT, key);
	}
	playMakeUpEndAniAndParticle(save, playRandomAnimal("start", "aniHappy", "aniSmile", "aniCoversmile", "end").c_str());
}

void PrincessModel::initMakeFaceRender(RenderTexture *render, WJLayer *topLayer)
{
	spSkeleton_updateWorldTransform(m_skeleton->getSkeleton());

	Vec2 anchorPoint;
	Vec2 pos;
	Size size;
	// head layer
	m_boneRoot = m_skeleton->findBone("root");
	m_slotHead = m_skeleton->findSlot("layer");
	m_skeleton->getSlotInfo(m_slotHead, &anchorPoint, &pos, &size, &m_faceBoneDefaultRotation);
	// top layer
	m_modelTopLayerHead = WJLayer::create();
	m_modelTopLayerHead->setContentSize(size);
	m_modelTopLayerHead->ignoreAnchorPointForPosition(false);
	m_modelTopLayerHead->setAnchorPoint(anchorPoint);
	m_modelTopLayerHead->setPosition(pos);
	m_modelTopLayerHead->setVisible(false);
	this->addChild(m_modelTopLayerHead, 100);

	// 头部layer的item
	const Size &oldSize = topLayer->getContentSize();
	WJSprite *sprite;
	WJSprite *newSprite;
	Vector<Node*> _vectorNode = topLayer->getChildren();
	for (int i = 0; i < _vectorNode.size(); i++)
	{
		sprite = (WJSprite*)_vectorNode.at(i);
		sprite->setClickAble(false);
		sprite->setMoveAble(false);
		newSprite = (WJSprite*)sprite->clone();
		if (newSprite)
		{
			float x = (float(sprite->getPositionX() / oldSize.width)) * size.width;
			float y = (float(sprite->getPositionY() / oldSize.height)) * size.height;
			newSprite->setPosition(Vec2(x, y));
			newSprite->setScaleX(float(size.width / oldSize.width));
			newSprite->setScaleX(float(size.height/ oldSize.width));
			m_modelTopLayerHead->addChild(newSprite, sprite->getLocalZOrder());
		}
	}

	//// test
	//LayerColor *colorLayerHead = LayerColor::create(Color4B(0, 255, 255, 80));
	//colorLayerHead->setContentSize(size);
	//m_modelTopLayerHead->addChild(colorLayerHead);

	// 头部layer
	m_modelTopLayerHead->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_modelTopLayerHead->setScaleY(-m_modelTopLayerHead->getScaleY());

	// 画脸
	m_faceRenderTexture = RenderTexture::create(431, 494);
	m_faceRenderTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_faceRenderTexture->ignoreAnchorPointForPosition(false);
	m_faceRenderTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
	m_faceRenderTexture->retain();

	m_faceRenderTexture->beginWithClear(0, 0, 0, 0);
	visitNodeWithMakeFace(render);
	visitNodeWithMakeFace(m_modelTopLayerHead);
	m_faceRenderTexture->end();
	Director::getInstance()->getRenderer()->render();

	getSkeleton()->setSlotTexture("dress", m_faceRenderTexture->getSprite()->getTexture());
}

void PrincessModel::visitNodeWithMakeFace(Node *node)
{
	node->setVisible(true);
	node->setPosition(Vec2(431 / 2, 494 / 2));
	node->visit();
	node->setVisible(false);
}

PrincessModel::~PrincessModel()
{
	CC_SAFE_RELEASE(m_faceRenderTexture);
}

void PrincessModel::playAnimation( const char* animationName, bool loop, int stateIndex )
{
	ModelBase::playAnimation(animationName, loop, stateIndex);
}

void PrincessModel::onCompletedListener( int trackIndex, int loopCount )
{
	if (trackIndex == 1 && loopCount == 1)
	{
		std::string name = m_skeleton->getAnimationName(trackIndex);
		if (!WJUtils::equals(m_playNameWithChannel1.c_str(), name.c_str()))
		{
			playAnimation(m_playNameWithChannel1.c_str(), true, 1);
		}
	}
}
