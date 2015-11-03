#include "PUtils.h"
#include "LBLibraryBase.h"

float PUtils::playItemBounceEffect(Node* pNode, float time)
{
	WJBase *base = WJBase::convertToWJBase(pNode);
	if (base)
	{
		float scale = base->getSavedScale();
		pNode->stopActionByTag(NODE_BOUNCE_ACTION_TAG);
		pNode->runAction(Sequence::create(ScaleTo::create(time, scale * 1.2f),
			ScaleTo::create(time, scale * 0.8f),
			ScaleTo::create(time, scale * 1.1f),
			ScaleTo::create(time, scale * 0.9f),
			ScaleTo::create(time * 0.667f, scale),
			nullptr))->setTag(NODE_BOUNCE_ACTION_TAG);
	}

	return time * 4.667f;
}

float PUtils::playItemBounceEffect(Node* pNode, int num, float time)
{
	WJBase *base = WJBase::convertToWJBase(pNode);
	if (base)
	{
		float scale = base->getSavedScale();
		pNode->stopActionByTag(NODE_BOUNCE_ACTION_TAG);
		FiniteTimeAction* act = Sequence::create(ScaleTo::create(time, scale * 1.2f),
			ScaleTo::create(time, scale * 0.8f),
			ScaleTo::create(time, scale * 1.1f),
			ScaleTo::create(time, scale * 0.9f),
			ScaleTo::create(time * 0.667f, scale),
			nullptr);
		pNode->runAction(Repeat::create(act, num))->setTag(NODE_BOUNCE_ACTION_TAG);
	}
	return num * time * 4.667f;
}

void PUtils::playBubbleEffect(Node *node)
{
	// 提示气泡的呼吸效果
	node->stopAllActions();
	Action *action = RepeatForever::create(
		Sequence::create(
		ScaleTo::create(1.0f, 0.95f, 1.0f),
		ScaleTo::create(1.0f, 1.0f, 0.95f),
		NULL));
	node->runAction(action);
}

void PUtils::playNodeWaveEffect(Node *node)
{
	// 节点的旋转效果
	WJBase *base = dynamic_cast<WJBase*>(node);
	if (base && !node->getActionByTag(NODE_RUN_WAVE_ACTION_TAG))
	{
		node->stopAllActions();
		base->restoreSavedRotation(0.2f);
		Action *action = Sequence::create(
			RotateBy::create(0.2f, 10),
			RotateBy::create(0.2f, -10),
			RotateBy::create(0.2f, -10),
			RotateBy::create(0.2f, 10),
			NULL);
		node->runAction(action)->setTag(NODE_RUN_WAVE_ACTION_TAG);
	}
}

void PUtils::playBubbleEffectWithCurrentScale(Node* node)
{
	float currentScaleX = node->getScaleX();
	float currentScaleY = node->getScaleY();

	node->stopAllActions();
	node->runAction(RepeatForever::create(Sequence::createWithTwoActions(
		ScaleTo::create(1.0f, 0.95f * currentScaleX, currentScaleY),
		ScaleTo::create(1.0f, currentScaleX, 0.95f * currentScaleY))));
}

void PUtils::playSuspenEffect(Node *node)
{
	WJBase *base = WJBase::convertToWJBase(node);
	if (base && base->getSavedPosition() != Vec2::ZERO)
		base->restoreSavedPosition(0.0f);

	node->stopAllActions();
	Action *action = RepeatForever::create(
		Sequence::create(
		MoveBy::create(1.0f, Vec2(0, 15)),
		MoveBy::create(1.0f, Vec2(0, -15)),
		NULL));
	node->runAction(action);
}

void PUtils::playSuspenEffect(Node *node, float delay)
{
	node->runAction(Sequence::create(
		DelayTime::create(delay),
		CCCallFunc::create([=](){
		PUtils::playSuspenEffect(node);
	}),
		NULL));
}

void PUtils::playNodeScaleAni(Node *node, int time, float delayTime)
{
	WJBase *base = WJBase::convertToWJBase(node);
	if (base)
	{
		node->stopActionByTag(SHOW_SCALE_ACTION_TAG);
		node->runAction(CCRepeat::create(
			Sequence::create(
			ScaleTo::create(0.15f, 1.1f * base->getSavedScale()),
			ScaleTo::create(0.15f, 1.0f * base->getSavedScale()),
			ScaleTo::create(0.15f, 1.1f * base->getSavedScale()),
			ScaleTo::create(0.15f, 1.0f * base->getSavedScale()),
			ScaleTo::create(0.15f, 1.1f * base->getSavedScale()),
			ScaleTo::create(0.15f, 1.0f * base->getSavedScale()),
			CCDelayTime::create(delayTime),
			NULL), time))->setTag(SHOW_SCALE_ACTION_TAG);
	}
}

void PUtils::playNodeScaleAniForever(Node *node, float delayTime)
{
	WJBase *base = WJBase::convertToWJBase(node);
	if (base)
	{
		node->stopActionByTag(SHOW_SCALE_ACTION_TAG);
		node->runAction(CCRepeatForever::create(
			Sequence::create(
			CCDelayTime::create(delayTime),
			ScaleTo::create(0.15f, 1.1f * base->getSavedScale()),
			ScaleTo::create(0.15f, 1.0f * base->getSavedScale()),
			ScaleTo::create(0.15f, 1.1f * base->getSavedScale()),
			ScaleTo::create(0.15f, 1.0f * base->getSavedScale()),
			ScaleTo::create(0.15f, 1.1f * base->getSavedScale()),
			ScaleTo::create(0.15f, 1.0f * base->getSavedScale()),
			NULL)))->setTag(SHOW_SCALE_ACTION_TAG);
	}
}

void PUtils::playNodeScaleAniForeverFirst(Node *node, float delayTime)
{
	WJBase *base = WJBase::convertToWJBase(node);
	if (base)
	{
		node->stopActionByTag(SHOW_SCALE_ACTION_TAG);
		node->runAction(CCRepeatForever::create(
			Sequence::create(
			ScaleTo::create(0.15f, 1.1f * base->getSavedScale()),
			ScaleTo::create(0.15f, 1.0f * base->getSavedScale()),
			ScaleTo::create(0.15f, 1.1f * base->getSavedScale()),
			ScaleTo::create(0.15f, 1.0f * base->getSavedScale()),
			ScaleTo::create(0.15f, 1.1f * base->getSavedScale()),
			ScaleTo::create(0.15f, 1.0f * base->getSavedScale()),
			CCDelayTime::create(delayTime),
			NULL)))->setTag(SHOW_SCALE_ACTION_TAG);
	}
}

void PUtils::stopNodeScaleAction(Node *node)
{
	WJBase *base = WJBase::convertToWJBase(node);
	if (base)
	{
		node->stopActionByTag(SHOW_SCALE_ACTION_TAG);
		base->restoreSavedScale();
	}
}

bool PUtils::isOneCollideWithOne(Vec2 src, Vec2 des, float raduis)
{
	return des.getDistance(src) <= raduis;
}

bool PUtils::isOneCollideWithAnotherOne(Node *src, Node *des, float raduis)
{
	return GetDistance(src, des) <= raduis;
}

bool PUtils::isOneCollideWithOne(Node *src,
	Node *des,
	bool isPointCheck,
	Vec2 srcOffset,
	Vec2 desOffset)
{
	Rect srcRect = WJUtils::calculateAABBInWorld(src);
	Rect desRect = WJUtils::calculateAABBInWorld(des);
	if (isPointCheck)
	{
		Vec2 ps = Vec2(srcRect.getMidX(), srcRect.getMidY()) + srcOffset;
		Vec2 pd = Vec2(desRect.getMidX(), desRect.getMidY()) + desOffset;
		return isOneCollideWithOne(ps, pd);
	}
	else
	{
		return desRect.intersectsRect(srcRect);
	}
}

bool PUtils::isOneCollideWithOne(Vec2 src,
	Node *des,
	bool isRectCheck,
	float halfWidth,
	float halfHeight)
{
	Rect desRect = WJUtils::calculateAABBInWorld(des);
	if (isRectCheck)
	{
		Rect rect = Rect(src.x - halfWidth, src.y - halfHeight, halfWidth * 2, halfHeight * 2);
		return desRect.intersectsRect(rect);

	}
	else
	{
		return desRect.containsPoint(src);
	}
}

bool PUtils::isOneCollideWithOne(Node *src, Node*dest, float x, float y)
{
	Rect srcRect = WJUtils::calculateAABBInWorld(src);

	Rect temp = WJUtils::calculateAABBInWorld(dest);
	Vec2 origin = temp.origin;
	float	width = temp.size.width + x;
	float	height = temp.size.height + y;
	Rect destRect(origin.x, origin.y, width, height);

	return destRect.intersectsRect(srcRect);
}

bool PUtils::isOneCollideWithOne(Vec2 src, Node*dest, float offsetX, float offsetY, float x, float y)
{
	Rect temp = WJUtils::calculateAABBInWorld(dest);
	float ox = temp.origin.x + offsetX;
	float oy = temp.origin.y + offsetY;

	float	width = temp.size.width + x;
	float	height = temp.size.height + y;

	Rect destRect(ox, oy, width, height);

	return destRect.containsPoint(src);
}

float PUtils::GetDistance(Node* _node1, Node* _node2)
{
	Vec2 _p1 = _node1->getParent()->convertToWorldSpace(_node1->getPosition());
	Vec2 _p2 = _node2->getParent()->convertToWorldSpace(_node2->getPosition());

	return _p1.getDistance(_p2);
}

bool PUtils::getPointImageIsTransparent(Image *m_textureImage, const Vec2 &pointImage)
{
	if (pointImage.x <= 0 || pointImage.x >= m_textureImage->getWidth() ||
		pointImage.y <= 0 || pointImage.y >= m_textureImage->getHeight())
		return true;

	Color4B color = WJGraphics::getImagePointColor(m_textureImage, pointImage.x,
		m_textureImage->getHeight() - pointImage.y);

	if (color.r > 10 || color.g > 10 || color.b > 10)
		return false;

	return true;
}

void PUtils::runActionMoveTo(Node *node, const Vec2 &pointWorld, bool enableMoving /*= false*/, bool stopAllAction /*= true*/)
{
	runActionMoveToWithCallBack(node, pointWorld, nullptr, enableMoving, stopAllAction);
}

void PUtils::runActionMoveToWithCallBack(Node *node, const Vec2 &pointWorld, const ActionEndCallback &callback /*= nullptr*/, bool enableMoving /*= false*/, bool stopAllAction /*= true*/)
{
	WJBase *base = dynamic_cast<WJBase*>(node);
	if (base)
	{
		base->setEnabled(enableMoving);
		Vec2 &pointNow = base->getPositionWorld();
		float distance = pointNow.getDistance(pointWorld);
		//float time = float(distance / GAME_ITEM_MOVE_SPEED);
		float time = 0.5f;

		if (stopAllAction)
			node->stopAllActions();

		node->runAction(Sequence::create(
			MoveTo::create(time, node->getParent()->convertToNodeSpace(pointWorld)),
			CCCallFunc::create([=]() {
			dynamic_cast<WJBase*>(node)->setEnabled(true);
		}),
			nullptr));

		// 移动完了的回调
		WJUtils::delayExecute(time, [=](float d) {
			if (callback)
				callback(node);
		});
	}
}

Vec2 PUtils::getPositionInWorldFromPoint(Node *node, const Vec2 &point)
{
	if (node->getParent())
	{
		return node->getParent()->convertToWorldSpace(point);
	}

	return node->getPosition();
}

void PUtils::addListener(WJBase* base, const WJTouchBoolCallback &touchAble,
	const WJTouchBoolCallback &willMoveto, const WJTouchCallback &touchEnd,
	const WJTouchCallback &click)
{
	base->noClickMoveEffect();
	base->setRelativeMove(true);
	base->setTouchSwallowsTouches(true);
	base->setOnTouchAble(touchAble);
	base->setOnWillMoveTo(willMoveto);
	base->setOnTouchEnded(touchEnd);
	base->setOnClick(click);
	base->saveCurrentProperties();
}

void PUtils::addListenerWithKey(WJLayerJson *json, const char * key, int fromint, int endInt,
	const WJTouchBoolCallback &touchAble, const WJTouchBoolCallback &willMoveto, 
	const WJTouchCallback &touchEnd, const WJTouchCallback &click)
{
	WJBase *base;
	for (int i = fromint; i <= endInt; i++)
	{
		std::string str = WJUtils::stringAddInt(key, i, 3);
		base = json->getSubBase(str.c_str());
		base->setUserTag(i);
		PUtils::addListener(base, touchAble, willMoveto, touchEnd, click);
	}
}

void PUtils::setNodeShadowVisible(WJLayerJson *json, Node *node, bool visible)
{
	// node对应的阴影的key默认为node的key + _shadow
	std::string key = json->getSubKeyByNode(node);
	key.append("_shadow");
	Node *shadow = json->getSubSprite(key.c_str());
	if (shadow)
	{
		shadow->setVisible(visible);
	}
}


void PUtils::addWJChildToAnotherParent(cocos2d::Node *child, cocos2d::Node*newParent)
{
	WJBase *base = WJBase::convertToWJBase(child);
	if (base)
	{
		base->setAutoRegisterTouchEventListener(false);
	}

	child->retain();
	child->removeFromParentAndCleanup(false);
	newParent->addChild(child);

	child->release();

	if (base)
	{
		base->setAutoRegisterTouchEventListener(true);
	}
}

//为 1 时代表准确的获取每个像素， 值越大，越不准确
#define CZ 4

float PUtils::getImgBlankspace(Image* image)
{
	unsigned char *data = image->getData();
	size_t len = image->getDataLen();
	int w = image->getWidth();
	int h = image->getHeight();

	int total_pixel = ((w + CZ - 1) / CZ) * ((h + CZ - 1) / CZ);
	int blankspace_pixel = 0;

	for (int i = 0; i < h; i += CZ)
	{
		for (int j = 0; j < w; j += CZ)
		{
			int index = i * w * 4 + j * 4 + 3;
			if (data[index] <= 20)
			{
				blankspace_pixel++;
			}
		}
	}
	return (float)blankspace_pixel / total_pixel;
}

void PUtils::playParticle(const char * fileName, Node *parent, const Vec2 &pointParent, int zOrder, int tag)
{
	// 播放粒子特效
	ParticleSystemQuad *particle = ParticleSystemQuad::create(fileName);
	particle->setPosition(pointParent);
	particle->setTag(tag);
	particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	parent->addChild(particle, zOrder);
}

Action * PUtils::delayPlayFunc(Node *node, CallFunc *call, float time)
{
	return 	node->runAction(Sequence::create(
		DelayTime::create(time),
		call,
		nullptr));
}

void PUtils::playNodeCircleTipForever(Node *node, float delayTime /*= 5.0f*/, float interval /*= 0.3f*/)
{
	if (node)
	{
		node->runAction(RepeatForever::create(Sequence::create(
			DelayTime::create(interval),
			FadeOut::create(0.3f),
			DelayTime::create(interval),
			FadeIn::create(0.3f),
			DelayTime::create(interval),
			FadeOut::create(0.3f),
			DelayTime::create(interval),
			FadeIn::create(0.3f),
			DelayTime::create(interval),
			FadeOut::create(0.3f),
			DelayTime::create(delayTime),
			FadeIn::create(0.3f),
			DelayTime::create(interval),
			FadeOut::create(0.3f),
			nullptr
			)));
	}

}
