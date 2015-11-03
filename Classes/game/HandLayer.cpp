#include "HandLayer.h"
#include "NailLayer.h"
#include "ScrollItem.h"
#include "WJPenHue.h"
#include "P008Nail.h"

#define CALL_DOEVENT(_selector) if (_selector) { _selector(node, e); }

// 一共为左侧item个数 + 2 个
static bool P008ShowScrollIsFirstRun[5] = {
	true, true, true, true, true
};

static bool s_firstWristLayer = true;


HandLayer* HandLayer::create(WJLayer *layer)
{
	HandLayer* hand = new (std::nothrow) HandLayer();
	if (hand && hand->init(layer))
	{
		hand->autorelease();
		return hand;
	}
	CC_SAFE_DELETE(hand);
	return NULL;
}

bool HandLayer::init(WJLayer *layer)
{
	if (!WJLayer::init())
		return false;

	const Size &size = layer->getContentScaleSize();
	WJBase::copyProperties(layer, this);
	this->setContentSize(size);
	this->setScale(1.0f);
	
	// hand layer
	m_handLayer = dynamic_cast<WJLayer*>(layer->clone());
	m_handLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_handLayer->ignoreAnchorPointForPosition(false);
	m_handLayer->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(m_handLayer, 10);

	// 手环
	m_bandSprite = WJSprite::create("game/03_minigame/p008/bracelet/001.png");
	m_bandSprite->setPosition(Vec2(size.width / 2, size.height / 2) + Vec2(-85, -50));
	m_bandSprite->setVisible(false);
	this->addChild(m_bandSprite, 20);

	initNailLayer(layer);

	return true;
}

HandLayer::HandLayer()
{
	m_handLayer = nullptr;
	m_nailVector.clear();
	m_Handstate = HandLayerState::normalShowAllState;
	m_clickNormalNailCallBack = nullptr;
	m_usedWhole = nullptr;
	m_back = nullptr;
	m_earseSprite = nullptr;
	m_colorTool = nullptr;
	m_usedWhole = nullptr;
	m_bandSprite = nullptr;
	m_wristScrollLayer = nullptr;
	m_chooseWristCallBack = nullptr;
}

HandLayer::~HandLayer()
{

}

void HandLayer::onEnterTransitionDidFinish()
{
	WJLayer::onEnterTransitionDidFinish();
}

void HandLayer::initNailLayer(WJLayer *oldLayer)
{
	const Size &size = Director::getInstance()->getWinSize();
	// 返回
	m_back = WJButton::create("button/hand.png");
	m_back->setPosition(oldLayer->getParent()->convertToNodeSpace(Vec2(size.width / 2 - 65, size.height - 50)));
	m_back->noClickMoveEffect();
	m_back->setClickAniType(WJButton::ClickAniType::Scale);
	m_back->setNormalAniType(WJButton::NormalAniType::QBreathing);
	m_back->setOnClick(CC_CALLBACK_2(HandLayer::onClickBackButton, this));
	m_back->setVisible(false);
	oldLayer->getParent()->addChild(m_back, 100);

	// 应用到所有
	m_usedWhole = WJButton::create("button/hand.png");
	m_usedWhole->setPosition(oldLayer->getParent()->convertToNodeSpace(Vec2(size.width / 2 + 65, size.height - 50)));
	m_usedWhole->noClickMoveEffect();
	m_usedWhole->setClickAniType(WJButton::ClickAniType::Scale);
	m_usedWhole->setNormalAniType(WJButton::NormalAniType::QBreathing);
	m_usedWhole->setVisible(false);
	m_usedWhole->setOnClick(CC_CALLBACK_2(HandLayer::onClickUsedWholeButton, this));
	oldLayer->getParent()->addChild(m_usedWhole, 100);

	// 擦除
	m_earseSprite = WJSprite::create("game/03_minigame/p008/eraser.png");
	m_earseSprite->setMoveAble(true);
	m_earseSprite->noClickMoveEffect();
	m_earseSprite->setTouchSwallowsTouches(false);
	m_earseSprite->setPosition(oldLayer->getParent()->convertToNodeSpace(Vec2(180, 200)));
	m_earseSprite->setOnTouchAble(CC_CALLBACK_2(HandLayer::onEarseTouchAble, this));
	m_earseSprite->setOnWillMoveTo(CC_CALLBACK_2(HandLayer::onEarseWillMoveTo, this));
	m_earseSprite->setOnTouchEnded(CC_CALLBACK_2(HandLayer::onEarseTouchEnded, this));
	m_earseSprite->setVisible(false);
	oldLayer->getParent()->addChild(m_earseSprite, 800);
	m_earseSprite->saveCurrentProperties();

	// 笔刷
	m_colorTool = WJSprite::create("game/03_minigame/p008/nailcolorbrush/001.png");
	m_colorTool->setPosition(oldLayer->getParent()->convertToNodeSpace(Vec2(size.width - 330, 250)));
	m_colorTool->setUserTag(1);
	m_colorTool->setMoveAble(true);
	m_colorTool->noClickMoveEffect();
	m_colorTool->setVisible(false);
	m_colorTool->setRotation(10);
	m_colorTool->setOnTouchAble(CC_CALLBACK_2(HandLayer::onColorToolTouchAble, this));
	m_colorTool->setOnWillMoveTo(CC_CALLBACK_2(HandLayer::onColorToolWillMoveTo, this));
	m_colorTool->setOnTouchEnded(CC_CALLBACK_2(HandLayer::onColorToolTouchEnded, this));
	oldLayer->getParent()->addChild(m_colorTool, 800);
	m_colorTool->saveCurrentProperties();

	// 选择弹窗
	m_scrollLayer = ScrollDoubleLayer::create("P008_left_", ScrollDragDir::StopDrag, 3, true, P008ShowScrollIsFirstRun);
	m_scrollLayer->setClickScrollItemEvent(CC_CALLBACK_3(HandLayer::onClickScrollItemCallBack, this));
	m_scrollLayer->setClickCancelItemEvent(CC_CALLBACK_3(HandLayer::onClickCancelItemCallBack, this));
	m_scrollLayer->setClickMenuScrollItemEvent(CC_CALLBACK_3(HandLayer::onClickMenuScrollItemCallBack, this));
	m_scrollLayer->setScrollItemTouchAbleEvent(CC_CALLBACK_3(HandLayer::onScrollItemTouchAble, this));
	m_scrollLayer->setScrollItemDragOutCallBack(CC_CALLBACK_3(HandLayer::onScrollItemDragOutCallBack, this));
	m_scrollLayer->setScrollItemMovingCollisionEvent(CC_CALLBACK_3(HandLayer::onScrollItemMovingCollision, this));
	m_scrollLayer->setScrollItemTouchEndedEvent(CC_CALLBACK_3(HandLayer::onScrollItemTouchEnded, this));
	m_scrollLayer->setScrollItemCollisonEndedEvent(CC_CALLBACK_3(HandLayer::onScrollItemCollisonEnded, this));
	m_scrollLayer->setAutoDealCancel(AUTO_DEAL_CANCEL_START_INDEX, 2, AUTO_DEAL_CANCEL_END_INDEX);
	m_scrollLayer->setStackMaxNum(20);
	m_scrollLayer->setPosition(oldLayer->getParent()->convertToNodeSpace(Vec2(size.width + 300, size.height / 2 - 300)));
	oldLayer->getParent()->addChild(m_scrollLayer, 600);

	// 手环
	m_wristScrollLayer = ScrollLayer::create("P008_bracelet_", ScrollDragDir::DragLeft, 10, true, s_firstWristLayer);
	m_wristScrollLayer->setClickScrollItemEvent(CC_CALLBACK_3(HandLayer::onClickWristStyleScrollItem, this));
	//m_wristScrollLayer->setScrollItemMovingCollisionEvent(CC_CALLBACK_3(P008::onScrollStyleMovingCollision, this));
	//m_wristScrollLayer->setScrollItemCollisonEndedEvent(CC_CALLBACK_3(P008::onScrollStyleCollisonEndedEvent, this));
	m_wristScrollLayer->setPosition(oldLayer->getParent()->convertToNodeSpace(Vec2(size.width + 300, size.height / 2 - 300)));
	oldLayer->getParent()->addChild(m_wristScrollLayer, 600);
	m_wristScrollLayer->initItemSelectFromIndex(0);
	s_firstWristLayer = false;

	// 单个指甲
	WJSprite *sprite = nullptr;
	NailLayer *layer = nullptr;
	for (int i = 0; i < 5; i++)
	{
		std::string key = WJUtils::stringAddInt("nail", i + 1);
		sprite = dynamic_cast<WJSprite*>(m_handLayer->getChildByName(key.c_str()));
		layer = NailLayer::create(sprite);
		layer->setNailNo(i + 1);
		layer->setNailStyleNo(4);
		layer->setName(key.c_str());
		layer->_scrollLayer = m_scrollLayer;
		layer->setClickNormalNailCallBack(CC_CALLBACK_2(HandLayer::onClickNormalNail, this));
		m_handLayer->addChild(layer, sprite->getLocalZOrder());
		m_nailVector.pushBack(layer);

		sprite->removeFromParentAndCleanup(true);
		sprite = nullptr;
	}
}

void HandLayer::onClickScrollItemCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	// 颜色
	if (m_Handstate == HandLayerState::drawingSingleNailColorState)
		onClickScrollColorItem(node, touchEvent, scroll);

	// 花纹
	else if (m_Handstate == HandLayerState::drawingSingleNailPatternState)
		onClickScrollPatternItem(node, touchEvent, scroll);

	// 装饰品
	else if (m_Handstate == HandLayerState::drawingSingleNailDecorState)
		getSelectNailLayer()->onClickScrollDecorItem(node, touchEvent, scroll);
}

void HandLayer::onClickWristStyleScrollItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	// 选择花纹
	if (m_Handstate == HandLayerState::wristbandHandState)
	{
		ScrollItem *item = (ScrollItem*)node;
		m_bandSprite->setVisible(true);
		m_bandSprite->loadSpriteTexture(item->getItemFilePath().c_str());

		if (m_chooseWristCallBack)
			m_chooseWristCallBack(node, touchEvent);
	}
}

void HandLayer::onClickCancelItemCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	// 取消花纹
	if (m_Handstate == HandLayerState::drawingSingleNailPatternState)
		getSelectNailLayer()->visitPatternTexture(BASE_NONE_PNG, true);
}

void HandLayer::onClickMenuScrollItemCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	int index = item->getItemIndex();
	switch (index)
	{
		// 颜色
		case 0:
			m_Handstate = HandLayerState::drawingSingleNailColorState;
			break;
		// 花纹
		case 1:
			m_Handstate = HandLayerState::drawingSingleNailPatternState;
			break;
		// 装饰品
		case 2:
			m_Handstate = HandLayerState::drawingSingleNailDecorState;
			break;
		default:
			break;
	}
	// 改变当前选中指甲的状态和设置工具的隐藏性
	changeNailStateAndToolVisible(node, touchEvent, scroll);
}

void HandLayer::changeNailStateAndToolVisible(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	if (m_Handstate == HandLayerState::drawingSingleNailColorState)
	{
		getSelectNailLayer()->changeNailState(NailState::drawingColorState);
		m_earseSprite->setVisible(true);
	}
	else if (m_Handstate == HandLayerState::drawingSingleNailPatternState)
	{
		getSelectNailLayer()->changeNailState(NailState::drawingPatternState);
		// 工具的移除和移入
		m_earseSprite->setVisible(false);
		onScrollColorDragOutCallBack(node, touchEvent, scroll);
	}
	else if (m_Handstate == HandLayerState::drawingSingleNailDecorState)
	{
		getSelectNailLayer()->changeNailState(NailState::drawingDecorState);
		// 工具的隐藏
		m_earseSprite->setVisible(false);
		onScrollColorDragOutCallBack(node, touchEvent, scroll);
	}
}

void HandLayer::onClickScrollColorItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem *)node;
	// 显示画笔
	m_colorTool->setVisible(true);
	m_colorTool->loadSpriteTexture(WJUtils::stringAddInt("game/03_minigame/p008/nailcolorbrush/",
		item->getItemIndex() + 1, 3).append(".png").c_str());
	m_colorTool->setUserTag(item->getItemIndex() + 1);

	// 笔刷出来后弹一下
	m_colorTool->stopAllActions();
	PUtils::playNodeScaleAni(m_colorTool, 1, 1.5f);
	PUtils::playSuspenEffect(m_colorTool, 1.0f);
}

void HandLayer::onClickScrollPatternItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	getSelectNailLayer()->visitPatternTexture(item->getItemFilePath().c_str());
}

bool HandLayer::onScrollPatternMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	return PUtils::isOneCollideWithOne(item->getMovingSprite(), getSelectNailLayer()->getNailSprite());
}

bool HandLayer::onScrollDecorMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	return getSelectNailLayer()->onScrollDecorMovingCollision(node, touchEvent, scroll);
}

void HandLayer::onScrollPatternCollisonEndedEvent(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	WJSprite *sprite = item->getMovingSprite();
	WJSprite* newSprite = dynamic_cast<WJSprite*>(sprite->getUserObject());
	newSprite->followNode(nullptr);
	// 替换花纹
	getSelectNailLayer()->visitPatternTexture(item->getItemFilePath().c_str());
	// 移除掉新产生的花纹
	sprite->removeFromParentAndCleanup(true);
	sprite = NULL;
	newSprite->removeFromParentAndCleanup(true);
	newSprite = nullptr;
}

void HandLayer::onScrollDecorCollisonEndedEvent(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	getSelectNailLayer()->onScrollDecorCollisonEndedEvent(node, touchEvent, scroll);
}

bool HandLayer::onColorToolTouchAble(Node *node, WJTouchEvent *event)
{
	WJSprite *sprite = (WJSprite*)node;
	sprite->stopAllActions();
	sprite->restoreSavedScale();
	doCanvasTouchBegan(node, event, sprite->getUserTag());
	return true;
}

bool HandLayer::onColorToolWillMoveTo(Node *node, WJTouchEvent *event)
{
	((WJPenHue*)getSelectNailLayer()->getNailCanvas()->getPen())->addPenHue();
	getSelectNailLayer()->getNailCanvas()->doTouchMoved(event->touch);

	return true;
}

void HandLayer::onColorToolTouchEnded(Node *node, WJTouchEvent *event)
{
	WJSprite *sprite = (WJSprite*)node;
	getSelectNailLayer()->getNailCanvas()->doTouchEnded(event->touch);
	sprite->restoreSavedPosition(0.3f, false, true);
	PUtils::playSuspenEffect(m_colorTool, 1.0f);
	checkNailLayerColor();
}

void HandLayer::checkNailLayerColor()
{
	Image *image = getSelectNailLayer()->getNailCanvas()->getCanvasImageNew();
	float f = PUtils::getImgBlankspace(image);
	if (f <= 0.2f)
	{
		P008 *base = (P008*)(this->getParent()->getParent());
		base->setStarNumWhenMakeNail(ProgressStar::oneStar);
	}

	CC_SAFE_DELETE(image);
}

void HandLayer::checkBothNailDecorNum()
{
	NailLayer* layer;
	for (int i = 0; i < m_nailVector.size(); i++)
	{
		layer = m_nailVector.at(i);
		if (layer->getDecorLayer()->getChildrenCount() < 3)
			return;
	}

	// 所有手指都放了3个以上的装饰物
	P008 *base = (P008*)(this->getParent()->getParent());
	base->setStarNumWhenMakeNail(ProgressStar::twoStar);
}

bool HandLayer::onScrollColorTouchAble(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem *)node;
	doCanvasTouchBegan(node, touchEvent, item->getItemIndex() + 1);

	return true;
}

bool HandLayer::onScrollItemTouchAble(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	if (m_Handstate == HandLayerState::drawingSingleNailColorState)
		return onScrollColorTouchAble(node, touchEvent, scroll);

	return true;
}

void HandLayer::onScrollColorDragOutCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	m_colorTool->setVisible(false);
	m_colorTool->stopAllActions();
	m_colorTool->restoreSavedPosition();
	m_colorTool->restoreSavedScale();
}

void HandLayer::onScrollItemDragOutCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	if (m_Handstate == HandLayerState::drawingSingleNailColorState)
		onScrollColorDragOutCallBack(node, touchEvent, scroll);
}

void HandLayer::doCanvasTouchBegan(Node *node, WJTouchEvent *touchEvent, int index)
{
	WJCanvas *canvas = getSelectNailLayer()->getNailCanvas();
	// 偏移量计算
	Point offset = canvas->convertTouchToNodeSpace(touchEvent->touch);
	Point pointWorld = node->getParent()->convertToWorldSpace(node->getPosition());
	Point point = canvas->convertToNodeSpace(pointWorld);
	canvas->setTouchDrawOffset(point - offset + Vec2(-80, -160));
	canvas->doTouchBegan(touchEvent->touch);

	// 改变画笔的颜色
	if (((WJPenHue*)canvas->getPen())->getPenIndex() != index - 1)
	{
		WJPenHue *pen = WJPenHue::create(WJUtils::stringAddInt("game/03_minigame/p008/nailcolor/", index, 3).append(".png").c_str());
		pen->setPenSize(canvas->getPen()->getPenSize());
		pen->setPenOpacity(canvas->getPen()->getPenOpacity());
		pen->setHueChangeAble(index >= 9);
		canvas->setPen(pen);
	}
}

bool HandLayer::onScrollColorMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	((WJPenHue*)getSelectNailLayer()->getNailCanvas()->getPen())->addPenHue();
	getSelectNailLayer()->getNailCanvas()->doTouchMoved(touchEvent->touch);

	return false;
}

bool HandLayer::onScrollItemMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	if (m_Handstate == HandLayerState::drawingSingleNailColorState)
		return onScrollColorMovingCollision(node, touchEvent, scroll);
	else if (m_Handstate == HandLayerState::drawingSingleNailPatternState)
		return onScrollPatternMovingCollision(node, touchEvent, scroll);
	else if (m_Handstate == HandLayerState::drawingSingleNailDecorState)
		return onScrollDecorMovingCollision(node, touchEvent, scroll);
	
	return false;
}

void HandLayer::onScrollItemTouchEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	if (m_Handstate == HandLayerState::drawingSingleNailColorState)
		setScrollColorTouchEnded(node, touchEvent, scroll);
}

void HandLayer::onScrollItemCollisonEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	if (m_Handstate == HandLayerState::drawingSingleNailPatternState)
		onScrollPatternCollisonEndedEvent(node, touchEvent, scroll);
	else if (m_Handstate == HandLayerState::drawingSingleNailDecorState)
		onScrollDecorCollisonEndedEvent(node, touchEvent, scroll);
}

void HandLayer::setScrollColorTouchEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	checkNailLayerColor();
	getSelectNailLayer()->getNailCanvas()->doTouchEnded(touchEvent->touch);
}

bool HandLayer::onEarseTouchAble(Node *node, WJTouchEvent *event)
{
	// 偏移量计算
	WJCanvas *canvas = getSelectNailLayer()->getNailCanvas();
	Point offset = canvas->convertTouchToNodeSpace(event->touch);
	Point pointWorld = node->getParent()->convertToWorldSpace(node->getPosition());
	Point point = canvas->convertToNodeSpace(pointWorld);
	
	canvas->setTouchDrawOffset(point - offset + Vec2(0, 0));
	canvas->getPen()->setPenEraserMode(true);
	canvas->getPen()->setPenSize(2.5f);
	canvas->doTouchBegan(event->touch);

	return true;
}

bool HandLayer::onEarseWillMoveTo(Node *node, WJTouchEvent *event)
{
	getSelectNailLayer()->getNailCanvas()->doTouchMoved(event->touch);
	return true;
}

void HandLayer::onEarseTouchEnded(Node *node, WJTouchEvent *event)
{
	getSelectNailLayer()->getNailCanvas()->doTouchEnded(event->touch);
	m_earseSprite->restoreSavedPosition(0.3f, false, true);
	getSelectNailLayer()->getNailCanvas()->getPen()->setPenEraserMode(false);
	getSelectNailLayer()->getNailCanvas()->getPen()->setPenSize(getSelectNailLayer()->getPenSize());
}

NailLayer* HandLayer::getSelectNailLayer()
{
	NailLayer *layer;
	for (ssize_t i = 0; i < m_nailVector.size(); i++)
	{
		layer = m_nailVector.at(i);
		if (layer->getNailState() == NailState::drawingColorState || 
			layer->getNailState() == NailState::drawingPatternState ||
			layer->getNailState() == NailState::drawingDecorState)

			return layer;
	}

	return nullptr;
}

void HandLayer::onClickBackButton(Node *node, WJTouchEvent *event)
{
	if (m_back->isVisible())
	{
		// 隐藏涂抹指甲需要的按钮
		m_back->setVisible(false);
		m_usedWhole->setVisible(false);
		m_earseSprite->setVisible(false);
		onScrollColorDragOutCallBack(node, event, nullptr);
		this->restoreSavedPositionAndScale();
		setNailAndFingerVisibleExceptDrawingNail(getSelectNailLayer(), true);
		// 滚动条移除屏幕
		m_scrollLayer->moveOutSideScene(0.0f, true);
	}
}

void HandLayer::changeNewStyleNail(int styleNo, int nailno /*= -1*/)
{
	if (m_nailVector.size() <= 0)
		return;

	NailLayer *layer;
	for (ssize_t i = 0; i < m_nailVector.size(); i++)
	{
		layer = m_nailVector.at(i);
		if (nailno == -1)
		{
			layer->changeNewStyleNail(styleNo);
		}
		else if (nailno != -1 && layer->getNailNo() == nailno)
		{
			layer->changeNewStyleNail(styleNo);
			break;
		}
	}
}

void HandLayer::onClickNormalNail(Node *node, WJTouchEvent *event)
{
	// 当前显示整个手的时候点击单个的指甲进入指甲修饰
	if (m_Handstate == HandLayerState::normalShowAllState)
	{
		m_Handstate = HandLayerState::begingShowSingleNailState;
		NailLayer *layer = (NailLayer*)node;
		Vec2 point = layer->getPositionInNode(this);
		const Size &size = this->getContentSize();
		WJUtils::setNodeAnchorPoint(this, Vec2(point.x / size.width, point.y / size.height));
		this->saveCurrentPosition();

		// 当前指甲需要移动到的世界坐标
		const Size &winSize = Director::getInstance()->getWinSize();
		const Vec2 &worldPoint = Vec2(winSize.width / 2 - 80, Common::getAdsSize().height + 200);
		// 计算其父控件的世界坐标
		const Vec2 &movepoint = this->getParent()->convertToNodeSpace(worldPoint);

		this->runAction(Sequence::create(
			Spawn::create(
				RotateBy::create(0.5f, -layer->getRotation()),
				ScaleTo::create(0.5f, HAND_LAYER_SCALE_NUM),
				MoveTo::create(0.5f, movepoint),
			nullptr),
		CCCallFunc::create(CC_CALLBACK_0(HandLayer::ClickNormalNailEnded, this, layer)),
		nullptr));

		// 移除选择指甲样式的滚动条
		doEventClickNormalNailCallBack(node, event);
		// 显示涂色滚动条
		m_scrollLayer->show(1.0f);
	}
}

void HandLayer::ClickNormalNailEnded(NailLayer *layer)
{
	// 显示指甲装饰界面
	m_Handstate = HandLayerState::drawingSingleNailColorState;
	m_back->setVisible(true);
	m_earseSprite->setVisible(true);
	m_usedWhole->setVisible(true);

	setNailAndFingerVisibleExceptDrawingNail(layer, false);
	m_scrollLayer->setStackIndex(layer->getNailNo() - 1);
	m_scrollLayer->setDealCancelIndex(0);
}

void HandLayer::setNailAndFingerVisibleExceptDrawingNail(NailLayer *layer, bool visible)
{
	// 其余手指的隐藏
	WJSprite *finger;
	NailLayer *nail;
	for (int i = 0; i < 5; i++)
	{
		if (layer->getNailNo() == i + 1)
			continue;

		// 手指
		std::string key = WJUtils::stringAddInt("hand", i + 1, 3);
		finger = dynamic_cast<WJSprite*>(m_handLayer->getChildByName(key.c_str()));
		finger->setVisible(visible);

		// 指甲layer
		std::string nailKey = WJUtils::stringAddInt("nail", i + 1);
		nail = dynamic_cast<NailLayer*>(m_handLayer->getChildByName(nailKey.c_str()));
		nail->setVisible(visible);
	}

	setHandWholeVisible(visible);
}

void HandLayer::restoreSavedPositionAndScale()
{
	if (m_Handstate == HandLayerState::drawingSingleNailColorState ||
		m_Handstate == HandLayerState::drawingSingleNailPatternState ||
		m_Handstate == HandLayerState::drawingSingleNailDecorState)
	{
		m_Handstate = HandLayerState::begingEndSingleNailState;
		this->stopAllActions();
		this->runAction(Sequence::create(
			Spawn::create(
			RotateTo::create(0.5f, this->getSavedRotation()),
			ScaleTo::create(0.5f, this->getSavedScale()),
			MoveTo::create(0.5f, this->getSavedPosition()),
			nullptr),
			CCCallFunc::create(CC_CALLBACK_0(HandLayer::onRestoreSavedPosEnd, this)),
		nullptr));
	}
	
}

void HandLayer::onRestoreSavedPosEnd()
{
	if (m_nailVector.size() <= 0)
		return;
	NailLayer *layer;
	for (int i = 0; i < 5; i++)
	{
		layer = m_nailVector.at(i);
		layer->changeNailState(NailState::NormalState);
		layer->getNailCanvas()->setTouchDrawEnabled(false);
		
		if (m_Handstate != HandLayerState::wristbandHandState)
			layer->startplayCircleAni(3.5f);
	}
	WJUtils::setNodeAnchorPoint(this, Vec2::ANCHOR_MIDDLE);

	// 进入了装饰手环就不能在操作手指了
	if (m_Handstate != HandLayerState::wristbandHandState)
		m_Handstate = HandLayerState::normalShowAllState;

}

void HandLayer::setHandWholeVisible(bool visible)
{
	m_handLayer->getChildByName("hand")->setVisible(visible);
	m_handLayer->getChildByName("handall")->setVisible(visible);
}

void HandLayer::doEventClickNormalNailCallBack(Node *node, WJTouchEvent *e)
{
	CALL_DOEVENT(m_clickNormalNailCallBack);
}

void HandLayer::setClickNormalNailCallBack(const WJTouchCallback &callback)
{
	m_clickNormalNailCallBack = callback;
}

int HandLayer::checkStyleCalculateAABBInWorld(ScrollItem *item)
{
	Vec2& point = item->getMovingSprite()->getPositionWorld();
	NailLayer *layer;
	for (ssize_t i = 0; i < m_nailVector.size(); i++)
	{
		layer = m_nailVector.at(i);
		Rect rect = WJUtils::calculateAABBInWorld(layer->getNailSprite());
		if (rect.containsPoint(point))
			return layer->getNailNo();
	}

	return -1;
}

void HandLayer::onClickUsedWholeButton(Node *node, WJTouchEvent *event)
{
	WJUtils::showMessageYesNo(NAIL_APPLY_TO_ALL_MESSAGE, 8998, [this](int i, const char* msg){
		// 应用到所有
		if (WJUtils::equals("Y", msg))
		{
			// 当前正在操作的指甲
			NailLayer *selcetLayer = this->getSelectNailLayer();

			// 克隆指甲
			NailLayer *layer;
			for (ssize_t i = 0; i < m_nailVector.size(); i++)
			{
				layer = m_nailVector.at(i);
				if (layer->getNailNo() == selcetLayer->getNailNo())
					continue;
				
				// 改变为已经装饰过指甲
				layer->setIsDrawed(true);

				// 克隆画布、渲染颜色
				selcetLayer->getNailCanvas()->cloneTo(layer->getNailCanvas());
				layer->visitColor();

				// 替换花纹、渲染花纹
				layer->visitPatternTexture(selcetLayer->getPatternFile().c_str(), true);
				layer->visitPattern();

				// 克隆装饰物
				selcetLayer->cloneDecorToNode(layer);
			}
			// 返回
			onClickBackButton(m_back, nullptr);
		}
	});
}

void HandLayer::gotoWristBandMode()
{
	onClickBackButton(m_back, nullptr);
	m_Handstate = HandLayerState::wristbandHandState;
	m_wristScrollLayer->show(1.5f);
}

void HandLayer::setChooseWristCallBack(const WJTouchCallback &callback)
{
	m_chooseWristCallBack = callback;
}
