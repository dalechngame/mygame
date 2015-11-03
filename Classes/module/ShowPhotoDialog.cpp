// Created by yuanzhongji on 2014/04

#include "showPhotoDialog.h"
#include "GameManager.h"
#include "Common.h"

#define LAYER_MOVE_BACK_LENGH       0
#define LAYER_MOVE_BORDER_LENGH     50

ShowPhotoDialog* ShowPhotoDialog::create(const char * jsonFile, int photoIndex)
{
	ShowPhotoDialog *layer = new ShowPhotoDialog();
	if (layer && layer->init(jsonFile, photoIndex))
	{
		layer->autorelease();
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return NULL;
}

bool ShowPhotoDialog::init(const char * jsonFile, int photoIndex)
{
	if (!ShowPhotoDialog::initFromJsonFile(jsonFile))
		return false;

	initPhotosFromSaved(photoIndex);
	initButtonEvent();

	return true;
}

void ShowPhotoDialog::initPhotosFromSaved(int photoIndex)
{
	ZoomPhotoLayer *layer  = ZoomPhotoLayer::create(photoIndex);
	WJSprite *oldSprite = m_layerJson->getSubSprite("photo");
	const Size &size = oldSprite->getContentScaleSize();
	const Point &point = oldSprite->getPosition();
    //const Size &layerSize = layer->getContentScaleSize();
	layer->setPosition(oldSprite->getPosition());
	layer->setLocalZOrder(oldSprite->getLocalZOrder());
	layer->setClipArea(Rect(point.x - size.width / 2 + 10, point.y - size.height / 2, size.width - 9, size.height), true);
    layer->setMoveArea(Rect(point.x - size.width / 2 - 15, point.y - size.height / 2 - 15, size.width + 30, size.height + 30));
    layer->setScale(0.75f);
	m_layerJson->replaceSubNode("photo", layer);
}

bool ShowPhotoDialog::initFromJsonFile(const char * fileName)
{
	const Size &size = Director::getInstance()->getWinSize();
	m_layerJson = WJLayerJson::create(fileName);
	m_layerJson->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_layerJson->ignoreAnchorPointForPosition(false);
	m_layerJson->setPosition(Vec2(size.width / 2, size.height / 2));

	// init dialog
	if (!LBModalDialog::init(m_layerJson, true))
	{
		CC_SAFE_DELETE(m_layerJson);
		return false;
	}

	return true;
}

void ShowPhotoDialog::initButtonEvent()
{
	// close
	WJSprite *close = m_layerJson->getSubSpriteByKey("cancle");
	close->setTag(MENU_TAG_CLOSE);
	close->setOnClick(CC_CALLBACK_2(ShowPhotoDialog::onClickClose, this));
}

void ShowPhotoDialog::onEnterTransitionDidFinish()
{
	LBModalDialog::onEnterTransitionDidFinish();
}

void ShowPhotoDialog::onClickClose( Node *node, WJTouchEvent *event )
{
	Common::sound.play("Common:close");
	this->close();
	// œ‘ æπ„∏Ê
	if (m_adsIsVisible && !WJUtils::isAdsVisible())
		Common::showAds();
}

ShowPhotoDialog::ShowPhotoDialog()
{
	m_adsIsVisible = false;
	m_layerJson = nullptr;
}

void ShowPhotoDialog::onEnter()
{
	LBModalDialog::onEnter();

	// “˛≤ÿπ„∏Ê
	m_adsIsVisible = WJUtils::isAdsVisible();
	if (m_adsIsVisible)
		WJUtils::hideAds();
}


//--------------------------------ZoomPhotoLayer-------------------------------------------------------

bool ZoomPhotoLayer::doEventTouchAble(Node* node, WJTouchEvent* touchEvent)
{
	if (!m_clipRect.containsPoint(this->getParent()->convertTouchToNodeSpace(touchEvent->touch)))
		return false;

	m_moveOutSide = false;
	// ∂‡µ„¥•√˛”≈œ»
	if (m_isMulitiTouching)
		return false;

	return true;
}

bool ZoomPhotoLayer::doEventWillMoveTo(Node* node, WJTouchEvent* touchEvent)
{
	if (m_isMulitiTouching)
		return false;

	Size winSize = Director::getInstance()->getWinSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	// Ω´µ±«∞“∆∂Øµƒµ„œﬁ÷∆‘⁄ø…“‘“∆∂Øµƒ∑∂Œßƒ⁄
	Point toPoint = Vec2(touchEvent->x, touchEvent->y);

	// ≈–∂œµ±«∞µƒ“∆∂Ø «∑Ò“∆∂ØµΩ¡À∆¡ƒªµƒÕ‚√Ê
	m_moveOutSide = !checkLayerInView(toPoint, this->getScale());
	if (!m_moveOutSide)
		node->setPosition(toPoint);

	return false;
}

void ZoomPhotoLayer::doEventTouchEnded(Node* node, WJTouchEvent* touchEvent)
{
	WJLayer *layer = (WJLayer*)node;
	if (m_isMulitiTouching || layer->isMaybeClick())
		return;

}


void ZoomPhotoLayer::touchesBegan(const std::vector<Touch*>& pTouches, Event *pEvent)
{
	if (!m_isMoving || (!m_zoomAble && !m_rotationAble))
		return;

	// second touch point
	for (Touch* pTouch : pTouches)
	{
		if (pTouch && pTouch->getID() != m_primaryTouchId)
		{
			if (!m_clipRect.containsPoint(this->getParent()->convertTouchToNodeSpace(pTouch)))
				break;

			// found
			m_isMulitiTouching = true;
			m_isMulitiTouched = true;

			// is not click
			m_isClick = false;

			break;
		}
	}
}

void ZoomPhotoLayer::touchesMoved(const std::vector<Touch*>& pTouches, Event *pEvent)
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
	if (m_twoTouchPointLen != 0)
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

			// µ±«∞≤ªƒ‹“‘¡Ωµ„÷Æº‰µƒ÷––ƒµ„Ω¯––Àı∑≈°¢—∞’“±ﬂΩÁ
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

				// ‘Ÿ¥Œ“‘±ﬂΩÁµ„Ω¯––Àı∑≈
				scaleNodeFromZoom(len);
			}
		}
	}
	m_twoTouchPointLen = len;
}

bool ZoomPhotoLayer::scaleNodeFromZoom(float len)
{
	Node* node = nodeInstance();

	float w = Director::getInstance()->getWinSize().width;
	float p = 1.0f + (len - m_twoTouchPointLen) / w * 2.0f;
	float scale = node->getScale() * p;
	if (scale < m_zoomMinScale) scale = m_zoomMinScale;
	if (scale > m_zoomMaxScale) scale = m_zoomMaxScale;

	// µ±«∞Àı∑≈∫Û «∑Òlayer»‘»ª‘⁄∆¡ƒª¿Ô√Ê
	if (checkLayerInView(this->getPosition(), scale))
	{
		node->stopAllActions();
		node->setScale(scale);
		return true;
	}

	return false;
}

void ZoomPhotoLayer::touchesEnded(const std::vector<Touch*>& pTouches, Event *pEvent)
{
	WJUtils::setNodeAnchorPoint(nodeInstance(), Vec2(0.5f, 0.5f));
	m_isMulitiTouching = false;
	m_twoTouchPointLen = 0;
}

ZoomPhotoLayer* ZoomPhotoLayer::create( int photoIndex)
{
	ZoomPhotoLayer *layer = new ZoomPhotoLayer();
	if (layer && layer->init(photoIndex))
	{
		layer->autorelease();
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return NULL;
}

bool ZoomPhotoLayer::init(int photoIndex)
{
	if (!WJLayer::init())
	{
		return false;
	}

	m_winSize = Director::getInstance()->getWinSize();
	// ’’∆¨
	std::string fileName = GManager->getShowPhotosFileFromIndex(photoIndex);
	std::string fullpath = GameManager::getAtlasSavePath() + fileName;
	m_sprite = WJSprite::create(fullpath.c_str());
	m_sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_sprite->ignoreAnchorPointForPosition(false);
	m_sprite->setVisible(true);
	this->addChild(m_sprite);
	const Size &size = m_sprite->getContentSize();
	// init
	this->setMoveAble(true);
	this->setRelativeMove(true);
	this->setZoomAble(true);
	this->setTouchSwallowsTouches(false);
	this->noClickMoveEffect();
	this->setContentSize(size);
	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->setZoomMaxScale(1.6f);
	this->setZoomMinScale(0.75f);
	this->ignoreAnchorPointForPosition(false);
	m_sprite->setPosition(Vec2(size.width / 2, size.height / 2));

	return true;
}

bool ZoomPhotoLayer::checkLayerInView(const Point &point, float scale)
{
	//Point origion = Director::getInstance()->getVisibleOrigin();

	Vec2 anchorpoint = this->getAnchorPoint();
	Size size = this->getContentSize() * scale;

	bool result = true;
	Point leftHigh = point + Vec2(-anchorpoint.x * size.width, (1 - anchorpoint.y) * size.height);
	Point leftLow = point + Vec2(-anchorpoint.x * size.width, - anchorpoint.y * size.height);

	Point rightHigh = point + Vec2((1 - anchorpoint.x) * size.width, (1 - anchorpoint.y) * size.height);
	Point rightLow = point + Vec2((1 - anchorpoint.x) * size.width, - anchorpoint.y * size.height);
	
//	if (m_clipRect.containsPoint(leftHigh) || m_clipRect.containsPoint(leftLow)
//		|| m_clipRect.containsPoint(rightHigh) || m_clipRect.containsPoint(rightLow))
//	{
//		return false;
//	}
    if (leftLow.x > m_moveRect.getMinX() || leftLow.y > m_moveRect.getMinY() ||
        leftHigh.x > m_moveRect.getMinX() || leftHigh.y < m_moveRect.getMaxY() ||
        rightLow.x < m_moveRect.getMaxX() || rightLow.y > m_moveRect.getMinY() ||
        rightHigh.x < m_moveRect.getMaxX() || rightHigh.y < m_moveRect.getMaxY())
    {
        result = false;
    }

	return result;
}

ZoomPhotoLayer::ZoomPhotoLayer()
{
	m_moveOutSide = false;
	m_sprite = nullptr;
	m_twoTouchPointLen = 0.0f;
}

void ZoomPhotoLayer::setMoveArea( const Rect &moveRect)
{
    m_moveRect = moveRect;
}