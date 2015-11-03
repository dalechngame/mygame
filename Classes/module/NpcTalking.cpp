// Created by wangwenjie on 2013/12

#include "NpcTalking.h"
#include "Common.h"
#include "GameManager.h"
//#include "CCTexture2D.h"
USING_NS_CC;

#define NPC_TALK_WAIT_TIME    1.0f       // 第一个人说话后的等待时间

// ShaderBlur
#define GUEST_PLAY_ACTION_TAG              8987        // 客人播放动画声音tag
#define PRINCESS_PLAY_ACTION_TAG           8988        // 公主播放动画声音tag

class SpriteBlur : public Sprite
{
public:
	~SpriteBlur();
	bool initWithTexture(Texture2D* texture, const Rect&  rect);
	void initGLProgram();

	static SpriteBlur* create(const char *pszFileName);
	static SpriteBlur* createWithTexture(Texture2D *pTexture);
	void setBlurRadius(float radius);
	void setBlurSampleNum(float num);

protected:
	float _blurRadius;
	float _blurSampleNum;
};

SpriteBlur::~SpriteBlur()
{
}

SpriteBlur* SpriteBlur::create(const char *pszFileName)
{
	SpriteBlur* pRet = new (std::nothrow) SpriteBlur();
	if (pRet && pRet->initWithFile(pszFileName))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool SpriteBlur::initWithTexture(Texture2D* texture, const Rect& rect)
{
	_blurRadius = 0;
	if( Sprite::initWithTexture(texture, rect) ) 
	{
#if CC_ENABLE_CACHE_TEXTURE_DATA
		auto listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event){
			setGLProgram(nullptr);
			initGLProgram();
		});

		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif

		initGLProgram();

		return true;
	}

	return false;
}

void SpriteBlur::initGLProgram()
{
	GLchar * fragSource = (GLchar*) String::createWithContentsOfFile(
		FileUtils::getInstance()->fullPathForFilename("Shaders/example_Blur.fsh").c_str())->getCString();
	auto program = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource);

	auto glProgramState = GLProgramState::getOrCreateWithGLProgram(program);
	setGLProgramState(glProgramState);

	auto size = getTexture()->getContentSizeInPixels();
	getGLProgramState()->setUniformVec2("resolution", size);
	getGLProgramState()->setUniformFloat("blurRadius", _blurRadius);
	getGLProgramState()->setUniformFloat("sampleNum", 7.0f);
}

void SpriteBlur::setBlurRadius(float radius)
{
	_blurRadius = radius;
	getGLProgramState()->setUniformFloat("blurRadius", _blurRadius);
}

void SpriteBlur::setBlurSampleNum(float num)
{
	_blurSampleNum = num;
	getGLProgramState()->setUniformFloat("sampleNum", _blurSampleNum);
}

SpriteBlur* SpriteBlur::createWithTexture(Texture2D *pTexture)
{
		Rect rect = Rect(0, 0, 0, 0);
		rect.size = pTexture->getContentSize();
		SpriteBlur* pRet = new SpriteBlur();
		if (pRet && pRet->initWithTexture(pTexture,rect))
		{
			pRet->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(pRet);
		}
		return pRet;
}

NpcTalking* NpcTalking::create(WJLayerJson *json, GameNumber game)
{
	NpcTalking *talk = new NpcTalking();
	talk->autorelease();
	talk->init(json, game);

	return talk;
}

bool NpcTalking::init(WJLayerJson *json, GameNumber game)
{
	if (!WJLayer::init())
		return false;

	WJUtils::randomize();
	m_gameNumber = game;
	this->setContentSize(Size(1363, 768));
	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->ignoreAnchorPointForPosition(false);
	getAnimalKeyFromGameNumber();

	AnimeTemplate object = AnimeTemplateTem(m_playKey.c_str());

	// 保存当前npc的客人编号
	/*PBase *base = (PBase*)json->getParent();
	base->setNpcGuest(object.guestModel);*/

	// 保存当前需要救治的人物
	m_healthModelIndex = object.healthModel;

	const Size& size = this->getContentSize();
	/* 禁止再次操作界面 */
	WJLayer *maskLayer = WJLayer::create();
	maskLayer->noClickMoveEffect();
	maskLayer->setClickAble(true);
	this->addChild(maskLayer);

	// skip按钮
	const Size &winSize = Director::getInstance()->getWinSize();
	m_skipButton = WJButton::create("game/01_common/skip.png");
	m_skipButton->setNormalAniType(WJButton::NormalAniType::QBreathing);
	m_skipButton->setClickAniType(WJButton::ClickAniType::Scale);
	m_skipButton->setOnClick(CC_CALLBACK_2(NpcTalking::onClickSkipButton, this));
	m_skipButton->setOpacity(0);
	this->addChild(m_skipButton, 100);

	ModelTemplate princesModel = ModelTemplateTem("model1");
	// 公主
	m_princess = PrincessModel::create(false);
	m_princess->setPosition(Vec2(size.width / 2 - 200, princesModel.npcPosY));
	m_princess->getSkeleton()->setOpacity(0);
	m_princess->playAnimation("aniIdle", true, 1);
	m_princess->getSkeleton()->setCompleteListener(CC_CALLBACK_2(NpcTalking::onSkeletonCompleted, this, m_princess->getSkeleton()));
	m_princess->setSoundIsBindAni(false);
	this->addChild(m_princess, 20);

	// 客人
	ModelTemplate guestModel = ModelTemplateTem(WJUtils::stringAddInt("model", object.guestModel, 1).c_str());
	m_guest = GuestModel::create(object.guestModel, false);
	m_guest->setPosition(Vec2(size.width / 2 + 200, guestModel.npcPosY));
	m_guest->getSkeleton()->setOpacity(0);
	m_guest->playAnimation("aniIdle", true, 1);
	m_guest->setSoundIsBindAni(false);
	m_guest->getSkeleton()->setCompleteListener(CC_CALLBACK_2(NpcTalking::onSkeletonCompleted, this, m_guest->getSkeleton()));
	this->addChild(m_guest, 20);

	// 用shader进行高斯模糊
	shaderBlur(json);

	// cup计算进行高斯模糊
	//cupBlur(json);

	initSpeakFonts();

	return true;
}

void NpcTalking::onSkeletonCompleted(int trackIndex, int loopCount, WJSkeletonAnimation *skeleton)
{
	if (trackIndex == 1 && loopCount == 1)
	{
		std::string name = skeleton->getAnimationName(trackIndex);
		if (!WJUtils::equals("aniIdle", name.c_str()))
		{
			skeleton->playAnimation("aniIdle", true, 1);
		}
	}
}

void NpcTalking::cupBlur(WJLayerJson *json)
{
	/*const Size& size = this->getContentSize();

	m_texture = RenderTexture::create(1363, 768);
	m_texture->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_texture->ignoreAnchorPointForPosition(false);
	m_texture->getSprite()->getTexture()->setAntiAliasTexParameters();
	m_texture->retain();

	auto renderer = Director::getInstance()->getRenderer();
	auto& parentTransform = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Point savePoint = json->getPosition();

	m_texture->beginWithClear(0, 0, 0, 0);
	json->setPosition(Vec2(size.width / 2, size.height / 2));
	json->visit(renderer, parentTransform, true);
	m_texture->end();
	Director::getInstance()->getRenderer()->render();
	json->setPosition(savePoint);

	Image *image = m_texture->newImage();
	vector<float> vec1;
	vector<float> vec2;
	Gauss(vec1, 3);
	Image *imageFinal = Blur1D(*image, vec1, vec2, 3);

	m_texture2D = new Texture2D();
	m_texture2D->initWithImage(imageFinal, Texture2D::PixelFormat::RGBA8888);
	m_texture2D->autorelease();

	Sprite *sprite = Sprite::createWithTexture(m_texture2D);
	sprite->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(sprite, 10);

	CC_SAFE_DELETE(image);
	CC_SAFE_DELETE(imageFinal);
	CC_SAFE_RELEASE(m_texture);*/
}

void NpcTalking::shaderBlur(WJLayerJson *json)
{
    const Size& size = this->getContentSize();
    // create render
    m_texture = RenderTexture::create(1363, 768);
    m_texture->getSprite()->getTexture()->setAntiAliasTexParameters();
    m_texture->setPosition(Vec2(size.width / 2, size.height / 2));
    
    // visit json to render
    Point savePoint = json->getPosition();
    m_texture->beginWithClear(0, 0, 0, 0);
    json->setPosition(Vec2(size.width / 2, size.height / 2));
    json->Node::visit();
    m_texture->end();
    Director::getInstance()->getRenderer()->render();
    json->setPosition(savePoint);
    
    // set as blur shader
    m_texture->getSprite()->setGLProgram(WJSpriteShaderBlurQuick::getShader());
    GLProgramState* glState = m_texture->getSprite()->getGLProgramState();
    glState->setUniformVec2("resolution", Vec2(1.0f / 1363.0f, 1.0f / 768.0f));
    
    // visit blur image to render final
    m_finalTexture = RenderTexture::create(1363, 768);
    m_finalTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
    m_finalTexture->setPosition(Vec2(size.width / 2, size.height / 2));
    this->addChild(m_finalTexture, 10);
    
    m_finalTexture->beginWithClear(0, 0, 0, 0);
    m_texture->Node::visit();
    m_finalTexture->end();
    Director::getInstance()->getRenderer()->render();
    
    // set render blendfunc
    BlendFunc bf = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
    m_finalTexture->getSprite()->setBlendFunc(bf);
    
    
    m_sprite = m_finalTexture->getSprite();
}

void NpcTalking::initSpeakFonts()
{
	// 字体json
	const Size& size = this->getContentSize();
	m_fontsJson = WJLayerJson::create("game/json/dia.json");
	m_fontsJson->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_fontsJson->ignoreAnchorPointForPosition(false);
	m_fontsJson->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(m_fontsJson, 1000);

	// 比较长的语句
	Label *labelLong = m_fontsJson->getSubLabelByKey("talk_02");
	labelLong->setMaxLineWidth(1024);
	if (ConfigLanguageSetting::instance().getCurLanguage() != LanguageType::ENGLISH)
	{
		Label *newLong = Label::createWithSystemFont("", "Consolas", 40, labelLong->getDimensions(),
			labelLong->getHorizontalAlignment(), labelLong->getVerticalAlignment());
        newLong->setOpacity(labelLong->getOpacity());
		m_fontsJson->replaceSubNode("talk_02", newLong, true);
	}

	// 短的语句
	Label *labelShort = m_fontsJson->getSubLabelByKey("talk");
	if (ConfigLanguageSetting::instance().getCurLanguage() != LanguageType::ENGLISH)
	{
		Label *newShort = Label::createWithSystemFont("", "Consolas", 40, labelShort->getDimensions(),
			labelShort->getHorizontalAlignment(), labelLong->getVerticalAlignment());
         newShort->setOpacity(labelShort->getOpacity());
		m_fontsJson->replaceSubNode("talk", newShort, true);
	}

	// 从配置文件读取内容
	resetTalk(true);
}

void NpcTalking::changeTalk()
{
	AnimeTemplate object = AnimeTemplateTem(m_playKey.c_str());
	Label *label1 = m_fontsJson->getSubLabelByKey(object.princessFonts.c_str());
	Label *label2 =  m_fontsJson->getSubLabelByKey(object.guestFonts.c_str());

	Label *nowLabel = object.princessFirst ? label1 : label2;
	nowLabel->runAction(Sequence::create(
		FadeOut::create(0.3f),
		CCCallFunc::create(CC_CALLBACK_0(NpcTalking::resetTalk, this, false)),
		NULL));
}

void NpcTalking::resetTalk(bool first)
{
	AnimeTemplate object = AnimeTemplateTem(m_playKey.c_str());
	Label *label1 = m_fontsJson->getSubLabelByKey(object.princessFonts.c_str());
	Label *label2 =  m_fontsJson->getSubLabelByKey(object.guestFonts.c_str());
	if (first)
	{
	m_nowLabel = object.princessFirst ? label1 : label2;
	m_nowLabel->setString(object.princessFirst ? object.princessTalk.c_str() : object.guestTalk.c_str());
	}
	else
	{
		m_nowLabel = object.princessFirst ? label2 : label1;
		m_nowLabel->setString(object.princessFirst ? object.guestTalk.c_str() : object.princessTalk.c_str());
	}
}

void NpcTalking::getAnimalKeyFromGameNumber()
{
	//std::string str = "P004";
	//switch (m_gameNumber)
	//{
	//	case GameNumber::P004:
	//		str = "P004";
	//		break;
	//	case GameNumber::P006_Choose:
	//		str = "P006";
	//		break;
	//	case GameNumber::P008_Hair:
	//		str = "P008";
	//		break;
	//	case GameNumber::P009_HealthCare:
	//		str = "P009";
	//		break;
	//	case GameNumber::P010_Menu:
	//		str = "P010";
	//		break;
	//	default:
	//		break;
	//}

	//// 按照顺序依次播放每个游戏的开场动画
	//std::string saveStr = str;
	//int saveIndex = UserDefault::getInstance()->getIntegerForKey(saveStr.append("lastnpcIndex").c_str(), 0);
	//saveIndex++;
	//if (saveIndex > 3)
	//	saveIndex = 1;

	//UserDefault::getInstance()->setIntegerForKey(saveStr.c_str(), saveIndex);
	//UserDefault::getInstance()->flush();
	//str = str.append(":").append(WJUtils::stringAddInt("", saveIndex, 1).c_str());

	//m_playKey = str;
}

void NpcTalking::onEnter()
{
	WJLayer::onEnter();

	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(NpcTalking::gohomeNotification),
		"gohome", NULL);
}

void NpcTalking::gohomeNotification(Ref *object)
{
	stopPlaySoundAndAni();
}

void NpcTalking::onExit()
{
	WJLayer::onExit();

	NotificationCenter::getInstance()->removeObserver(this, "gohome");
}

void NpcTalking::onEnterTransitionDidFinish()
{
	WJLayer::onEnterTransitionDidFinish();

	// 延迟开始播放动画
	this->runAction(Sequence::create(
		DelayTime::create(0.5f),
		CCCallFunc::create(CC_CALLBACK_0(NpcTalking::fadeInNpcAndFonts, this)),
		DelayTime::create(1.3f),
		CCCallFunc::create(CC_CALLBACK_0(NpcTalking::playNpcTalkAndAni, this)),
		nullptr));
}

void NpcTalking::fadeInNpcAndFonts()
{
	float time = 0.3f;
	runActionFadeIn(m_princess->getSkeleton(), 0.0f, time);
	runActionFadeIn(m_guest->getSkeleton(), 0.0f, time);
	runActionFadeIn(m_fontsJson->getSubSpriteByKey("bg"), 0.0f, time);
	runActionFadeIn(m_skipButton, 0.0f, time);
}

NpcTalking::NpcTalking()
{
	m_isEnd = false;
	m_healthModelIndex = -1;
}

void NpcTalking::playNpcTalkAndAni()
{
	AnimeTemplate object = AnimeTemplateTem(m_playKey.c_str());
	// 公主是否先说话
	if (object.princessFirst)
	{
		// 播放公主的动画和配音
		playPrincessAniAndSound();
	}
	else
	{
		// 播放客人的动画和配音
		playGuestAniAndSound();
	}
}

void NpcTalking::onEndPlayNpcAni(float time)
{
	if (m_isEnd) return;

	m_isEnd = true;
	((WJLayer*)this->getParent())->setEnabled(true);
	
	m_princess->getSkeleton()->stopAllAnimation();
	m_princess->getSkeleton()->unscheduleUpdate();

	m_guest->getSkeleton()->stopAllAnimation();
	m_guest->getSkeleton()->unscheduleUpdate();

	//// 隐藏
	actionFadeOut(m_princess->getSkeleton(), 0.0f, 0.5f);
	actionFadeOut(m_guest->getSkeleton(), 0.0f, 0.5f);
	actionFadeOut(m_sprite, 0.0f, 0.5f);
	actionFadeOut(m_fontsJson->getSubSpriteByKey("bg"), 0.0f, 0.5f);
	actionFadeOut(m_nowLabel, 0.0f, 0.5f);
	actionFadeOut(m_skipButton, 0.0f, 0.5f);
    
    // release blur render
    m_finalTexture->runAction(Sequence::createWithTwoActions(
                                                        DelayTime::create(1.2f),
                                                        RemoveSelf::create()));
    m_sprite = nullptr;
    m_finalTexture = nullptr;

	this->runAction(Sequence::create(
		DelayTime::create(1.0f),
		CCCallFunc::create([&](){
			stopPlaySoundAndAni();
			doEventNpcTalkEndCallBack();
		}),
		DelayTime::create(0.5f),
		RemoveSelf::create(),
		NULL));
}

void NpcTalking::actionFadeOut(Node *node, float delay, float time)
{
	node->stopAllActions();
	node->runAction(Sequence::create(
		DelayTime::create(delay),
		FadeTo::create(time, 0),
		Hide::create(),
		NULL));
}

void NpcTalking::runActionFadeIn(Node *node, float delay, float time)
{
	node->setOpacity(0);
	node->runAction(Sequence::create(
		DelayTime::create(delay),
		FadeTo::create(time, 255),
		NULL));
}

void NpcTalking::onDestorySelf()
{
	this->removeFromParentAndCleanup(true);

}

NpcTalking::~NpcTalking()
{

}

void NpcTalking::setNpcTalkEndCallBack( const NpcTalkEndCallBack& callback )
{
	m_talkEndCallBack = callback;
}

void NpcTalking::doEventNpcTalkEndCallBack()
{
	if (m_talkEndCallBack)
		m_talkEndCallBack();
}

void NpcTalking::onPrincessCompleted()
{
	AnimeTemplate object = AnimeTemplateTem(m_playKey.c_str());
	//m_princess->getSkeleton()->stopAnimation(1);
	// 客人的动画播放完了
	if (object.princessFirst)
	{
		playGuestAniAndSound(NPC_TALK_WAIT_TIME);
		changeTalk();
	}
	else
	{
		endPlayAniAndSound();
	}
}

void NpcTalking::onGuestCompleted()
{
	AnimeTemplate object = AnimeTemplateTem(m_playKey.c_str());
	//m_guest->getSkeleton()->stopAnimation(1);
	if (object.princessFirst)
	{
		endPlayAniAndSound();
	}
	else
	{
		playPrincessAniAndSound(NPC_TALK_WAIT_TIME);
		changeTalk();
	}
}

void NpcTalking::playPrincessAniAndSound(float delay)
{
	m_guest->getSkeleton()->stopAnimation(2);
	this->runAction(Sequence::create(
		DelayTime::create(delay),
		CCCallFunc::create([this](){
			runActionFadeIn(m_nowLabel, 0.0f, 0.5f);
			AnimeTemplate object = AnimeTemplateTem(m_playKey.c_str());
			m_princess->playAnimation(object.princessAni.c_str(), false, 1);
			m_princess->playAnimation("aniTalk", true, 2);
			Common::sound.play(object.princessSoundId);
			this->runAction(Sequence::create(
				DelayTime::create(object.princessTime),
				CCCallFunc::create([this](){
					onPrincessCompleted();
				}),
				NULL));
		}),
		NULL))->setTag(PRINCESS_PLAY_ACTION_TAG);
}

void NpcTalking::playGuestAniAndSound(float delay)
{
	m_princess->getSkeleton()->stopAnimation(2);
	this->runAction(Sequence::create(
		DelayTime::create(delay),
		CCCallFunc::create([this](){
			runActionFadeIn(m_nowLabel, 0.0f, 0.5f);
			AnimeTemplate object = AnimeTemplateTem(m_playKey.c_str());
			m_guest->playAnimation(object.guestAni.c_str(), false, 1);
			m_guest->playAnimation("aniTalk", true, 2);
			Common::sound.play(object.guestSoundId);
			this->runAction(Sequence::create(
				DelayTime::create(object.guestTime),
				CCCallFunc::create([this](){
					onGuestCompleted();
				}),
				NULL));
		}),
		NULL))->setTag(GUEST_PLAY_ACTION_TAG);
}

void NpcTalking::endPlayAniAndSound()
{
	this->setEnabled(false);
	// 播放完了
	this->runAction(Sequence::create(
		DelayTime::create(0.5f),
		CCCallFunc::create([&](){
			onEndPlayNpcAni(0.1f);
		}),
		NULL));
}

void NpcTalking::onClickSkipButton(Node *node, WJTouchEvent *event)
{
	if (m_isEnd) return;

	Common::sound.play("Common:button");
	stopPlaySoundAndAni();
	this->setEnabled(false);
	onEndPlayNpcAni(0.1f);
}

void NpcTalking::stopPlaySoundAndAni()
{
	this->stopActionByTag(GUEST_PLAY_ACTION_TAG);
	this->stopActionByTag(PRINCESS_PLAY_ACTION_TAG);
	this->setEnabled(false);
	// 停止当前正在播放的声音
	AnimeTemplate object = AnimeTemplateTem(m_playKey.c_str());
	Common::sound.stop(object.guestSoundId);
	Common::sound.stop(object.princessSoundId);
}

void NpcTalking::setSkipButtonPostition( const Vec2 &pointWorld )
{
	m_skipButton->setPosition(m_skipButton->getParent()->convertToNodeSpace(pointWorld));
}

WJButton* NpcTalking::getSkipButton()
{
	return m_skipButton;
}

