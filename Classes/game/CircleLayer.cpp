#include "CircleLayer.h"
#include "P002Catcher.h"

CircleLayer* CircleLayer::create(WJLayerJson *layer, bool usingSke, PBase *parent)
{
	CircleLayer* deco = new (std::nothrow) CircleLayer();
	if (deco && deco->init(layer, usingSke, parent))
	{
		deco->autorelease();
		return deco;
	}

	CC_SAFE_DELETE(deco);

	return NULL;
}

bool CircleLayer::init(WJLayerJson *layer, bool usingSke, PBase *parent)
{
	if (!WJLayer::init())
	{
		return false;
	}
	m_progressState = ProgressStar::noStar;
	m_parent = parent;

	initDecoLayer(layer, usingSke);

	return true;
}

void CircleLayer::initDecoLayer(WJLayerJson *layer, bool usingSke)
{
	WJLayer *circle = nullptr;

	for (int i = 1; i <= 8; i++)
	{
		circle = layer->getSubLayer(WJUtils::stringAddInt("style_", i, 3).c_str());
		DecoLayer *deco = DecoLayer::create(circle, i - 1, usingSke);
		addChild(deco, ZORDER_PROGRESS);

		if (i != 1)
		{
			deco->seeThis(false);
		}

		m_decoVec.push_back(deco);
	}
	m_currentIndex = 0;

}

void  CircleLayer::onEnterTransitionDidFinish()
{
	WJLayer::onEnterTransitionDidFinish();
}

WJLayer * CircleLayer::getCurrentLayer()
{
	return getCurrentDeco()->getLayer();
}

DecoLayer* CircleLayer::getCurrentDeco()
{
	return m_decoVec.at(m_currentIndex);
}

Node * CircleLayer::getCurrentCircle()
{
	return getCurrentDeco()->getCircle();
}

int CircleLayer::getCurrentColor()
{
	return getCurrentDeco()->getColorIndex();
}

void CircleLayer::setCurrentColor(int currentColor)
{
	getCurrentDeco()->setColorIndex(currentColor);
}

int CircleLayer::getOriginColor()
{
	return getCurrentDeco()->getOriginColor();
}

void CircleLayer::setOriginColor(int originColor)
{
	getCurrentDeco()->setOriginColor(originColor);
}

ProgressStar  CircleLayer::getProgressState()
{
	return dynamic_cast<P002 *>(m_parent)->getProgressStarNum();
}

void CircleLayer::setProgressState(ProgressStar state)
{
	m_progressState = state;
	dynamic_cast<P002 *>(m_parent)->setProgressStarNum(state);
}

Mark* CircleLayer::isCollided(Node *destSpr)
{
	return getCurrentDeco()->isCollisioned(destSpr);
}

void CircleLayer::changeCircle(int index, Node *newSpr)
{
	if (index != m_currentIndex)
	{
		getCurrentDeco()->seeThis(false);
		m_decoVec.at(index)->seeThis(true);

		m_decoVec.at(index)->changeCircle(index, newSpr);
		setCurrentIndex(index);

	}
	else if (getCurrentColor() != getOriginColor())
	{
		setCurrentColor(m_originColor);
		getCurrentDeco()->changeCircle(index, newSpr);
	}

}

void CircleLayer::changeColor(int index, Node *newSpr)
{
	getCurrentDeco()->changeCircle(index, newSpr);

	setCurrentColor(index);
}

void CircleLayer::changeDrop(Node *newSpr, Mark*mark, int index)
{
	int enterCount = 0;
	for each (Mark* mark in getCurrentDeco()->m_drop)
	{
		if (mark->isPlaced)
		{
			enterCount++;
		}
	}
	this->runAction(Sequence::create(
		CallFunc::create([=](){
		PUtils::playParticle("particles/drop.plist", newSpr,
			newSpr->convertToNodeSpace(dynamic_cast<WJSkeletonAnimation *>(newSpr)->getPositionWorld()), ZORDER_PROGRESS);
	}),
		DelayTime::create(1.0f),
		CallFunc::create([=](){
		if (enterCount == 1)
		{
			setProgressState(ProgressStar::oneStar);
		}
		else if (enterCount == 3)
		{
			setProgressState(ProgressStar::twoStar);
			dynamic_cast<P002 *>(m_parent)->showNextButton();
		}

	}),
		nullptr));

	getCurrentDeco()->changeDrop(newSpr, mark, index);
}

void CircleLayer::startPlayCircleAni(float delay)
{
	getCurrentDeco()->startplayCircleAni(delay);
}

void CircleLayer::stopPlayCircleAni()
{
	getCurrentDeco()->stopplayCircleAni();
}

Mark* CircleLayer::getRandomMark(int index)
{
	return getCurrentDeco()->getRandomMark(index);
}

void CircleLayer::changeDeco(int index, Node *newNode)
{
	getCurrentDeco()->changeDeco(index, newNode);
}