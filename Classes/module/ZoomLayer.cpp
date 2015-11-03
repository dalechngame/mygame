// Created by wangwenjie on 2013/04

#include "ZoomLayer.h"
#include "Common.h"

#define LAYER_MOVE_BACK_LENGH       0
#define LAYER_MOVE_BORDER_LENGH     50

#define LAYER_RUN_WAVE_OFFSET       18      // ��ˮ�ƶ����Ĳ�ֵ

bool ZoomLayer::doEventTouchAble(Node* node, WJTouchEvent* touchEvent)
{
	m_moveOutSide = false;
	// ��㴥������
	if (m_isMulitiTouching)
		return false;


	Point point = this->convertTouchToNodeSpace(touchEvent->touch);
	ParticleSystemQuad *particle = ParticleSystemQuad::create("particles/tapsea.plist");
	particle->setPosition(point);
	particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	this->addChild(particle, 1000);

	return true;
}

bool ZoomLayer::doEventWillMoveTo(Node* node, WJTouchEvent* touchEvent)
{
    if (m_isMulitiTouching)
        return false;
    
	Size winSize = Director::getInstance()->getWinSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	// ����ǰ�ƶ��ĵ������ڿ����ƶ��ķ�Χ��
	Point toPoint = Vec2(touchEvent->x, touchEvent->y);
	
	// �жϵ�ǰ���ƶ��Ƿ��ƶ�������Ļ������
	m_moveOutSide = !checkLayerInView(toPoint, this->getScale());
	if (!m_moveOutSide)
		node->setPosition(toPoint);

	return false;
}

void ZoomLayer::doEventTouchEnded(Node* node, WJTouchEvent* touchEvent)
{
	WJLayer *layer = (WJLayer*)node;
	if (m_isMulitiTouching || layer->isMaybeClick())
		return;

	Size winSize = Director::getInstance()->getWinSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	Point point = node->getPosition();
	Point endpoint = point;

	// ��ǰ���ƶ��ƶ�������Ļ���桢������layer�ƶ�����Ļ�ı߽�Ȼ��ص�
	if (m_moveOutSide)
	{
        
        //return;
        
		float moveWidth = touchEvent->widthInWorld;
		float moveHeight = touchEvent->heightInWorld;
		endpoint = endpoint + Vec2(moveWidth, moveHeight);
		// �ص�������
		Point backPoint = endpoint;

		// ��ǰ��Ҫ�ƶ��ĵ㳬��������ֵ
		if (endpoint.x > getMaxXFromScale())
		{
			endpoint.x = getMaxXFromScale() - LAYER_MOVE_BORDER_LENGH;
			backPoint.x = getMaxXFromScale() - LAYER_MOVE_BACK_LENGH * TO_SCALE_HEIGHT;
		}
		else if (endpoint.x < getMinXFromScale())
		{
			endpoint.x = getMinXFromScale() + LAYER_MOVE_BORDER_LENGH;
			backPoint.x = getMinXFromScale() + LAYER_MOVE_BACK_LENGH * TO_SCALE_HEIGHT;
		}
		if (endpoint.y > getMaxYFromScale())
		{
			endpoint.y = getMaxYFromScale() - LAYER_MOVE_BORDER_LENGH;
			backPoint.y = getMaxYFromScale() - LAYER_MOVE_BACK_LENGH * TO_SCALE_HEIGHT;
		}
		else if (endpoint.y < getMinYFromScale())
		{
			endpoint.y = getMinYFromScale() + LAYER_MOVE_BORDER_LENGH;
			backPoint.y = getMinYFromScale() + LAYER_MOVE_BACK_LENGH * TO_SCALE_HEIGHT;
		}

		if (checkLayerInView(endpoint, this->getScale()) && checkLayerInView(backPoint, this->getScale()))
		{
			node->stopAllActions();
			node->runAction(CCSequence::create(
				//CCEaseSineOut::create(CCMoveTo::create(0.5f, endpoint)),
				CCEaseSineOut::create(CCMoveTo::create(0.5f, backPoint)),
				NULL));
		}
	}

	else
	{
        
		float wh = touchEvent->widthInWorld;
		float wy = touchEvent->heightInWorld;
		long long time = touchEvent->time;
			float x = node->getPosition().x;
			float y = node->getPosition().y;
			if ((time < 300 && (wh > 30  || wh < -30) && time != 0) || 
				(time < 300 && (wy > 30 || wy < -30) && time != 0))
			{
				x += ((float)(200 / time) * wh);
				y += ((float) (200 / time) * wy);
			}
			else
			{
				return;
			}

			// �߽�ֵ���ж�
			if (x > getMaxXFromScale()) x = getMaxXFromScale() - LAYER_MOVE_BORDER_LENGH;
			else if (x < getMinXFromScale()) x = getMinXFromScale() + LAYER_MOVE_BORDER_LENGH;
			if (y > getMaxYFromScale()) y= getMaxYFromScale() - LAYER_MOVE_BORDER_LENGH;
			else if (y < getMinYFromScale()) y = getMinYFromScale() + LAYER_MOVE_BORDER_LENGH;

			if (checkLayerInView(Vec2(x, y), this->getScale()))
			{
				node->stopAllActions();
				node->runAction(CCSequence::create(
					CCEaseSineOut::create(CCMoveTo::create(0.5f, Vec2(x, y))),
					NULL));
			}
	}
}


void ZoomLayer::touchesBegan(const std::vector<Touch*>& pTouches, Event *pEvent)
{
	if (!m_isMoving || (!m_zoomAble && !m_rotationAble))
		return;

	// second touch point
	for (Touch* pTouch : pTouches)
	{
		if (pTouch && pTouch->getID() != m_primaryTouchId)
		{
			// found
			m_isMulitiTouching = true;
			m_isMulitiTouched = true;

			// is not click
			m_isClick = false;

			break;
		}
	}
}

void ZoomLayer::touchesMoved(const std::vector<Touch*>& pTouches, Event *pEvent)
{
	if (!m_isMoving || !m_isMulitiTouching || m_touchStoped)
		return;

	// find second touch point
	Vec2 secondPoint;
	bool bFound = false;
	for (Touch* pTouch : pTouches)
	{
		if (pTouch && pTouch->getID() != m_primaryTouchId)
		{
			// found
			bFound = true;
			secondPoint = pTouch->getLocation();  // Point in world
			break;
		}
	}

	if (!bFound)
		return;

	double len = sqrt(pow(secondPoint.x - m_lastTouchPointInWorld.x, 2) + pow(secondPoint.y - m_lastTouchPointInWorld.y, 2));
	if (                                                                                                                                                                                                                                                                                                                                                                                                                                            m_twoTouchPointLen != 0)
	{
		Node* node = nodeInstance();

		// scale
		if (m_zoomAble)
		{
            node->stopAllActions();
			Point pointWorld = Vec2(secondPoint.x - m_lastTouchPointInWorld.x / 2 + m_lastTouchPointInWorld.x, secondPoint.y - 
				m_lastTouchPointInWorld.y + m_lastTouchPointInWorld.y);
			Point point = node->convertToNodeSpace(pointWorld);
			Size size = node->getContentSize();
			Point anchorpoint = Vec2(point.x / size.width, point.y / size.height);
			WJUtils::setNodeAnchorPoint(node, anchorpoint);

			// ��ǰ����������֮������ĵ�������š�Ѱ�ұ߽�
			if (!scaleNodeFromZoom(len))
			{
				if (pointWorld.x > m_winSize.width / 2 &&  pointWorld.y > m_winSize.height / 2 )
					WJUtils::setNodeAnchorPoint(node, Vec2(1.0f, 1.0f));
				else if (pointWorld.x < m_winSize.width / 2 && pointWorld.y > m_winSize.height / 2)
					WJUtils::setNodeAnchorPoint(node, Vec2(0.0f, 1.0f));
				else if (pointWorld.x < m_winSize.width / 2 && pointWorld.y < m_winSize.height / 2)
					WJUtils::setNodeAnchorPoint(node, Vec2(0.0f, 0.0f));
				else 
					WJUtils::setNodeAnchorPoint(node, Vec2(1.0f, 0.0f));

				// �ٴ��Ա߽���������
				scaleNodeFromZoom(len);
			}
		}
	}
	m_twoTouchPointLen = len;
}

bool ZoomLayer::scaleNodeFromZoom(float len)
{
	Node* node = nodeInstance();

	float w = Director::getInstance()->getWinSize().width;
	float p = 1.0f + (len - m_twoTouchPointLen) / w * 2.0f;
	float scale = node->getScale() * p;
	if (scale < m_zoomMinScale) scale = m_zoomMinScale;
	if (scale > m_zoomMaxScale) scale = m_zoomMaxScale;

	// ��ǰ���ź��Ƿ�layer��Ȼ����Ļ����
	if (checkLayerInView(this->getPosition(), scale))
	{
        node->stopAllActions();
		node->setScale(scale);
		return true;
	}

	return false;
}

void ZoomLayer::touchesEnded(const std::vector<Touch*>& pTouches, Event *pEvent)
{
	WJUtils::setNodeAnchorPoint(nodeInstance(), Vec2(0.5f, 0.5f));
	m_isMulitiTouching = false;
	m_twoTouchPointLen = 0;
}

ZoomLayer* ZoomLayer::create( const char *jsonFileName)
{
	ZoomLayer *layer = new ZoomLayer();
	if (layer && layer->initFromJsonFile(jsonFileName))
	{
		layer->autorelease();
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return NULL;
}

bool ZoomLayer::initFromJsonFile( const char *jsonFileName )
{
	if (!WJLayerJson2x::initFromJsonFile(jsonFileName))
	{
		return false;
	}

	m_winSize = Director::getInstance()->getWinSize();
	m_origion = Director::getInstance()->getVisibleOrigin();
	Size size = this->getContentSize() * this->getScale();

	// init
	this->setMoveAble(true);
	this->setRelativeMove(true);
	this->setZoomAble(true);
	this->setTouchSwallowsTouches(false);
	this->noClickMoveEffect();
	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->ignoreAnchorPointForPosition(false);
	this->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));

	return true;
}

bool ZoomLayer::checkLayerInView(const Point &point, float scale)
{
	const Size &winSize = Director::getInstance()->getWinSize();
	Point origion = Director::getInstance()->getVisibleOrigin();

	Point anchorpoint = this->getAnchorPoint();
	Size size = this->getContentSize() * scale;
	
	bool result = true;
	Point leftHigh = point + Vec2(-anchorpoint.x * size.width, (1 - anchorpoint.y) * size.height);
	Point leftLow = point + Vec2(-anchorpoint.x * size.width, - anchorpoint.y * size.height);

	Point rightHigh = point + Vec2((1 - anchorpoint.x) * size.width, (1 - anchorpoint.y) * size.height);
	Point rightLow = point + Vec2((1 - anchorpoint.x) * size.width, - anchorpoint.y * size.height);

	if (leftLow.x > origion.x - LAYER_RUN_WAVE_OFFSET || leftLow.y > origion.y - LAYER_RUN_WAVE_OFFSET ||
		leftHigh.x > origion.x - LAYER_RUN_WAVE_OFFSET || leftHigh.y < winSize.height - origion.y + LAYER_RUN_WAVE_OFFSET ||
		rightLow.x < winSize.width - origion.x + LAYER_RUN_WAVE_OFFSET || rightLow.y > origion.y - LAYER_RUN_WAVE_OFFSET ||
		rightHigh.x < winSize.width - origion.x + LAYER_RUN_WAVE_OFFSET || rightHigh.y < winSize.height - origion.y + LAYER_RUN_WAVE_OFFSET)
	{
		result = false;
	}

	return result;
}

ZoomLayer::ZoomLayer()
{
	m_moveOutSide = false;
}

float ZoomLayer::getMaxXFromScale()
{
	const Size &size = this->getContentSize() * this->getScale();
	Point anchorpoint = this->getAnchorPoint();
	Point origion = Director::getInstance()->getVisibleOrigin();

	return origion.x + size.width / 2 - LAYER_RUN_WAVE_OFFSET;
}

float ZoomLayer::getMinXFromScale()
{
	const Size &size = this->getContentSize() * this->getScale();
	Point anchorpoint = this->getAnchorPoint();

	return m_winSize.width - m_origion.x - 0.5f * size.width + LAYER_RUN_WAVE_OFFSET;
}

float ZoomLayer::getMaxYFromScale()
{
	const Size &size = this->getContentSize() * this->getScale();
	Point anchorpoint = this->getAnchorPoint();

	return m_origion.y + anchorpoint.y * size.height - LAYER_RUN_WAVE_OFFSET;
}

float ZoomLayer::getMinYFromScale()
{
	const Size &size = this->getContentSize() * this->getScale();
	Point anchorpoint = this->getAnchorPoint();

	return  m_winSize.height - m_origion.y - 0.5f * size.height + LAYER_RUN_WAVE_OFFSET;
}

