
#include "WJPenHue.h"
#include "SpriteWithHue.h"

#define PI_180	3.1415926535898f
#define PI_360	(PI_180 * 2)

WJPenHue* WJPenHue::create(const char* penImageFile)
{
	WJPenHue *pRet = new WJPenHue();
	if (pRet && pRet->init(penImageFile))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool WJPenHue::init(const char* penImageFile)
{
	SpriteWithHue* sprite = SpriteWithHue::create(penImageFile);
	sprite->setHue(0.0f);
	sprite->getTexture()->setAntiAliasTexParameters();
	//sprite->setVisible(false);
	m_spriteCacheArray.pushBack(sprite);

	ignoreAnchorPointForPosition(false);
	setAnchorPoint(Vec2(0.5f, 0.5f));

	m_penSpriteFileName = penImageFile;
	m_spriteWithHue = sprite;

	return true;
}

void WJPenHue::addPenHue(GLfloat addHue)
{
	if (!WJUtils::canClick("changhue", 500) || !m_hueChangeAble)
		return;

	m_hue = m_hue + addHue;
	if (m_hue >= PI_360)
		m_hue = 0.0f;

	// 改变缓存的精灵的hue值
	for (Sprite* sprite : m_spriteCacheArray)
		((SpriteWithHue*)sprite)->setHue(m_hue);
}

void WJPenHue::setPenHue(GLfloat hue)
{
	if (hue <= 0)
		hue = 0.0f;
	if (hue >= PI_360)
		hue = 0.0f;

	m_hue = hue;
	// 改变缓存的精灵的hue值
	for (Sprite* sprite : m_spriteCacheArray)
		((SpriteWithHue*)sprite)->setHue(hue);
}

WJPenHue::~WJPenHue()
{

}

WJPenHue::WJPenHue()
{
	m_spriteWithHue = nullptr;
	m_hue = 0.0f;
	m_penIndex = 0;
	m_hueChangeAble = true;
}

void WJPenHue::_visitHueSprite(const Vec2 &position, const Vec2 &startPoint, const Vec2 &endPoint)
{
	SpriteWithHue *sprite;

	m_batchIndex++;
	if (m_batchIndex < m_spriteCacheArray.size())
	{
		sprite = (SpriteWithHue*)m_spriteCacheArray.at(m_batchIndex);
	}
	else
	{
		SpriteWithHue* sprite0 = (SpriteWithHue*)m_spriteCacheArray.at(0);

		sprite = SpriteWithHue::createWithTexture(m_spriteWithHue->getTexture(), m_hue);
		sprite->setScale(sprite0->getScale());
		sprite->setOpacity(sprite0->getOpacity());
		sprite->setColor(sprite0->getColor());
		sprite->setRotation(sprite0->getRotation());
		sprite->setBlendFunc(sprite0->getBlendFunc());
		
		m_spriteCacheArray.pushBack(sprite);
	}
	
	sprite->setPosition(position);

	if (m_randomPenColorEveryPoint)
		sprite->setColor(getPenRandomColor());
	else if (m_smoothPenColorEveryPoint)
		sprite->setColor(getNextSmoothPenColor());

	if (m_randomPenRotate)
	{
		sprite->setRotation(m_randomPenRotateDegree - (rand() % (m_randomPenRotateDegree * 2)));
	}
	else if (m_penAutoRotate)
	{
		float degree = WJUtils::radianToDegree(
			WJUtils::twoPointRadianNormal(&endPoint, &startPoint));
		sprite->setRotation(-degree);
	}

	if (m_minPenSize != m_maxPenSize)
	{
		sprite->setScale(cocos2d::random(m_minPenSize, m_maxPenSize));
	}

	sprite->Node::visit();

}

GLfloat WJPenHue::getPenHue()
{
	return m_hue;
}

void WJPenHue::drawLine(const Vec2 &pointFrom, const Vec2 &pointTo)
{
	beginDraw();

	_drawHueLine(pointFrom, pointTo);

	endDraw();
}

bool WJPenHue::_drawHueLine(const Vec2 &pointFrom, const Vec2 &pointTo)
{
	Vec2 basePoint = m_lastDrawPoint;
	if (m_isFirstDrawPoint)
	{
		basePoint = pointFrom;
		m_lastDrawPoint.x = pointFrom.x;
		m_lastDrawPoint.y = pointFrom.y;
	}

	float distance = basePoint.getDistance(pointTo);

	//LOGD_L2("_drawLine, distance:", (long)distance, (long)m_drawStep);

	if (distance < m_drawStep)
	{
		if (!m_isFirstDrawPoint)
		{
			//LOGD("distance < drawStep, return false");
			return false;
		}

		//LOGD("draw point");
		if (!m_penAutoRotate)
			_visitHueSprite(pointTo, pointTo, pointTo);

		m_lastDrawPoint.x = pointTo.x;
		m_lastDrawPoint.y = pointTo.y;
	}
	else
	{
		// draw line
		Vec2 point = Vec2(basePoint.x, basePoint.y);
		float difx = pointTo.x - basePoint.x;
		float dify = pointTo.y - basePoint.y;
		float delta;
		float i = (m_isFirstDrawPoint && !m_penAutoRotate) ? 0 : m_drawStep;
		for (; i <= distance; i += m_drawStep)
		{
			delta = i / distance;
			point.x = basePoint.x + (difx * delta);
			point.y = basePoint.y + (dify * delta);

			if (m_waitingAutoRotate)
			{
				_visitHueSprite(basePoint, basePoint, point);
				m_waitingAutoRotate = false;
			}

			_visitHueSprite(point, m_lastDrawPoint, point);

			m_lastDrawPoint.x = point.x;
			m_lastDrawPoint.y = point.y;
		}
	}
	m_isFirstDrawPoint = false;

	return true;
}

WJPen* WJPenHue::clone()
{
	WJPenHue *pen = WJPenHue::create(getPenImageFileName());
	pen->setPenSize(getPenSize());
	pen->m_minPenSize = m_minPenSize;
	pen->m_maxPenSize = m_maxPenSize;

	pen->setPenIndex(getPenIndex());
	pen->setPenColor(getPenColor());
	pen->setPenOpacity(getPenOpacity());
	pen->setRotation(getPenRotation());
	pen->setPenBlendMode(getPenBlendMode());
	pen->setDrawStep(getDrawStep());

	return pen;
}
