#include "P011Draw.h"
#include "WJPenHue.h"

INIT_SCENE_LOADER_SYNC(P011_Draw);

// 一共为左侧item个数 + 2 个
static bool sideScrollIsFirstRun[6] = {
	true, true, true, true, true, true
};

static bool s_firstDrawLayer = true;

P011_Draw::P011_Draw()
{
	m_scrollLayer = nullptr;
	m_earseSprite = nullptr;
	m_drawState = DrawLayerState::drawingState;
}

P011_Draw::~P011_Draw()
{

}

bool P011_Draw::init()
{
	if ( !PBase::init() )
	{
		return false;
	}

	initSceneJson("game/json/p011_001_b.json");

	initScrollLayer();

	initDrawLayer();
	
	return true;
}

void P011_Draw::initScrollLayer()
{
	m_scrollLayer = ScrollLayer::create("P011_draw_", ScrollDragDir::DragLeft, 18, false, s_firstDrawLayer);
	m_scrollLayer->setScrollItemTouchAbleEvent(CC_CALLBACK_3(P011_Draw::onScrollLayerTouchAble, this));
	m_scrollLayer->setScrollItemMovingCollisionEvent(CC_CALLBACK_3(P011_Draw::onScrollLayerMovingCollision, this));
	m_scrollLayer->setScrollItemCollisonEndedEvent(CC_CALLBACK_3(P011_Draw::onScrollItemCollisonEnded, this));
	m_scrollLayer->setScrollItemTouchEndedEvent(CC_CALLBACK_3(P011_Draw::onScrollLayerTouchEnded, this));
	m_scrollLayer->setScrollItemDragOutCallBack(CC_CALLBACK_3(P011_Draw::onScrollItemDragOutCallBack, this));
	SCROLL_POSITION_IN_JSON(m_scrollLayer);
	m_sceneJson->addChild(m_scrollLayer, 600);
	m_scrollLayer->initItemSelectFromIndex(0);
	s_firstDrawLayer = false;

	// 擦除
	m_earseSprite = m_sceneJson->getSubSprite("eraser");
	m_earseSprite->setMoveAble(true);
	m_earseSprite->noClickMoveEffect();
	m_earseSprite->setTouchSwallowsTouches(false);
	m_earseSprite->setOnTouchAble(CC_CALLBACK_2(P011_Draw::onEarseTouchAble, this));
	m_earseSprite->setOnWillMoveTo(CC_CALLBACK_2(P011_Draw::onEarseWillMoveTo, this));
	m_earseSprite->setOnTouchEnded(CC_CALLBACK_2(P011_Draw::onEarseTouchEnded, this));
	m_earseSprite->saveCurrentProperties();
}

bool P011_Draw::onScrollLayerTouchAble(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	if (m_drawState == DrawLayerState::drawingState)
	{
		for (DrawLayer* layer : m_drawLayerArray)
		{
			layer->doDrawTouchAble(node, touchEvent);
		}
	}

	return true;
}

bool P011_Draw::onScrollLayerMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	if (m_drawState == DrawLayerState::drawingState)
	{
		for (DrawLayer* layer : m_drawLayerArray)
		{
			layer->doDrawTouchMoved(node, touchEvent);
		}
	}

	return false;
}

void P011_Draw::onScrollLayerTouchEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	if (m_drawState == DrawLayerState::drawingState)
	{
		for (DrawLayer* layer : m_drawLayerArray)
		{
			layer->doDrawTouchEnded(node, touchEvent);
			WJCanvas *canvas = layer->getCanvas();
			Image *image = canvas->getCanvasImageNew();
			float blankSpace = PUtils::getImgBlankspace(image);
			if (blankSpace <= 0.35f)
				showNextButton();

			CC_SAFE_DELETE(image);
		}
	}
}

void P011_Draw::onScrollItemCollisonEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	WJSprite *sprite = item->getMovingSprite();

	WJSprite* newSprite = dynamic_cast<WJSprite*>(sprite->getUserObject());
	DrawLayer *layer = (DrawLayer*)item->getItemUserData();

	newSprite->followNode(nullptr);
	sprite->removeFromParentAndCleanup(true);
	sprite = NULL;

	Point pointWorld = newSprite->getPositionWorld();
	Point point = layer->convertToNodeSpace(pointWorld);

	newSprite->retain();
	newSprite->removeFromParentAndCleanup(true);
	newSprite->setPosition(point);
	layer->addChild(newSprite, 100);
	// 已经放在脸上的装饰品
	newSprite->setMoveAble(true);
	newSprite->setTouchSwallowsTouches(true);
	newSprite->noClickMoveEffect();
	newSprite->release();
}

void P011_Draw::onScrollItemDragOutCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	for (DrawLayer* layer : m_drawLayerArray)
	{
		WJCanvas *canvas = layer->getCanvas();
		WJPenHue *pen = WJPenHue::create(item->getItemIconFile().c_str());
		pen->setPenSize(canvas->getPen()->getPenSize());
		pen->setPenOpacity(canvas->getPen()->getPenOpacity());
		pen->setPenIndex(item->getItemIndex());
		pen->addPenHue(0);
		pen->setHueChangeAble(item->getItemIndex() >= 13);
		canvas->setPen(pen);
	}
}

GameNumber P011_Draw::getGameNumber()
{
	return GameNumber::P011_Draw;
}

void P011_Draw::onEnterTransitionDidFinish()
{
	PBase::onEnterTransitionDidFinish();
	m_scrollLayer->show(0.5f);
}

void P011_Draw::initDrawLayer()
{
	// 画布
	DrawLayer *drawLayer;

	drawLayer = DrawLayer::create(m_sceneJson->getSubSprite("001_s"), m_sceneJson->getSubSprite("001"));
	m_sceneJson->addChild(drawLayer, 100);
	m_drawLayerArray.pushBack(drawLayer);

	drawLayer = DrawLayer::create(m_sceneJson->getSubSprite("002_s"), m_sceneJson->getSubSprite("002"));
	m_sceneJson->addChild(drawLayer, 100);
	m_drawLayerArray.pushBack(drawLayer);

	drawLayer = DrawLayer::create(m_sceneJson->getSubSprite("003_s"), m_sceneJson->getSubSprite("003"));
	m_sceneJson->addChild(drawLayer, 100);
	m_drawLayerArray.pushBack(drawLayer);
}

bool P011_Draw::onEarseTouchAble(Node *node, WJTouchEvent *event)
{
	m_earseSprite->setLocalZOrder(1000);

	for (DrawLayer* layer : m_drawLayerArray)
	{
		layer->getCanvas()->getPen()->setPenEraserMode(true);
		layer->doDrawTouchAble(node, event, Vec2(0, 40));
	}

	return true;
}

bool P011_Draw::onEarseWillMoveTo(Node *node, WJTouchEvent *event)
{
	for (DrawLayer* layer : m_drawLayerArray)
	{
		layer->doDrawTouchMoved(node, event);
	}

	return true;
}

void P011_Draw::onEarseTouchEnded(Node *node, WJTouchEvent *event)
{
	m_earseSprite->restoreSavedPosition(0.3f, false, true);
	m_earseSprite->restoreSavedZOrder();
	for (DrawLayer* layer : m_drawLayerArray)
	{
		layer->getCanvas()->getPen()->setPenEraserMode(false);
		layer->doDrawTouchEnded(node, event);
	}	
}
