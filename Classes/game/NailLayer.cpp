#include "NailLayer.h"
#include "WJPenHue.h"
#include "HandLayer.h"

#define CALL_DOEVENT(_selector) if (_selector) { _selector(node, e); }

#define PLAY_NAIL_CIRCLE_ANI_TAG              6655               // 提示动画

static bool s_firstDecorNailLayer = true;

NailLayer* NailLayer::create(WJSprite *sprite)
{
	NailLayer* layer = new (std::nothrow) NailLayer();
	if (layer && layer->init(sprite))
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}

bool NailLayer::init(WJSprite *sprite)
{
	if (!WJLayer::init())
		return false;
	const Size &size = sprite->getContentSize();
	this->setContentSize(size);
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	this->ignoreAnchorPointForPosition(false);
	this->setScaleX(sprite->getScaleX());
	this->setScaleY(sprite->getScaleY());
	//this->setScale(sprite->getScaleX());
	this->setPosition(sprite->getPosition());
	this->setRotation(sprite->getRotation());
	
	// 指甲
	m_sprite = dynamic_cast<WJSprite*>(sprite->clone());
	m_sprite->setScale(1.0f);
	m_sprite->setRotation(0.0f);
	m_sprite->setPosition(Vec2(size.width / 2, 0));
	this->addChild(m_sprite, 80);

	this->noClickMoveEffect();
	this->setTouchSwallowsTouches(false);
	this->setCustomizeTouchRect(Rect(0, 0, size.width, size.height), true);
	this->setOnClick(CC_CALLBACK_2(NailLayer::onClickNailSprite, this));

	// 装饰layer
	m_decorLayer = WJLayer::create();
	m_decorLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_decorLayer->ignoreAnchorPointForPosition(false);
	m_decorLayer->setContentSize(size);
	m_decorLayer->setPosition(size / 2);
	this->addChild(m_decorLayer, 500);

	initCanvasLayer();

	initImage();

	scheduleUpdate();

	initNailTips();

	return true;
}

void NailLayer::initNailTips()
{
	// 提示圆圈
	const Size &size = this->getContentSize();

	m_circleSprite1 = WJSprite::create("game/03_minigame/p008/cicle001.png");
	m_circleSprite1->setPosition(Vec2(size.width / 2, size.height / 2));
	m_circleSprite1->setOpacity(200);
	m_circleSprite1->setVisible(false);
	m_circleSprite1->setScaleX(1.5f / this->getScaleX());
	m_circleSprite1->setScaleY(1.5f / this->getScaleY());
	m_circleSprite1->saveCurrentProperties();
	this->addChild(m_circleSprite1, 900);

	m_circleSprite2 = WJSprite::create("game/03_minigame/p008/cicle001.png");
	m_circleSprite2->setPosition(Vec2(size.width / 2, size.height / 2));
	m_circleSprite2->setOpacity(200);
	m_circleSprite2->setVisible(false);
	m_circleSprite2->setScaleX(1.5f / this->getScaleX());
	m_circleSprite2->setScaleY(1.5f / this->getScaleY());
	m_circleSprite2->saveCurrentProperties();
	this->addChild(m_circleSprite2, 901);

	// 花纹
	m_patternSprite = WJSprite::create("game/03_minigame/p008/pattern/001.png");
	m_patternSprite->getTexture()->setAntiAliasTexParameters();
	m_patternSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	m_patternSprite->setScale(1.2f);
	m_patternSprite->setPosition(m_sprite->getPosition());
	m_patternSprite->setVisible(false);
	m_patternSprite->loadSpriteTexture(BASE_NONE_PNG);
	BlendFunc blend = { GL_DST_ALPHA, GL_ZERO };
	m_patternSprite->setBlendFunc(blend);
	this->addChild(m_patternSprite, 150);

	// 用来做渲染的花纹
	m_patternSprite2 = (WJSprite*)m_patternSprite->clone(true);
	BlendFunc blend2 = { GL_ZERO, GL_DST_ALPHA };
	m_patternSprite2->setBlendFunc(blend2);
	m_patternSprite2->setVisible(false);
	this->addChild(m_patternSprite2, 150);
	// 花纹图片
	m_patternFile = m_patternSprite->getSpriteFileName();
}

void NailLayer::startplayCircleAni(float delay)
{
	if (m_isDrawed)
		return;

	float time = 0.9f;
	this->runAction(RepeatForever::create(
		Sequence::create(
			DelayTime::create(delay),
			CCCallFunc::create([this]() {
				runCircleScaleAction(m_circleSprite1);
			}),
			DelayTime::create(time),
			CCCallFunc::create([this]() {
				runCircleScaleAction(m_circleSprite2);
			}),
				DelayTime::create(time),
			CCCallFunc::create([this]() {
				runCircleScaleAction(m_circleSprite1);
			}),
				DelayTime::create(time),
			CCCallFunc::create([this]() {
				runCircleScaleAction(m_circleSprite2);
			}),
				DelayTime::create(time),
			CCCallFunc::create([this]() {
				runCircleScaleAction(m_circleSprite1);
			}),
				DelayTime::create(time),
			CCCallFunc::create([this]() {
				runCircleScaleAction(m_circleSprite2);
			}),
			DelayTime::create(3.0f),
			nullptr)))->setTag(PLAY_NAIL_CIRCLE_ANI_TAG);
}

void NailLayer::stopplayCircleAni()
{
	this->stopActionByTag(PLAY_NAIL_CIRCLE_ANI_TAG);
	m_circleSprite1->stopAllActions();
	m_circleSprite2->stopAllActions();
	m_circleSprite1->setVisible(false);
	m_circleSprite2->setVisible(false);
}

void NailLayer::runCircleScaleAction(WJSprite *sprite)
{
	sprite->stopAllActions();
	sprite->setOpacity(0);
	sprite->setVisible(true);
	sprite->restoreSavedScale();
	sprite->runAction(Sequence::create(
		DelayTime::create(0.1f),
		ScaleTo::create(1.2f, 0.01f * sprite->getSavedScale()),
		nullptr));
	sprite->runAction(Sequence::create(
		DelayTime::create(0.1f),
		FadeTo::create(0.3f, 0.7f * sprite->getSavedOpacity()),
		FadeTo::create(0.9f, 0.1f * sprite->getSavedOpacity()),
		nullptr));
}

void NailLayer::initCanvasLayer()
{
	const Size &size = m_sprite->getContentSize();

	if (m_coverSprite)
	{
		m_coverSprite->removeFromParentAndCleanup(true);
		m_coverSprite = nullptr;
	}
	//画指甲油用
	m_coverSprite = dynamic_cast<WJSprite*>(m_sprite->clone());
	m_coverSprite->setVisible(false);
	m_coverSprite->saveCurrentProperties();
	this->addChild(m_coverSprite, 10);

	
	if (m_canvas)
	{
		m_canvas->removeFromParentAndCleanup(true);
		m_canvas = nullptr;
	}
	// 画布
	m_canvas = WJCanvas::create();
	m_canvas->setTouchSwallowsTouches(false);
	m_canvas->setCanvasSize(size.width, size.height);
	m_canvas->ignoreAnchorPointForPosition(false);
	m_canvas->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_canvas->setPosition(size / 2);
	
	BlendFunc blend = { GL_DST_ALPHA, GL_ZERO };
	m_canvas->getCanvasSprite()->setBlendFunc(blend);
	m_canvas->getCanvasSprite()->getTexture()->setAntiAliasTexParameters();
	m_canvas->setVisible(false);
	m_canvas->setOnTouchDrawBegan(CC_CALLBACK_2(NailLayer::onTouchNailBegan, this));
	m_canvas->setOnTouchDrawing(CC_CALLBACK_2(NailLayer::onTouchNailDrawing, this));
	m_canvas->setOnTouchDrawEnd(CC_CALLBACK_2(NailLayer::onTouchNailDrawEnd, this));
	m_canvas->setTouchDrawEnabled(false);
	m_canvas->setTouchDrawOutside(false);

	// 画笔
	WJPenHue *pen = WJPenHue::create("game/03_minigame/p008/nailcolor/001.png");
	m_canvas->setPen(pen);
	pen->setPenIndex(0);
	pen->setPenSize(m_penSize);
	pen->setPenOpacity(155);
	pen->setHueChangeAble(false);
	this->addChild(m_canvas, 100);
	m_canvas->saveCurrentProperties();

	// 指甲油
	if (m_renderTexture)
	{
		m_renderTexture->removeFromParentAndCleanup(true);
		m_renderTexture = nullptr;
	}
	m_renderTexture = RenderTexture::create(size.width, size.height);
	m_renderTexture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_renderTexture->ignoreAnchorPointForPosition(false);
	m_renderTexture->setPosition(size / 2);
	m_renderTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
	this->addChild(m_renderTexture, 101);

	// 花纹
	if (m_patternTexture)
	{
		m_patternTexture->removeFromParentAndCleanup(true);
		m_patternTexture = nullptr;
	}
	m_patternTexture = RenderTexture::create(size.width, size.height);
	m_patternTexture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_patternTexture->ignoreAnchorPointForPosition(false);
	m_patternTexture->setPosition(size / 2);
	m_patternTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
	this->addChild(m_patternTexture, 150);
}

NailLayer::NailLayer()
{
	m_sprite = nullptr;
	m_nailNo = -1;
	m_nailStyleNo = 1;
	m_state = NailState::NormalState;
	m_clickNormalNailCallBack = nullptr;
	m_canvas = nullptr;
	m_coverSprite = nullptr;
	m_canvas = nullptr;
	m_renderTexture = nullptr;
	m_patternTexture = nullptr;
	m_circleSprite1 = nullptr;
	m_circleSprite2 = nullptr;
	m_isDrawed = false;
	m_patternSprite = nullptr;
	m_maxdecorZorder = 0;
	m_nailImage = nullptr;
	m_patternFile = "";
	m_penSize = 0.5f;
}

NailLayer::~NailLayer()
{
	CC_SAFE_RELEASE(m_nailImage);
	CC_SAFE_DELETE(m_nailImage);
}

void NailLayer::onEnterTransitionDidFinish()
{
	WJLayer::onEnterTransitionDidFinish();
	startplayCircleAni(1.5f);
}

void NailLayer::changeNewStyleNail(int styleNo)
{
	if (m_nailStyleNo == styleNo)
		return;

	m_nailStyleNo = styleNo;
	ScrollItemTemplate object = ScrollItemTemplateTem(WJUtils::stringAddInt("P008_style_", styleNo, 3).c_str());
	m_sprite->setScale(1.0f);
	if (styleNo == 1 || styleNo == 5)
	{
		m_sprite->setScaleY(1.35f);
	}
	m_sprite->loadSpriteTexture(object.itemFile.c_str());
	initCanvasLayer();
	initImage();
}

void NailLayer::initImage()
{
	CC_SAFE_RELEASE(m_nailImage);
	CC_SAFE_DELETE(m_nailImage);

	// 分割网格相关
	const Size &size = m_sprite->getContentSize();
	RenderTexture *checkTexture = RenderTexture::create(size.width, size.height);
	checkTexture->ignoreAnchorPointForPosition(false);
	checkTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
	checkTexture->setVisible(false);
	checkTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
	this->addChild(checkTexture, 100);

	Point pointSave = m_sprite->getPosition();
	Point point = Vec2(size.width / 2, 0);

	checkTexture->beginWithClear(0, 0, 0, 0);
	m_sprite->setPosition(point);
	m_sprite->Node::visit();
	checkTexture->end();
	Director::getInstance()->getRenderer()->render();

	m_sprite->setPosition(pointSave);

	m_nailImage = checkTexture->newImage();
	CC_SAFE_RETAIN(m_nailImage);

}

void NailLayer::onClickNailSprite(Node *node, WJTouchEvent *event)
{
	if (m_state == NailState::NormalState)
		doEventClickNormalNailCallBack(this, event);
}

void NailLayer::setClickNormalNailCallBack(const WJTouchCallback &callback)
{
	m_clickNormalNailCallBack = callback;
}

void NailLayer::doEventClickNormalNailCallBack(Node *node, WJTouchEvent *e)
{
	// 对选择的指甲进行美化
	stopplayCircleAni();
	CALL_DOEVENT(m_clickNormalNailCallBack);
	m_state = NailState::drawingColorState;
	m_isDrawed = true;
	NotificationCenter::getInstance()->postNotification("showSingleNail", __Integer::create(m_nailNo));
}

void NailLayer::onEnter()
{
	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(NailLayer::onShowSingleNailNotification),
		"showSingleNail", NULL);

	WJLayer::onEnter();
}

void NailLayer::onExit()
{
	NotificationCenter::getInstance()->removeObserver(this, "showSingleNail");
	WJLayer::onExit();
}

void NailLayer::onShowSingleNailNotification(Ref *ref)
{
	stopplayCircleAni();
	__Integer *integer = (__Integer*)ref;
	if (integer->getValue() != m_nailNo)
		m_state = NailState::waitState;
}

void NailLayer::changeNailState(NailState state)
{
	m_state = state;
}

void NailLayer::onTouchNailBegan(WJCanvas* canvas, Touch* touch)
{

}

void NailLayer::onTouchNailDrawing(WJCanvas* canvas, Touch* touch)
{

}

void NailLayer::onTouchNailDrawEnd(WJCanvas* canvas, Touch* touch)
{

}

WJCanvas* NailLayer::getNailCanvas()
{
	return m_canvas;
}

void NailLayer::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
	WJLayer::visit(renderer, parentTransform, parentFlags);
}

void NailLayer::update(float delta)
{
	WJLayer::update(delta);

	// 渲染颜色
	if (m_state == NailState::drawingColorState && m_canvas->isTouchDrawing())
	{
		visitColor();
	}
	// 渲染花纹
	else if (m_state == NailState::drawingPatternState)
	{
		visitPattern();
	}
}

void NailLayer::visitColor()
{
	auto renderer = Director::getInstance()->getRenderer();
	auto &parentTransform = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

	const Size &size = m_sprite->getContentSize();
	const Vec2 &nailPos = m_coverSprite->getSavedPosition();
	const Vec2 &canvasPos = m_canvas->getSavedPosition();

	m_canvas->setVisible(true);
	m_coverSprite->setVisible(true);
	m_coverSprite->setPosition(Vec2(size.width / 2, 0));
	m_canvas->setPosition(Vec2(size.width / 2, size.height / 2));

	m_renderTexture->beginWithClear(0, 0, 0, 0);
	m_coverSprite->visit(renderer, parentTransform, true);
	m_canvas->visit(renderer, parentTransform, true);
	m_renderTexture->end();

	m_coverSprite->setPosition(nailPos);
	m_coverSprite->setVisible(false);
	m_canvas->setVisible(false);
	m_canvas->setPosition(canvasPos);

	Director::getInstance()->getRenderer()->render();
}

void NailLayer::visitPattern()
{
	// 渲染花纹
	if (WJUtils::equals(BASE_NONE_PNG, m_patternFile.c_str()))
	{
		m_patternTexture->clear(0, 0, 0, 0);
	}
	else
	{
		auto renderer = Director::getInstance()->getRenderer();
		auto &parentTransform = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
		const Size &size = m_sprite->getContentSize();
		const Vec2 &nailPos = m_coverSprite->getSavedPosition();

		m_coverSprite->setVisible(true);
		m_patternSprite->setVisible(true);
		m_patternSprite2->setVisible(true);
		m_coverSprite->setPosition(Vec2(size.width / 2, 0));

		m_patternTexture->beginWithClear(0, 0, 0, 0);
		m_coverSprite->visit(renderer, parentTransform, true);
		m_patternSprite->visit(renderer, parentTransform, true);
		m_patternSprite2->visit(renderer, parentTransform, true);
		m_patternTexture->end();

		m_coverSprite->setPosition(nailPos);
		m_coverSprite->setVisible(false);
		m_patternSprite->setVisible(false);
		m_patternSprite2->setVisible(false);

		Director::getInstance()->getRenderer()->render();
	}
}

NailState NailLayer::getNailState()
{
	return m_state;
}

void NailLayer::visitPatternTexture(const char * fileName, bool enforce)
{
	// 替换花纹
	if (enforce || m_state == NailState::drawingPatternState)
	{
		// 新的花纹
		m_patternSprite->loadSpriteTexture(fileName);
		BlendFunc blend = { GL_DST_ALPHA, GL_ZERO };
		m_patternSprite->setVisible(false);
		m_patternSprite->setBlendFunc(blend);

		m_patternSprite2->loadSpriteTexture(fileName);
		BlendFunc blend2 = { GL_ZERO, GL_DST_ALPHA };
		m_patternSprite2->setBlendFunc(blend2);
		m_patternSprite2->setVisible(false);

		// 花纹图片
		m_patternFile = m_patternSprite->getSpriteFileName();
	}
}

WJSprite * NailLayer::getNailSprite()
{
	return m_sprite;
}

void NailLayer::onClickScrollDecorItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	const Size &size = m_sprite->getContentSize();
	const Vec2 &point = m_sprite->getPosition();

	// 计算坐标
	float offset = 35.0f;
	float minx = point.x - size.width / 2 + offset;
	float maxX = point.x + size.width / 2 - offset;
	float miny = point.y + offset;
	float maxy = point.y + size.height - offset;

	Vec2 toPoint;
	do 
	{
		float x = minx + WJUtils::randomInt((int)(maxX - minx));
		float y = miny + WJUtils::randomInt((int)(maxy - miny));
		toPoint = Vec2(x, y);

		if (!PUtils::getPointImageIsTransparent(m_nailImage, m_sprite->convertToNodeSpace(
			this->convertToWorldSpace(toPoint))))
			break;

	} while (1);

	// 生成一个新的装饰品
	WJSprite *newSprite = WJSprite::create(item->getItemFilePath().c_str());
	newSprite->setMoveAble(false);
	newSprite->setPosition(toPoint);
	newSprite->setRotation(0.0f);
	newSprite->setScaleX(newSprite->getScaleInNode(m_decorLayer).x);
	newSprite->setScaleY(newSprite->getScaleInNode(m_decorLayer).y);
	newSprite->saveCurrentProperties();
	m_decorLayer->addChild(newSprite, m_maxdecorZorder++);
	initDecorSpriteEvent(newSprite);
}

bool NailLayer::onScrollDecorMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	Rect rect = WJUtils::calculateAABBInWorld(m_sprite);
	Vec2& point = item->getMovingSprite()->getPositionWorld();
	return rect.containsPoint(point) && !PUtils::getPointImageIsTransparent(m_nailImage, m_sprite->convertToNodeSpace(point));
}

void NailLayer::onScrollDecorCollisonEndedEvent(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	WJSprite *sprite = item->getMovingSprite();
	WJSprite* newSprite = static_cast<WJSprite*>(sprite->getUserObject());

	newSprite->followNode(nullptr);
	sprite->removeFromParentAndCleanup(true);
	sprite = NULL;

	const Vec2 &point = newSprite->getPositionInNode(this);
	newSprite->retain();
	newSprite->removeFromParentAndCleanup(true);
	newSprite->setPosition(point);
	newSprite->setRotation(0.0f);
	newSprite->setScaleX(newSprite->getScaleInNode(m_decorLayer).x);
	newSprite->setScaleY(newSprite->getScaleInNode(m_decorLayer).y);
	m_decorLayer->addChild(newSprite, m_maxdecorZorder++);
	// 已经放在脸上的装饰品
	initDecorSpriteEvent(newSprite);
	newSprite->release();
}

void NailLayer::initDecorSpriteEvent(WJSprite *sprite)
{
	sprite->setMoveAble(true);
	sprite->setTouchSwallowsTouches(true);
	sprite->noClickMoveEffect();
	sprite->setOnTouchAble(CC_CALLBACK_2(NailLayer::onDecorSpriteTouchAble, this));
	sprite->setOnTouchEnded(CC_CALLBACK_2(NailLayer::onDecorSpriteTouchEnded, this));

	_scrollLayer->setStackIndex(this->getNailNo() - 1);
	_scrollLayer->addNodeIntoStack(sprite);

	// 评星
	((HandLayer*)this->getParent()->getParent())->checkBothNailDecorNum();
}

bool NailLayer::onDecorSpriteTouchAble(Node *node, WJTouchEvent *event)
{
	// 装饰layer不显示的情况，指甲上面的装饰物不能移动
	if (!_scrollLayer->getScrollIsShowed())
		return false;

	node->setLocalZOrder(20000);
	return true;
}

void NailLayer::onDecorSpriteTouchEnded(Node *node, WJTouchEvent *event)
{
	WJSprite *sprite = (WJSprite*)node;
	// 当前的装饰物不在碰撞框里面
	Rect rect = WJUtils::calculateAABBInWorld(m_sprite);
	Vec2& pointWorld = sprite->getPositionWorld();

	// 不在区域里面或者所在的点是透明的
	if (!rect.containsPoint(pointWorld) || 
		PUtils::getPointImageIsTransparent(m_nailImage, sprite->getPositionInNode(m_sprite)))
	{
		// 装饰物掉落
		Vec2 &topoint = this->convertToNodeSpace(Vec2(0, -200));
		sprite->setMoveAble(false);
		sprite->stopAllActions();
		sprite->runAction(Sequence::create(
			Spawn::create(
				EaseBackOut::create(MoveTo::create(1.0f, Vec2(sprite->getPositionX(), topoint.y))),
				FadeOut::create(1.0f),
			nullptr),
			DelayTime::create(0.3f),
			RemoveSelf::create(),
			NULL));
		// 装饰物移除出堆栈
		_scrollLayer->setStackIndex(this->getNailNo() - 1);
		_scrollLayer->deleteNodeFromInStack(sprite);
		return;
	}
	sprite->setLocalZOrder(m_maxdecorZorder++);
}

void NailLayer::cloneDecorToNode(NailLayer *toLayer)
{
	// 先清空当前指甲的所有装饰品
	toLayer->cleanDecorLayerStack();

	// 克隆当前的装饰品到其他的指甲上面去
	if (m_decorLayer->getChildrenCount() <= 0)
		return;
	
	// 遍历装饰layer
	const Size &size = this->getContentSize();
	const Size &toSize = toLayer->getContentSize();
	for each (WJSprite* sprite in m_decorLayer->getChildren())
	{
		float toPointX = (sprite->getPositionX() / size.width) * toSize.width;
		float toPointY = (sprite->getPositionY() / size.height) * toSize.height;
		// 当前待添加的装饰物的坐标是否在指甲上面、防止悬空在空白的地方
		if (!PUtils::getPointImageIsTransparent(toLayer->getNailImage(), Vec2(toPointX, toPointY)))
		{
			WJSprite *cloneSprite = (WJSprite*)sprite->clone(false);
			cloneSprite->setPosition(Vec2(toPointX, toPointY));
			toLayer->addDecorSpriteIntoLayer(cloneSprite);
		}
	}
}

void NailLayer::cleanDecorLayerStack()
{
	_scrollLayer->setStackIndex(this->getNailNo() - 1);
	_scrollLayer->cleanWholeNodeFromStack();
	m_maxdecorZorder = 0;
}

void NailLayer::addDecorSpriteIntoLayer(WJSprite *sprite)
{
	m_decorLayer->addChild(sprite, m_maxdecorZorder++);
	initDecorSpriteEvent(sprite);
}
