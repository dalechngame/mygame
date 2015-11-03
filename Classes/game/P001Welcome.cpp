#include "P001Welcome.h"
#include "TestGameMenu.h"
#include "P011Draw.h"
#include "DailyBonus.h"

INIT_SCENE_LOADER_ASYNC(P001);

void P001::asyncLoadSceneResource()
{
	// 预加载场景Json中的 所有图片 及 骨骼
	WJLayerJsonPreload::getInstance()->asyncPreload("game/json/p001_welcome.json");

	// 同时再预加载1个额外的 骨骼(因此骨骼不在上面的Json中)
	WJSkeletonDataCache::getInstance()->addSkeletonDataAsync("game/skeleton/p001_welcome/princess.json", 
		"game/skeleton/p001_welcome/princess.atlas");
}

bool P001::init()
{
    m_render = nullptr;
	m_test3dRotate = false;
    
	if ( !PBase::init() )
	{
		return false;
	}

	// 初始化场景Json
	initSceneJson("game/json/p001_welcome.json");

	// 初始化工具栏
	initToolbar();

	// 测试 骨骼动画
	testSkeleton();

	// 测试 3d
	testSprite3D();

	// 测试画笔
	testCanvas();
    
    // 测试模糊效果
    //testShader();

	// 测试2d图片3d旋转效果
	test3dRotate();
    
	return true;
}

void P001::initSceneJson( const char* jsonFileName )
{
	PBase::initSceneJson(jsonFileName);

	// 播放标题Idle动画
	m_sceneJson->playAnimation("aniTitleIdle");

	// Play按钮Idle动画
	m_sceneJson->getSubButtonByKey("start")->setNormalAniType(WJButton::NormalAniType::Breathing);

	// 绑定事件
    m_sceneJson->getSubButtonByKey("start")->setInvisibleTouchAble(true);
	m_sceneJson->getSubButtonByKey("start")->setOnClick([=](Node* node, WJTouchEvent* event)
	{
		Common::sound.play("Common:button");

		// show chartboost
		Common::showChartBoostOnPlayButton();

		// goto p002
		Loading::gotoScene(GameNumber::P014_Main);
	});

	// 下雪粒子效果
	//ParticleSystemQuad* snow = CCParticleSystemQuad::create("particles/snow1.plist");
	//snow->setPosition(Vec2(m_winSize.width /2 , m_winSize.height));
	//this->addChild(snow, 100);
}

void P001::initToolbar()
{
	m_toolbar = LBToolbar::create(CC_CALLBACK_2(P001::onToolbarClick, this));
	
	// forparents button
	m_toolbar->addButton("button/forParents.png", m_origin.x + 20, m_visibleRect.getMaxY() - 20, MENU_TAG_FORPARENTS)->setAnchorPoint(Vec2(0, 1));
	
	// info button
	m_toolbar->addButton("button/uiButton014.png", m_origin.x + 20, m_origin.y + 20, MENU_TAG_INFO)->setAnchorPoint(Vec2(0, 0));

	// store button
	if (!Store::isUnlockedAll() )
	{
		m_toolbar->addButton("button/uiButton013.png",m_origin.x + 95, 20 + m_origin.y, MENU_TAG_STORE)->setAnchorPoint(Vec2(0, 0));
	}

	//   music button
	if (WJUtils::isSmallScreen())
		m_toolbar->addButton("base/none.png", m_visibleRect.getMaxX() - 160, 20 + m_origin.y, MENU_TAG_MUSIC)->setAnchorPoint(Vec2(1, 0));
	else
		m_toolbar->addButton("base/none.png", m_visibleRect.getMaxX() - 115, 25 + m_origin.y, MENU_TAG_MUSIC)->setAnchorPoint(Vec2(1, 0));
	m_toolbar->setMusicButtonFile("button/uiButton015.png", "button/uiButton016.png");

	this->addChild(m_toolbar, 10);
}

void P001::onToolbarClick( Node* node, WJTouchEvent* evnet )
{
	int tag = node->getTag();
	switch (tag)
	{
	case MENU_TAG_FORPARENTS:
		Common::sound.play("Common:button");

		if (DEBUG_APP)
			TestGameMenu::show();
		else
			WJUtils::showForParentDialog(THIS_APP_ID, URL_FOR_PARENTS);

		break;

	case MENU_TAG_INFO:
		showInfoDialog();
		break;

	case MENU_TAG_STORE:
		Store::showStore();
		break;
	}
}

void P001::testSkeleton()
{
	WJSkeletonAnimation *sp = WJSkeletonAnimation::createWithFile("game/skeleton/p001_welcome/princess.json", 
																  "game/skeleton/p001_welcome/princess.atlas");
	sp->setPosition(Vec2(400, 0));
	sp->setSkin("hair01");
	sp->setRotation(-10);   // 测试 骨骼或其父节点进行旋转
	sp->setScale(0.75f);    // 测试 骨骼或其父节点进行缩放
	sp->setMoveAble(true);
	sp->setOnClick([](Node *node, WJTouchEvent* event)
	{
		node->setRotation(node->getRotation() + 10);
	});
	m_sceneJson->addChild(sp);

	// 演示 换装：皇冠
	sp->setSlotTexture("crown", "game/p001_welcome/crown/001.png");

	// 演示 动画：播 aniHair 2遍，再播 aniSalute 1遍，然后再如此循环
	sp->playAnimation("aniHair", true, 0);
	sp->addAnimation("aniSalute", false, 0, sp->getAnimation("aniHair")->duration * 2.0f);
	sp->setCompleteListener([=](int trackIndex, int loopCount)
	{
		if (trackIndex == 0 && WJUtils::equals(sp->getAnimationName(trackIndex), "aniSalute"))
		{
			// 继续循环
			sp->playAnimation("aniHair", true, 0);
			sp->addAnimation("aniSalute", false, 0, sp->getAnimation("aniHair")->duration * 2.0f);
		}
	});

	// 演示：绑定Node
	Layer *bindLayer = Layer::create();
	bindLayer->setRotation(30);			// 任意旋转
	bindLayer->setPosition(300, 300);   // 任意坐标 将不影响bind node的位置
	this->addChild(bindLayer, 5);

	WJSprite *lock = WJSprite::create("lock.png");
	bindLayer->addChild(lock, 1000);   // lock 同 sp 并不在同一层级下
	
	// 绑定 图片的 位置、旋转、缩放
	sp->bindNodeToBone(lock, "glove04", Vec2::ZERO, true, -80, true, 2.0f);

	//ParticleSystemQuad *particle = ParticleSystemQuad::create("particles/ballfire.plist");
	//bindLayer->addChild(particle);

	//// 绑定 粒子效果
	//sp->bindNodeToBone(particle, "glove03");
}

void P001::testSprite3D()
{
	// 3d 模型
	WJSprite3D *s3d = WJSprite3D::create("game/3d/tortoise.c3b");
	s3d->setScale(0.15f);
	s3d->setPosition(789, 402);
	s3d->playAnimation(0, 1.933f);   // 3d动画
	s3d->setMoveAble(true);			 // 可拖动
	this->addChild(s3d, 100);
	
	s3d->runAction(RepeatForever::create(RotateBy::create(5.0f, Vec3(0, 180.f, 0))));  // 3d旋转
}


void P001::onEnter()
{
	PBase::onEnter();

	if (m_test3dRotate)
		Director::getInstance()->setProjection(Director::Projection::_3D);
}

void P001::onEnterTransitionDidFinish()
{
    PBase::onEnterTransitionDidFinish();

	// show moregame button
    WJUtils::showMoreGameButton(-1, -1, 74, 74, THIS_APP_ID, URL_MOREGAME_DEF, wjalNone, MOREGAME_TEXT_COLOR_WHITE);

	DailyBonus::showItIfNeed(this, 1000);
}

void P001::onExit()
{
	PBase::onExit();

	if (m_test3dRotate)
		Director::getInstance()->setProjection(Director::Projection::_2D);

	WJUtils::hideMoreGameButton();
}

GameNumber P001::getGameNumber()
{
	return GameNumber::P001;
}

void P001::onStoreClosedAndPurchased()
{
	PBase::onStoreClosedAndPurchased();

	Node* node = m_toolbar->getButtonByTag(MENU_TAG_STORE);
	if (node)
	{
		node->setVisible(!Store::isUnlockedAll());
	}
}

void P001::testCanvas()
{
	WJCanvas* canvas = WJCanvas::create(400, 300);
	canvas->ignoreAnchorPointForPosition(false);
	canvas->setAnchorPoint(Vec2(0.5f, 1.0f));
	canvas->setPosition(VISIBLE_RECT_TOP);
	canvas->setTouchDrawEnabled(true);
	canvas->setOnTouchDrawBegan([](WJCanvas* canvas, Touch* touch)
	{
		canvas->getPen()->setPenColor(Color3B(WJUtils::randomInt(255), WJUtils::randomInt(255), WJUtils::randomInt(255)));
	});

	this->addChild(canvas, 100, 100);

	WJPen* pen = WJPen::create("pen/solid_32.png");
	pen->setPenColor(Color3B::YELLOW);
	pen->setPenSize(0.5f);
	canvas->setPen(pen);
}

// 全屏实时模糊，上真机效率很差，iPad mini1只能15帧左右。本例仅为演示用法
void P001::testShader()
{
    // --- Blur
    m_render = RenderTexture::create(m_visibleSize.width, m_visibleSize.height);
    m_render->setPosition(SCREEN_CENTER);
    m_render->clear(0, 0, 0, 0);
    
    m_render->getSprite()->setGLProgram(WJSpriteShaderBlurQuick::getShader());
    GLProgramState* glState = m_render->getSprite()->getGLProgramState();
    
    Size size = m_render->getSprite()->getTexture()->getContentSizeInPixels();
    size.width = 1.0f / size.width;
    size.height = 1.0f / size.height;
    glState->setUniformVec2("resolution", size);
    
    m_render->getSprite()->getTexture()->setAntiAliasTexParameters();
    
    this->addChild(m_render, m_sceneJson->getLocalZOrder());
    
    BlendFunc bf = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
    m_render->getSprite()->setBlendFunc(bf);
    m_render->getSprite()->runAction(RepeatForever::create(
                                        Sequence::create(
                                            DelayTime::create(2.0f),
                                            FadeOut::create(1.0f),
                                            DelayTime::create(1.0f),
                                            FadeIn::create(1.0f),
                                            NULL)));
    
    //m_sceneJson->setVisible(false);
    
    // --- Gray
    WJButton* btnStart = m_sceneJson->getSubButtonByKey("start");
    //btnStart->setGLProgram(WJSpriteShaderGray::getShader());  // 因为现在WJButton支持九宫格功能，对它设置Shader会无效
    btnStart->runAction(RepeatForever::create(
                                              Sequence::create(
                                                   DelayTime::create(2.0f),
                                                   CallFunc::create([=]()
                                                   {
													   btnStart->setGrayEnabled(true);
                                                   }),
                                                   DelayTime::create(2.0f),
                                                   CallFunc::create([=]()
                                                   {
													   btnStart->setGrayEnabled(false);
                                                   }),
                                                   NULL)));
}

void P001::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
    if (m_render)
    {
        m_render->beginWithClear(0, 0, 0, 0);
        //m_sceneJson->setVisible(true);
        m_sceneJson->saveCurrentPosition();
        m_sceneJson->setPosition(CENTER_SIZE(m_visibleSize));
        m_sceneJson->visit(renderer, parentTransform, parentFlags);
        //m_sceneJson->setVisible(false);
        m_render->end();

        renderer->render();
        m_sceneJson->restoreSavedPosition();
    }

    PBase::visit(renderer, parentTransform, parentFlags);
}

void P001::test3dRotate()
{
	m_test3dRotate = true;

	WJSprite* s = WJSprite::create("store/popup.png");
	s->setScale(0.3f);
	s->setMoveAble(true);
	s->setPosition(1086, 325);
	this->addChild(s);

	s->runAction(RepeatForever::create(
					CCRotateBy::create(5.0f, Vec3(360, 0, 360))));
}


