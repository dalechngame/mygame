#include "LineLayer.h"


LineLayer::LineLayer()
{
	m_isDrawedAble = false;
	m_star1 = nullptr;
	m_star2 = nullptr;
	m_isCompleted = false;
	m_nextLine = nullptr;
	m_lineBg = nullptr;
}

LineLayer::~LineLayer()
{

}

LineLayer* LineLayer::create(WJSprite *star1, WJSprite *star2)
{
	LineLayer* layer = new (std::nothrow) LineLayer();
	if (layer && layer->init(star1, star2))
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}

bool LineLayer::init(WJSprite *star1, WJSprite *star2)
{
	if (!WJLayer::init())
		return false;

	m_star1 = star1;
	m_star2 = star2;
	const Vec2 &point1 = star1->getPosition();
	const Vec2 &point2 = star2->getPosition();

	float radian = WJUtils::twoPointRadianNormal(&point1, &point2);
	float degree = WJUtils::radianToDegree(radian);

	// 两点之间的距离
	float distance = point1.getDistance(point2);
	// layer 的宽度和高度
	float width = 200;
	float height = distance + 100;

	// event
	this->setRotation(270 - degree);
	this->setContentSize(Size(width, height));
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->ignoreAnchorPointForPosition(false);
	this->setPosition(Vec2(point1.x + (point2.x - point1.x) / 2, point1.y + (point2.y - point1.y) / 2));
	this->setMoveAble(true);
	this->noClickMoveEffect();
	this->setCustomizeTouchRect(Rect(0, 0, width, height), true);
	this->setOnTouchAble(CC_CALLBACK_2(LineLayer::onLineLayerTouchAble, this));
	this->setOnWillMoveTo(CC_CALLBACK_2(LineLayer::onLineLayerWillMoveTo, this));
	this->setOnTouchEnded(CC_CALLBACK_2(LineLayer::onLineLayerTouchEnded, this));

	// 绘制开始点从层的底端的中间开始
	m_startpoint = Vec2(width / 2, this->convertToNodeSpace(m_star1->getPosition()).y);
	m_endpoint = Vec2(width / 2, this->convertToNodeSpace(m_star2->getPosition()).y);

	// line
	m_lineSprite = WJSprite::create("game/03_minigame/p009/line003.png");
	m_lineSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	m_lineSprite->ignoreAnchorPointForPosition(false);
	m_lineSprite->setPosition(m_startpoint);
	m_lineSprite->setScaleY(0.0f);
	m_lineSprite->setScaleX(1.0f);
	this->addChild(m_lineSprite, 100);

	// 线段的背景
	m_lineBg = WJSprite::create("game/03_minigame/p009/line004.png");
	m_lineBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	m_lineBg->ignoreAnchorPointForPosition(false);
	m_lineBg->setPosition(m_startpoint);
	m_lineBg->setVisible(false);
	m_lineBg->setScaleY(distance / m_lineBg->getContentSize().height);
	this->addChild(m_lineBg, 10);

	// line的头部
	m_lineHeadSprite = WJSprite::create("game/03_minigame/p009/starfull.png");
	m_lineHeadSprite->setScale(0.5f);
	m_lineHeadSprite->setVisible(false);
	m_lineHeadSprite->setPosition(m_startpoint);
	this->addChild(m_lineHeadSprite, 101);

	// 结束后线段的Y缩放比
	m_endScaleY = float(distance / m_lineSprite->getContentSize().height);

	// 点击线段的结束星星
	m_star2->setClickAble(true);
	m_star2->setTouchSwallowsTouches(false);
	m_star2->noClickMoveEffect();
	m_star2->setOnClick(CC_CALLBACK_2(LineLayer::onClickEndStar, this));

	//for test
	//LayerColor *colorLayerHead = LayerColor::create(Color4B(0, 255, 255, 80));
	//colorLayerHead->setContentSize(Size(width, height));
	//this->addChild(colorLayerHead);
	
	scheduleUpdate();

	return true;
}

void LineLayer::onClickEndStar(Node *node, WJTouchEvent *event)
{
	if (!m_isDrawedAble || m_isCompleted)
		return;

	WJSprite *sprite = (WJSprite*)node;
	// 点击星星直接完成
	sprite->setEnabled(false);
	m_isCompleted = true;
	m_lineSprite->stopAllActions();
	m_lineSprite->runAction(Sequence::create(
		ScaleTo::create(0.35f, 1.0f, m_endScaleY),
		CCCallFunc::create(CC_CALLBACK_0(LineLayer::onLineMoveEndPoint, this)),
		nullptr));
}

void LineLayer::onEnterTransitionDidFinish()
{
	WJLayer::onEnterTransitionDidFinish();
}

void LineLayer::onEnter()
{
	WJLayer::onEnter();
}

void LineLayer::onExit()
{
	WJLayer::onExit();
}

bool LineLayer::onLineLayerTouchAble(Node *node, WJTouchEvent *event)
{
	if (!m_isDrawedAble)
		return false;

	float scaleY = getLineScaleYFromTouch(node, event);
	runActionLineScaleY(scaleY, 0.8f);

	return true;
}

bool LineLayer::onLineLayerWillMoveTo(Node *node, WJTouchEvent *event)
{
	if (m_isCompleted)
		return false;

	float scaleY = getLineScaleYFromTouch(node, event);

	if (event->lastMoveHeightInWorld <= 50 && scaleY >= 0)
	{
		m_lineSprite->stopAllActions();
		m_lineSprite->setScaleY(scaleY);
	}
	else
	{
		runActionLineScaleY(scaleY, 0.5f);
	}

	return false;
}

float LineLayer::getLineScaleYFromTouch(Node *node, WJTouchEvent *event)
{
	const Vec2 &point = this->convertTouchToNodeSpace(event->touch);
	float diatance = point.y > this->getContentSize().height ? this->getContentSize().height : point.y;
	float scaleY = (float)(diatance / m_lineSprite->getContentSize().height);

	return scaleY;
}

void LineLayer::runActionLineScaleY(float toScaleY, float wholeTime)
{
	float nowScaleY = m_lineSprite->getScaleY();
	float time = fabs(nowScaleY - toScaleY) * wholeTime;

	if (toScaleY < 0 || m_isCompleted)
		return;

	m_lineSprite->stopAllActions();
	m_lineSprite->runAction(ScaleTo::create(time, 1.0f, toScaleY));
}

void LineLayer::onLineLayerTouchEnded(Node *node, WJTouchEvent *event)
{
	runActionLineScaleY(0.0f, 0.8f);
}

void LineLayer::update(float delta)
{
	WJLayer::update(delta);

	float scaleY = m_lineSprite->getScaleY();
	m_lineHeadSprite->setVisible(false);
	if (scaleY >= m_endScaleY)
		m_lineSprite->setScaleY(m_endScaleY);
	if (scaleY >= 0.05f && scaleY < m_endScaleY)
	{
		m_lineHeadSprite->setVisible(!m_isCompleted);
		float y = m_lineSprite->getContentSize().height * scaleY;
		m_lineHeadSprite->setPositionY(y + m_startpoint.y);
		// 完成了连线的90%就算完成
		if (m_lineHeadSprite->getPositionY() >= m_endpoint.y * 0.9f && !m_isCompleted)
		{
			m_isCompleted = true;
			m_lineSprite->stopAllActions();
			m_lineSprite->runAction(Sequence::create(
				ScaleTo::create(0.05f, 1.0f, m_endScaleY),
				CCCallFunc::create(CC_CALLBACK_0(LineLayer::onLineMoveEndPoint, this)),
			nullptr));
		}
	}
}

void LineLayer::onLineMoveEndPoint()
{
	m_star2->loadSpriteTexture("game/03_minigame/p009/starfull.png");
	if (m_nextLine)
	{
		WJUtils::playParticle("particles/yellowstar.plist", m_star2);
		m_nextLine->getStar2()->setVisible(true);
		m_nextLine->getLineBg()->setVisible(true);
		m_nextLine->setIsDrawedAble(true);
	}
}
