#include "P007MakeUp.h"
#include "LineLayer.h"

#define CANVAS_ZORDER 100
#define SCROLL_ZORDER 1000

#define TOOL_NUM	5
#define PNG_PATH	"game/03_minigame/p007/"
#define DEFAULT_PATH	{ "eyebrow/l/clothes000.png", "eyebrow/r/clothes000.png", "eyeshadow/clothes000.png", "cheek/clothes000.png", "mouth/clothes000.png" }
#define CLOTHES_PATH	{ "eyebrow/l/meimaol%03d.png", "eyebrow/r/meimaor%03d.png", "eyeshadow/yanying%03d.png", "cheek/saihong%03d.png", "mouth/kouhong%03d.png" }
#define BONE_NAME	{ "meimaol", "meimaor", "yanying", "saihong", "kouhong" }
#define SLOT_NAME	{ "meimaoL", "meimaoR", "yanying", "saihong", "kouhong" }

INIT_SCENE_LOADER_SYNC(P007_MakeUp);

P007_MakeUp::P007_MakeUp()
{
}

P007_MakeUp::~P007_MakeUp()
{
}

GameNumber P007_MakeUp::getGameNumber()
{
	return GameNumber::P009;
}

void P007_MakeUp::onEnterTransitionDidFinish()
{
	PBase::onEnterTransitionDidFinish();
	playAnimation(1);
}

void P007_MakeUp::onClickNextButton(Node *node, WJTouchEvent *event)
{
	if (m_step == step3_camera)
	{
		stopMoveFingerTips();
		unschedule(schedule_selector(P007_MakeUp::updateTips3));
		float dt = playAnimation(6);
		WJUtils::playParticle("particles/minigame_complete.plist", this, ZORDER_PROGRESS + 1);
		this->runAction(Sequence::create(
			DelayTime::create(dt),
			CallFunc::create([&]()
		{
			showStarDialog();
		}),
			nullptr));
	}
	else
	{
		stopMoveFingerTips();
		unschedule(schedule_selector(P007_MakeUp::updateTips2));
		float dt = playAnimation(6);
		WJUtils::playParticle("particles/yellowstar.plist", this, ZORDER_PROGRESS + 1);
		this->setEnabled(false);
		this->runAction(Sequence::create(
			DelayTime::create(dt),
			CallFunc::create([&]()
		{
			Loading::showLoading();
		}),
			DelayTime::create(0.6f),
			CallFunc::create([&]()
		{
			m_step = step3_camera;
			hideNextButton();
			endStep2();
			initSceneJson("game/json/P007_camera.json");
			initStep3();
			this->setEnabled(true);
		}),
			nullptr));
	}
}

void P007_MakeUp::initProgressBar(const Vec2 &pointWorld , float scale, const char * jsonFile)
{
	if (!m_progress)
	{
		m_progress = ProgressBar::create(jsonFile);
		m_progress->setPosition(m_sceneJson->convertToNodeSpace(pointWorld == Vec2::ZERO ?
			Vec2(40, m_winSize.height / 2) : pointWorld));
		m_progress->setScale(m_progress->getScale() * scale);
		this->addChild(m_progress, ZORDER_PROGRESS);
	}
}

void P007_MakeUp::onSnapshotClosed()
{
	PBase::onSnapshotClosed();
	if (m_scrollLayer && WJUtils::equals(m_scrollLayer->getUserString(), "canShowNext"))
	{
		showNextButton();
	}
}

bool P007_MakeUp::init()
{
	if (!PBase::init())
	{
		return false;
	}
	m_step = step1_baes;
	m_step1Base = nullptr;
	m_currentTool = nullptr;
	m_scrollLayer = nullptr;
	m_changeBGNum = 0;
	
	initSceneJson("game/json/P007.json");
	initStep1();
	return true;
}

void P007_MakeUp::initSceneJson(const char* jsonFileName)
{
	PBase::initSceneJson(jsonFileName);
#define background_PLIST "particles/background.plist"
	if (m_step == step1_baes)
	{
		WJSkeletonAnimation* sk = m_sceneJson->getSubSkeleton("LBSpine_big");
		sk->setVisible(false);
		m_skeleton = WJSkeletonAnimation::createWithFile("game/skeleton/p007/libiiBig/libiiBig");
		m_skeleton->setPosition(sk->getPosition());
		m_skeleton->update(0);
		m_skeleton->noClickMoveEffect();
		this->addChild(m_skeleton, ZORDER_SCENE_JSON + 2);
		m_skeleton->setOnClick([&](Node* node, WJTouchEvent* event)
		{
			playAnimation(2);
		});
		WJUtils::playParticle(background_PLIST, this, ZORDER_SCENE_JSON + 1);
	}
	else
	{
		WJSkeletonAnimation* sk = m_sceneJson->getSubSkeleton("XGZ");
		sk->setVisible(false);
		m_skeleton->setPosition(sk->getPosition());
		m_skeleton->setLocalZOrder(ZORDER_SCENE_JSON - 1);

		removeChildByName(background_PLIST);
		playAnimation(1);
	}
}

void P007_MakeUp::initStep1()
{
	m_freckle = WJSprite::create("game/03_minigame/p007/queban.png");
	m_freckle->retain();
	Size spsize = m_freckle->getContentSize();

	m_renderTexture_freckle = RenderTexture::create(spsize.width, spsize.height);
	m_renderTexture_freckle->setContentSize(spsize);
	m_renderTexture_freckle->retain();

	m_renderTexture_freckle->beginWithClear(0, 0, 0, 0);
	m_freckle->setFlippedY(true);
	m_freckle->setPosition(CENTER_SIZE(spsize));
	m_freckle->Node::visit();
	m_renderTexture_freckle->end();
	DIRECTOR_RENDER;

	Image* img = m_renderTexture_freckle->newImage();
	m_freckle->setUserTag(PUtils::getImgBlankspace(img));
	CC_SAFE_DELETE(img);

	m_canvas_freckle = WJCanvas::create(spsize.width, spsize.height);
	m_canvas_freckle->ignoreAnchorPointForPosition(false);
	m_canvas_freckle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_canvas_freckle->setTouchDrawFlipY(true);
	m_sceneJson->addChild(m_canvas_freckle, CANVAS_ZORDER);
	WJPen* pen = WJPen::create("pen/pen_sL.png");
	pen->setPenOpacity(40);
	pen->setDrawStep(10);
	m_canvas_freckle->setPen(pen);
	m_canvas_freckle->setVisible(false);
	m_skeleton->bindNodeToBone(m_canvas_freckle, "queban");

	m_endThisTouch = false;
	m_step1Base = m_sceneJson->getSubSprite("step1_base");
	m_step1Base->saveCurrentProperties();
	m_step1Base->noClickMoveEffect();
	m_step1Base->runAction(Sequence::create(
		DelayTime::create(2.0f),
		CallFunc::create([&]()
	{
		float t = PUtils::playItemBounceEffect(m_step1Base,2);
		this->schedule(schedule_selector(P007_MakeUp::updateTips1), 4.f);
	}),
		nullptr));
#define CLEAN_PARTICLE "particles/004clean.plist"
	m_step1Base->setOnTouchAble([&](Node* node, WJTouchEvent* event)->bool
	{
		m_step1Base->setGlobalZOrder(2);
		stopMoveFingerTips();
		m_step1Base->stopAllActions();
		m_canvas_freckle->doTouchBegan(event->touch);
		FiniteTimeAction* act = MoveTo::create(0.05f, node->getParent()->convertTouchToNodeSpace(event->touch));
		node->runAction(act);
		schedule(schedule_selector(P007_MakeUp::update_freckle), 0.1f);
		unschedule(schedule_selector(P007_MakeUp::updateTips1));
		playAnimation(4);
		return true;
	});
	m_step1Base->setOnWillMoveTo([&](Node* node, WJTouchEvent* event)->bool
	{
		if (m_endThisTouch)
		{
			m_step1Base->endThisTouch(event->touch);
			return false;
		}
		WJSprite* sp = static_cast<WJSprite*>(node);
		m_canvas_freckle->doTouchMoved(event->touch);
		if (m_canvas_freckle->getContentRectInWorld().intersectsRect(sp->getContentRectInWorld()))
		{
			if (!sp->getChildByName(CLEAN_PARTICLE))
			{
				ParticleSystem* par = WJUtils::playParticle(CLEAN_PARTICLE, sp, -1, true);
				par->setGlobalZOrder(1);
			}
			sp->getChildByName(CLEAN_PARTICLE)->setVisible(true);
		}
		else
		{
			if (sp->getChildByName(CLEAN_PARTICLE))
				sp->getChildByName(CLEAN_PARTICLE)->setVisible(false);
		}
		return true;
	});
	m_step1Base->setOnTouchEnded([&](Node* node, WJTouchEvent* event)
	{
		schedule(schedule_selector(P007_MakeUp::updateTips1), 4.f);
		unschedule(schedule_selector(P007_MakeUp::update_freckle));
		m_canvas_freckle->doTouchEnded(event->touch);
		m_step1Base->setEnabled(false);
		FiniteTimeAction* act = Sequence::createWithTwoActions(
			MoveTo::create(0.5f, m_step1Base->getSavedPosition()),
			WJActionSetEnabled::create(true, false));
		if (m_endThisTouch)
		{
			unschedule(schedule_selector(P007_MakeUp::updateTips1));
			act = Sequence::createWithTwoActions(
				MoveTo::create(0.5f, m_step1Base->getSavedPosition()),
				CallFunc::create([&]{ endStep1(); }));
		}
		m_step1Base->runAction(act);
		if (node->getChildByName(CLEAN_PARTICLE))
			node->getChildByName(CLEAN_PARTICLE)->setVisible(false);

		playAnimation(1);
	});

}

void P007_MakeUp::update_freckle(float delay)
{
	m_renderTexture_freckle->beginWithClear(0, 0, 0, 0);
	m_canvas_freckle->getCanvasSprite()->setPosition(CENTER_SIZE(m_renderTexture_freckle->getContentSize()));
	m_canvas_freckle->getCanvasSprite()->Node::visit();
	m_freckle->setBlendFunc(BlendFunc({ GL_ONE_MINUS_DST_ALPHA, GL_ZERO }));
	m_freckle->Node::visit();
	m_renderTexture_freckle->end();
	DIRECTOR_RENDER;
	m_skeleton->setSlotTexture("queban", m_renderTexture_freckle->getSprite()->getTexture());

	if (isFreckleClear())
	{
		m_endThisTouch = true;
		unschedule(schedule_selector(P007_MakeUp::update_freckle));
	}
}

bool P007_MakeUp::isFreckleClear()
{
	float fold = m_freckle->getUserTag();
	Image* img = m_renderTexture_freckle->newImage();
	float fnew = PUtils::getImgBlankspace(img);
	CC_SAFE_DELETE(img);

	float fend = (fnew - fold) / (1 - fold);
	if (fend >= 0.85f)
	{
		refreshStar(1);
		return true;
	}

	return false;
}

void P007_MakeUp::endStep1()
{
	if (m_step != step1_baes)
		return;

	m_step = step_none;
	m_step1Base->setEnabled(false);
	m_step1Base->runAction(FadeOut::create(0.5f));
	m_step1Base = nullptr;
	m_sceneJson->getSubSprite("foundation_1")->runAction(FadeOut::create(0.5f));
	m_skeleton->clearSlotTexture("queban");
	m_skeleton->removeBindNode(m_canvas_freckle);
	m_canvas_freckle->removeFromParentAndCleanup(true);
	CC_SAFE_DELETE(m_freckle);
	CC_SAFE_DELETE(m_renderTexture_freckle);
	REMOVE_UNUSE_TEXTURE;
	
	initStep2();
}

void P007_MakeUp::initStep2()
{
	initCanvas();
	initScrollLayer();
	initMakeUpRenderTexture();
	initPasterLayer();

	for (int i = 0; i < 5; i++)
		m_isPlayed[i] = false;
}

void P007_MakeUp::initCanvas()
{
	Size canvaSize = Size(500, 600);
	m_step2Canvas = WJCanvas::create(canvaSize.width, canvaSize.height);
	m_step2Canvas->ignoreAnchorPointForPosition(false);
	m_step2Canvas->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	m_step2Canvas->setTouchDrawFlipY(true);
	m_step2Canvas->setPosition(m_skeleton->getPosition());
	m_step2Canvas->saveCurrentPosition();
	m_sceneJson->addChild(m_step2Canvas, CANVAS_ZORDER);

	m_currentTool = nullptr;
	m_itemIndex = -1;
	m_step = makeUpStep::step2_eyebrow;
}

static bool P007ShowScrollIsFirstRun[7] = {
	true, true, true, true, true, true, true
};
void P007_MakeUp::initScrollLayer()
{
	m_scrollDoubleLayer = ScrollDoubleLayer::create("P007_left_", ScrollDragDir::StopDrag, 5, true, P007ShowScrollIsFirstRun);
	m_scrollDoubleLayer->setStackMaxNum(100);
	SCROLL_POSITION_IN_JSON(m_scrollDoubleLayer);
	this->addChild(m_scrollDoubleLayer, 100);

	m_scrollDoubleLayer->setClickMenuScrollItemEvent(CC_CALLBACK_3(P007_MakeUp::onRightScrollItemClick, this));
	m_scrollDoubleLayer->setClickScrollItemEvent(CC_CALLBACK_3(P007_MakeUp::onScrollItemClick, this));
	m_scrollDoubleLayer->setScrollItemTouchAbleEvent(CC_CALLBACK_3(P007_MakeUp::onScrollItemTouchBegin, this));
	m_scrollDoubleLayer->setScrollItemMovingCollisionEvent(CC_CALLBACK_3(P007_MakeUp::onScrollItemTouchMove, this));
	m_scrollDoubleLayer->setScrollItemTouchEndedEvent(CC_CALLBACK_3(P007_MakeUp::onScrollItemTouchEnd, this));
	m_scrollDoubleLayer->setClickCancelItemEvent(CC_CALLBACK_3(P007_MakeUp::onClickCancelItem, this));
	m_scrollDoubleLayer->show(1.0f);
}

void P007_MakeUp::initMakeUpRenderTexture()
{
	const char* slotName[TOOL_NUM] = SLOT_NAME;
	const char* boneName[TOOL_NUM] = BONE_NAME;
	const char* defaultName[TOOL_NUM] = DEFAULT_PATH;
	for (int i = 0; i < TOOL_NUM; i++)
	{
		string path = PNG_PATH;
		WJSprite* sp = WJSprite::create(path.append(defaultName[i]).c_str());
		m_step2RenderTexture[i] = RenderTexture::create(sp->getContentSize().width, sp->getContentSize().height);
		m_step2RenderTexture[i]->setContentSize(sp->getContentSize());
		m_step2RenderTexture[i]->beginWithClear(0, 0, 0, 0);
		sp->setPosition(CENTER_SIZE(sp->getContentSize()));
		sp->setFlippedY(true);
		sp->Node::visit();
		m_step2RenderTexture[i]->end();
		m_step2RenderTexture[i]->setPosition(m_skeleton->getBoneWorldPosition(boneName[i]));
		m_step2RenderTexture[i]->setVisible(false);

		m_sceneJson->addChild(m_step2RenderTexture[i], CANVAS_ZORDER);
		m_skeleton->setSlotTexture(slotName[i], m_step2RenderTexture[i]->getSprite()->getTexture());
	}
}

void P007_MakeUp::initPasterLayer()
{
	Size size_face = Size(420,460);
	m_pasterLayer = WJLayer::create();
	m_pasterLayer->setContentSize(size_face);
	m_skeleton->addChild(m_pasterLayer);
	m_skeleton->bindNodeToBone(m_pasterLayer, "face", -CENTER_SIZE(size_face) - Vec2(-15,30));

	//LayerColor* layerTatoo = LayerColor::create(Color4B(255, 0, 0, 50), size_face.width, size_face.height);
	//m_skeleton->addChild(layerTatoo, CANVAS_ZORDER);
	//m_skeleton->bindNodeToBone(layerTatoo, "face", -CENTER_SIZE(size_face) - Vec2(-15, 30));
}

void P007_MakeUp::onClickCancelItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ereasePaster();
}

void P007_MakeUp::onRightScrollItemClick(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem* sItem = static_cast<ScrollItem*>(node);
	m_step = (P007_MakeUp::makeUpStep)(sItem->getItemIndex() + 2);
	if (m_currentTool)
	{
		stopMoveFingerTips();
		unschedule(schedule_selector(P007_MakeUp::updateTips2));
		m_currentTool->removeFromParentAndCleanup(true);
		m_currentTool = nullptr;
	}
}

void P007_MakeUp::onScrollItemClick(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	if (m_currentTool)
	{
		stopMoveFingerTips();
		unschedule(schedule_selector(P007_MakeUp::updateTips2));
		m_currentTool->removeFromParentAndCleanup(true);
		m_currentTool = nullptr;
	}

	if (m_step == makeUpStep::step2_paster)
	{
		addPaster(node);
		return;
	}
	ScrollItem* sItem = static_cast<ScrollItem*>(node);
	string name = sItem->getItemFilePath();
	m_currentTool = WJSprite::create(name.c_str());
	m_currentTool->setPosition(Vec2(m_visibleRect.getMaxX() - 300, 200)); 
	m_currentTool->saveCurrentPosition();
	m_currentTool->noClickMoveEffect();
	this->addChild(m_currentTool, ZORDER_SCENE_JSON + 10);
	float t = PUtils::playItemBounceEffect(m_currentTool);
	m_currentTool->runAction(Sequence::createWithTwoActions(DelayTime::create(t), CallFunc::create([&]{ PUtils::playNodeScaleAniForever(m_currentTool,1.0f); })));
	schedule(schedule_selector(P007_MakeUp::updateTips2), 4.f);
	m_currentTool->setOnTouchAble([&](Node* node, WJTouchEvent* event)->bool
	{
		stopMoveFingerTips();
		unschedule(schedule_selector(P007_MakeUp::updateTips2));
		m_currentTool->stopAllActions();
		m_currentTool->runAction(MoveTo::create(0.1f, m_currentTool->getParent()->convertTouchToNodeSpace(event->touch) + Vec2(-30, 0)));

		m_step2Canvas->clear(0, 0, 0, 0);
		unschedule(schedule_selector(P007_MakeUp::updateSlotTexetureFromRender));
		schedule(schedule_selector(P007_MakeUp::updateSlotTexetureFromRender), 0.1f);
		m_step2Canvas->doTouchBegan(event->touch);

		playAnimation(3);

		return true;
	});
	m_currentTool->setOnWillMoveTo([&](Node* node, WJTouchEvent* event)->bool
	{
		if (m_step2Canvas->getContentRectInWorld().containsPoint(event->touch->getLocation()))
		{
			m_isPlayed[m_step - makeUpStep::step1_baes - 1] = true;
			refreshStar(2);
		}
		m_step2Canvas->doTouchMoved(event->touch);
		return true;
	});
	m_currentTool->setOnTouchEnded([&](Node* node, WJTouchEvent* event)
	{
		schedule(schedule_selector(P007_MakeUp::updateTips2),4.f);
		unschedule(schedule_selector(P007_MakeUp::updateSlotTexetureFromRender));
		m_step2Canvas->doTouchEnded(event->touch);

		m_currentTool->setEnabled(false);
		m_currentTool->runAction(Sequence::create(
			MoveTo::create(0.5f, m_currentTool->getSavedPosition()),
			WJActionSetEnabled::create(true, true),
			CallFunc::create([&](){ PUtils::playNodeScaleAniForever(m_currentTool, 1.0f); }),
			nullptr));
	});
}

bool P007_MakeUp::onScrollItemTouchBegin(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem* sItem = static_cast<ScrollItem*>(node);
	m_itemIndex = sItem->getItemIndex() + 1;

	if (m_step == makeUpStep::step2_paster)
	{
		return true;
	}

	m_step2Canvas->clear(0, 0, 0, 0);
	changePen(WJSprite::create(sItem->getItemFilePath().c_str()));
	unschedule(schedule_selector(P007_MakeUp::updateSlotTexetureFromRender));
	schedule(schedule_selector(P007_MakeUp::updateSlotTexetureFromRender), 0.1f);
	m_step2Canvas->doTouchBegan(touchEvent->touch);
	return true;
}

bool P007_MakeUp::onScrollItemTouchMove(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	if (m_step == makeUpStep::step2_paster)
	{
		return false;
	}
	if (m_currentTool)
	{
		m_currentTool->removeFromParentAndCleanup(true);
		m_currentTool = nullptr;
	}
	ScrollItem* sItem = static_cast<ScrollItem*>(node);
	if (sItem->getUserTag() == -1)
	{
		sItem->setUserTag(1);
		playAnimation(3);
	}
	if (m_step2Canvas->getContentRectInWorld().containsPoint(touchEvent->touch->getLocation()))
	{
		m_isPlayed[m_step - makeUpStep::step1_baes - 1] = true;
		refreshStar(2);
		if (sItem->getUserTag() != 0)
		{
			sItem->setUserTag(0);
			playAnimation(5);
		}
	}
	m_step2Canvas->doTouchMoved(touchEvent->touch);
	return false;
}

void P007_MakeUp::onScrollItemTouchEnd(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	if (m_step == makeUpStep::step2_paster)
	{
		ScrollItem* sItem = static_cast<ScrollItem*>(node);
		WJSprite* sp = sItem->getMovingSprite();
		Vec2 pos = m_skeleton->convertTouchToNodeSpace(touchEvent->touch);
		pos.x -= 30;
		if (isInFaceBox(pos))
		{
			addPaster(sItem, false, m_pasterLayer->convertTouchToNodeSpace(touchEvent->touch) + Vec2(-30, 0));
			static_cast<WJSprite *>(sp->getUserObject())->removeFromParentAndCleanup(true);
			sp->removeFromParentAndCleanup(true);
		}
		return;
	}
	ScrollItem* sItem = static_cast<ScrollItem*>(node);
	if (sItem->getUserTag() != -1)
	{
		sItem->setUserTag(-1);
		playAnimation(1);
	}
	unschedule(schedule_selector(P007_MakeUp::updateSlotTexetureFromRender));
	m_step2Canvas->doTouchEnded(touchEvent->touch);
}

void P007_MakeUp::changePen(WJSprite* pen_png)
{
	WJPen* pen = m_step2Canvas->getPen();
	if (!pen)
	{
		pen = WJPen::create("pen/solid_brush.png");
		m_step2Canvas->setPen(pen);
	}
	m_step2Canvas->setTouchDrawOffset(Vec2(0, pen_png->getContentSize().height / 2) - Vec2(0, pen->getPenTextureSize() / 2) + Vec2(-30, 0));
}

void P007_MakeUp::updateSlotTexetureFromRender(float delay)
{
	const char* clothes[TOOL_NUM] = CLOTHES_PATH;
	const char* slotName[TOOL_NUM] = SLOT_NAME;
	char buf[64];
	string path = PNG_PATH;
	switch (m_step)
	{
		case P007_MakeUp::step2_eyebrow:
			for (int i = 0; i < 2; i++)
			{
				path = PNG_PATH;
				sprintf(buf, clothes[i], m_itemIndex);
				path.append(buf);
				drawAndUpdateSlotTexture(m_step2RenderTexture[i], path, slotName[i]);
			}
			break;
		case P007_MakeUp::step2_eyeshadow:
		case P007_MakeUp::step2_cheek:
		case P007_MakeUp::step2_mouth:
			sprintf(buf, clothes[m_step - 1], m_itemIndex);
			path.append(buf);
			drawAndUpdateSlotTexture(m_step2RenderTexture[m_step - 1], path, slotName[m_step - 1]);
		case P007_MakeUp::step2_paster:
			break;
		default:
			break;
	}
}

void P007_MakeUp::drawAndUpdateSlotTexture(RenderTexture* rt, string pngPath, string slotName)
{
	RenderTexture* tmpRT = RenderTexture::create(rt->getContentSize().width, rt->getContentSize().height);
	tmpRT->setContentSize(rt->getContentSize());
	tmpRT->setPosition(rt->getPosition());

	Size renderSize = tmpRT->getContentSize();
	Vec2 renderPos = tmpRT->getPosition();
	Vec2 canvasPos = m_step2Canvas->getSavedPosition();

	float xcz = canvasPos.x - (renderPos.x - renderSize.width / 2);
	float ycz = canvasPos.y - (renderPos.y + renderSize.height / 2);
	Vec2 pos = Vec2(xcz, -(m_step2Canvas->getContentSize().height - abs(ycz)));

	tmpRT->beginWithClear(0, 0, 0, 0);
	m_step2Canvas->setPosition(pos);
	m_step2Canvas->setVisible(true);
	m_step2Canvas->Node::visit();
	WJSprite* slotSprite = WJSprite::create(pngPath.c_str());
	slotSprite->setFlippedY(true);
	slotSprite->setBlendFunc({ GL_DST_ALPHA, GL_ZERO });
	slotSprite->setPosition(CENTER_SIZE(slotSprite->getContentSize()));
	slotSprite->Node::visit();
	tmpRT->end();
	DIRECTOR_RENDER;

	rt->setVisible(true);
	tmpRT->setPosition(CENTER_SIZE(tmpRT->getContentSize()));
	BlendFunc blend = rt->getSprite()->getBlendFunc();
	rt->begin();
	m_step2Canvas->getCanvasSprite()->setBlendFunc({ GL_ZERO, GL_ONE_MINUS_SRC_ALPHA });
	m_step2Canvas->Node::visit();
	tmpRT->Node::visit();
	rt->end();
	DIRECTOR_RENDER;
	m_step2Canvas->getCanvasSprite()->setBlendFunc(blend);
	m_skeleton->setSlotTexture(slotName.c_str(), rt->getSprite()->getTexture());

	rt->setVisible(false);
	m_step2Canvas->setVisible(false);
	m_step2Canvas->restoreSavedPosition();
	m_step2Canvas->getCanvasSprite()->setPosition(Vec2::ZERO);
}

bool P007_MakeUp::isInFaceBox(Vec2 pos)
{
	spSkeletonBounds* bounds = spSkeletonBounds_create();
	spSkeletonBounds_update(bounds, m_skeleton->getSkeleton(), true);

	spSlot* cleanBox = m_skeleton->findSlot("box");
	spBoundingBoxAttachment* box = (spBoundingBoxAttachment*)cleanBox->attachment;
	spPolygon* poly = spSkeletonBounds_getPolygon(bounds, box);

	return spPolygon_containsPoint(poly, pos.x, pos.y) == 1;
}

#define  RANDOM_POSITION_IN_SIZE(size) Vec2(WJUtils::randomInt(size.width), WJUtils::randomInt(size.height))
Vec2 P007_MakeUp::getRandomPostion()
{
	spSkeletonBounds* bounds = spSkeletonBounds_create();
	spSkeletonBounds_update(bounds, m_skeleton->getSkeleton(), true);

	spSlot* cleanBox = m_skeleton->findSlot("box");
	spBoundingBoxAttachment* box = (spBoundingBoxAttachment*)cleanBox->attachment;
	spPolygon* poly = spSkeletonBounds_getPolygon(bounds, box);
	while (true)
	{
		Vec2 posInLayer = RANDOM_POSITION_IN_SIZE(m_pasterLayer->getContentSize());
		Vec2 posInWorld = m_pasterLayer->convertToWorldSpace(posInLayer);
		Vec2 posInSk = m_skeleton->convertToNodeSpace(posInWorld);
		if (spPolygon_containsPoint(poly, posInSk.x, posInSk.y) == 1)
		{
			spSkeletonBounds_dispose(bounds);
			return posInLayer;
		}
	} 
	return Vec2::ZERO;
}

void P007_MakeUp::addPaster(Node* node, bool isRandPos, Vec2 randPos)
{
	ScrollItem* sItem = static_cast<ScrollItem*>(node);
	string name = sItem->getItemFilePath();
	WJSprite* sp = WJSprite::create(name.c_str());

	if (m_scrollDoubleLayer->addNodeIntoStack(sp))
	{
		sp->noClickMoveEffect();
		if (isRandPos)
			sp->setPosition(getRandomPostion());
		else
			sp->setPosition(randPos);
		sp->saveCurrentProperties();
		sp->setOnTouchAble(CC_CALLBACK_2(P007_MakeUp::onPasterTouchAble, this));
		sp->setOnWillMoveTo(CC_CALLBACK_2(P007_MakeUp::onPasterTouchMove, this));
		sp->setOnTouchEnded(CC_CALLBACK_2(P007_MakeUp::onPasterTouchEnd, this));
		m_pasterLayer->addChild(sp);
	}
}

void throwSprite(Node* parent,WJSprite* sp)
{
	WJSprite* sp_throw = static_cast<WJSprite*>(sp->cloneToNode(parent, 100000));

	Vec2 pos = Director::getInstance()->getVisibleSize() / 2;
	float x = (sp_throw->getPositionWorld().x > pos.x) ? 80 : -80;

	sp_throw->runAction(Sequence::create(
		MoveBy::create(0.075f, Vec2(x, 200)),
		EaseExponentialIn::create(MoveBy::create(0.45f, Vec2(x * 2, -Director::getInstance()->getVisibleSize().height))),
		RemoveSelf::create(true),
		CallFunc::create([&]()
	{
		REMOVE_UNUSE_TEXTURE;
	}),
		nullptr));
}

void P007_MakeUp::ereasePaster(WJSprite* sp_old)
{
	if (sp_old)
	{
		throwSprite(this, sp_old);
		m_scrollDoubleLayer->deleteNodeFromInStack(sp_old);
		sp_old->removeFromParentAndCleanup(true);
	}
	else
	{
		WJSprite* sp_top = m_scrollDoubleLayer->getStackTopSprite();
		if (!sp_top)
			return;
		throwSprite(this, sp_top);
		m_scrollDoubleLayer->deleteNodeFromStackTop();
	}
	REMOVE_UNUSE_TEXTURE;
}

bool P007_MakeUp::onPasterTouchAble(Node* node, WJTouchEvent* event)
{
	return true;
}

bool P007_MakeUp::onPasterTouchMove(Node* node, WJTouchEvent* event)
{
	return true;
}

void P007_MakeUp::onPasterTouchEnd(Node* node, WJTouchEvent* event)
{
	WJSprite* sp = static_cast<WJSprite*>(node);

	Vec2 posInSk = m_skeleton->convertTouchToNodeSpace(event->touch);
	if (!isInFaceBox(posInSk))
	{
		ereasePaster((WJSprite*)node);
	}
}

void P007_MakeUp::endStep2()
{
	m_scrollDoubleLayer->removeFromParentAndCleanup(true);
	m_sceneJson->removeFromParentAndCleanup(true);
	m_sceneJson = nullptr;
	if (m_currentTool)
	{
		m_currentTool->removeFromParentAndCleanup(true);
		m_currentTool = nullptr;
	}

	REMOVE_UNUSE_TEXTURE;
}

void P007_MakeUp::initStep3()
{
	m_scrollLayer = ScrollLayer::create("P007_carmera_", ScrollDragDir::DragLeft, 5, true, true);
	m_scrollLayer->initItemSelectFromIndex(0);
	SCROLL_POSITION_IN_JSON(m_scrollLayer);
	this->addChild(m_scrollLayer, ZORDER_PROGRESS);
	schedule(schedule_selector(P007_MakeUp::updateTips3), 4.f);

	m_scrollLayer->show(0.5f);
	m_scrollLayer->setUserTag(1);
	m_scrollLayer->setClickScrollItemEvent([&](Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
	{
		stopMoveFingerTips();
		unschedule(schedule_selector(P007_MakeUp::updateTips3));
		ScrollItem* sitem = static_cast<ScrollItem*>(node);
		changeBG(sitem->getItemIndex() + 1);
	});
	m_scrollLayer->setScrollItemTouchAbleEvent([&](Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)->bool
	{
		stopMoveFingerTips();
		unschedule(schedule_selector(P007_MakeUp::updateTips3));
		return true;
	});
	m_scrollLayer->setScrollItemTouchEndedEvent([&](Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
	{
		schedule(schedule_selector(P007_MakeUp::updateTips3),4.f);
		ScrollItem* sitem = static_cast<ScrollItem*>(node);
		WJSprite* sp = sitem->getMovingSprite();
		if (sp->getUserObject())
		{
			static_cast<WJSprite *>(sp->getUserObject())->removeFromParentAndCleanup(true);
			sp->removeFromParentAndCleanup(true);
			changeBG(sitem->getItemIndex() + 1);
		}
	});
}

void P007_MakeUp::changeBG(int index)
{
	if (m_scrollLayer->getUserTag() == index)
		return;
	m_scrollLayer->setUserTag(index);

	if (m_changeBGNum != -1)
	{
		m_changeBGNum++;
		if (m_changeBGNum == 1)
		{
			showCameraTipsAction();
		}
		if (m_changeBGNum >= 3)
		{
			m_changeBGNum = -1;
			refreshStar(3);
		}
	}
	m_scrollLayer->setUserString("canShowNext");
	char buf[64];
	sprintf(buf, "game/03_minigame/p007/frame/%03d.png", index);
	m_sceneJson->getSubSprite("BG")->initWithFile(buf);

	WJUtils::playParticle("particles/yellowstar002.plist", this, ZORDER_PROGRESS + 1);
}

void P007_MakeUp::playAnimation(const char* aniName)
{
	m_skeleton->playAnimation(aniName, false);
	float delay = m_skeleton->getAnimation(aniName)->duration;
	m_skeleton->runAction(Sequence::createWithTwoActions(
		DelayTime::create(delay),
		CallFunc::create([&]()
	{
		playAnimation(1);
	})));
}

float P007_MakeUp::playAnimation(int type)
{
	m_skeleton->stopAllActions();
	m_skeleton->setEnabled(type != 2);
	switch (type)
	{
		case 1:
			{
				const int num = 4;
				const char* aniName[num] = { "aniNormal", "aniIdle001", "aniIdle002", "aniHappy" };
				playAnimation(aniName[WJUtils::randomInt(num)]);
			}
			break;
		case 2:
			{
				const int num = 5;
				const char* aniName[num] = { "aniIdle003", "aniIdle004", "aniKiss", "aniHappy", "aniAdd" };
				playAnimation(aniName[WJUtils::randomInt(num)]);
				Vec2 facePos = m_skeleton->getBonePosition("face");
				WJUtils::playParticle("particles/love.plist", m_skeleton, 100, true, false, facePos + Vec2(200, -80));
			}
			break;
		case 3:
			{
				const int num = 2;
				const char* aniName[num] = { "aniAdd","aniHappy" };
				playAnimation(aniName[WJUtils::randomInt(num)]);
				Vec2 facePos = m_skeleton->getBonePosition("face");
				WJUtils::playParticle("particles/love.plist", m_skeleton, 100, true, false, facePos + Vec2(200, -80));
			}
			break;
		case 4:
			{
				m_skeleton->playAnimation("aniClean", false);
				float delay = m_skeleton->getAnimation("aniClean")->duration;
				m_skeleton->runAction(Sequence::createWithTwoActions(
					DelayTime::create(delay),
					CallFunc::create([&](){ playAnimation(4); })));
			}
			break;
		case 5:
			{
				const int num = 3;
				const char* aniName[num] = { "aniMake001", "aniMake002", "aniMake003" };
				int index = WJUtils::randomInt(num);
				m_skeleton->playAnimation(aniName[index], false);
				float delay = m_skeleton->getAnimation(aniName[index])->duration;
				m_skeleton->runAction(Sequence::createWithTwoActions(
					DelayTime::create(delay),
					CallFunc::create([&](){ playAnimation(5); })));
			}
			break;
		case 6:
			{
				m_skeleton->playAnimation("aniHappy", false);
				return m_skeleton->getAnimation("aniHappy")->duration;
			}
			break;

	}
	return 0.f;
}


void P007_MakeUp::refreshStar(int num)
{
	if (num == 1)
	{
		setProgressStarNum(ProgressStar::oneStar, false);
	}
	else if (num == 2)
	{
		int index = 0;
		for (int i = 0; i < 4; i++)
		{
			if (m_isPlayed[i])
				index++;
		}
		if (index == 3)
			showNextButton();
		if (index == 4)
			setProgressStarNum(ProgressStar::twoStar, false);
	}
	else if (num == 3)
	{
		ProgressStar starnum = getProgressStarNum();
		setProgressStarNum((ProgressStar)((int)starnum + 1), false);
	}
}

void P007_MakeUp::updateTips1(float dt)
{
	unschedule(schedule_selector(P007_MakeUp::updateTips1));

	if (!m_step1Base)
		return;
	Vec2 endpos = m_skeleton->getBoneWorldPosition("queban") - Vec2(0, 100);
	startMoveFingerTips(m_step1Base->getPositionWorld(), endpos, 2, 0.5f);
	schedule(schedule_selector(P007_MakeUp::updateTips1), 7.f);
}

void P007_MakeUp::updateTips2(float dt)
{
	unschedule(schedule_selector(P007_MakeUp::updateTips2));
	if (!m_currentTool)
		return;
	Vec2 endpos = m_skeleton->getBoneWorldPosition("face");
	startMoveFingerTips(m_currentTool->getPositionWorld(), endpos, 2, 0.5f);
	schedule(schedule_selector(P007_MakeUp::updateTips2), 7.f);
}

void P007_MakeUp::updateTips3(float dt)
{
	unschedule(schedule_selector(P007_MakeUp::updateTips3));
	if (!m_scrollLayer)
		return;
	Vec2 endpos = m_skeleton->getBoneWorldPosition("face");
	startMoveFingerTips(m_scrollLayer->getPositionWorld(), endpos, 2, 0.5f);
	schedule(schedule_selector(P007_MakeUp::updateTips3), 7.f);
}