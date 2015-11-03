#include "DrawLayer.h"
#include "WJPenHue.h"

DrawLayer::DrawLayer()
{
	m_isDrawedAble = false;
	m_isCompleted = false;
	m_canvas = nullptr;
}

DrawLayer::~DrawLayer()
{

}

DrawLayer* DrawLayer::create(WJSprite *sprite1, WJSprite *sprite2)
{
	DrawLayer* layer = new (std::nothrow) DrawLayer();
	if (layer && layer->init(sprite1, sprite2))
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}

bool DrawLayer::init(WJSprite *sprite1, WJSprite *sprite2)
{
	if (!WJLayer::init())
		return false;
	
	// 遮罩成
	m_spriteCover = dynamic_cast<WJSprite*>(sprite1->clone());
	const Size &size = m_spriteCover->getContentSize();
	this->setContentSize(size);
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->ignoreAnchorPointForPosition(false);
	this->setPosition(sprite1->getPosition());
	m_spriteCover->setPosition(size / 2);
	m_spriteCover->setVisible(false);
	this->addChild(m_spriteCover, 1);

	// 显示剪影
	m_spriteHigher = dynamic_cast<WJSprite*>(sprite2->clone());
	m_spriteHigher->setPosition(m_spriteCover->getPosition());
	this->addChild(m_spriteHigher, 100);

	// 画布
	m_canvas = WJCanvas::create();
	m_canvas->setTouchSwallowsTouches(false);
	m_canvas->setCanvasSize(size.width, size.height);
	m_canvas->ignoreAnchorPointForPosition(false);
	m_canvas->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_canvas->setPosition(size / 2);
	BlendFunc blend = { GL_DST_ALPHA, GL_ZERO };
	m_canvas->getCanvasSprite()->setBlendFunc(blend);
	m_canvas->setVisible(false);
	m_canvas->getCanvasSprite()->getTexture()->setAntiAliasTexParameters();
	m_canvas->setTouchDrawEnabled(false);
	m_canvas->setTouchDrawOutside(false);

	// render
	m_renderTexture = RenderTexture::create(size.width, size.height);
	m_renderTexture->ignoreAnchorPointForPosition(false);
	m_renderTexture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_renderTexture->setPosition(size / 2);
	m_renderTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
	this->addChild(m_renderTexture, 50);

	// 默认为普通的画笔画笔
	WJPenHue *pen = WJPenHue::create("game/03_minigame/p011/nomal_s/001.png");
	m_canvas->setPen(pen);
	pen->setPenIndex(-1);
	pen->setPenSize(1.5f);
	pen->setPenOpacity(100);
	pen->setHueChangeAble(false);
	this->addChild(m_canvas, 50);
	m_canvas->saveCurrentProperties();
	
	scheduleUpdate();

	sprite1->removeFromParentAndCleanup(true);
	sprite2->removeFromParentAndCleanup(true);

	return true;
}


void DrawLayer::onEnterTransitionDidFinish()
{
	WJLayer::onEnterTransitionDidFinish();
}

void DrawLayer::onEnter()
{
	WJLayer::onEnter();
}

void DrawLayer::onExit()
{
	WJLayer::onExit();
}

void DrawLayer::update(float delta)
{
	WJLayer::update(delta);

	auto renderer = Director::getInstance()->getRenderer();
	auto &parentTransform = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

	m_canvas->setVisible(true);
	m_spriteCover->setVisible(true);

	m_renderTexture->beginWithClear(0, 0, 0, 0);
	m_spriteCover->visit(renderer, parentTransform, true);
	m_canvas->visit(renderer, parentTransform, true);
	m_renderTexture->end();
	Director::getInstance()->getRenderer()->render();

	m_canvas->setVisible(false);
	m_spriteCover->setVisible(false);

}

void DrawLayer::doDrawTouchAble(Node *node, WJTouchEvent *event, Vec2 &offSet)
{
	// 偏移量计算
	Point offsetTouch = m_canvas->convertTouchToNodeSpace(event->touch);
	Point pointWorld = node->getParent()->convertToWorldSpace(node->getPosition());
	Point point = m_canvas->convertToNodeSpace(pointWorld);
	m_canvas->setTouchDrawOffset(point - offsetTouch + offSet);
	m_canvas->doTouchBegan(event->touch);
}

void DrawLayer::doDrawTouchMoved(Node *node, WJTouchEvent *event)
{
	m_canvas->doTouchMoved(event->touch);
	((WJPenHue*)m_canvas->getPen())->addPenHue();
}

void DrawLayer::doDrawTouchEnded(Node *node, WJTouchEvent *event)
{
	m_canvas->doTouchEnded(event->touch);
}
