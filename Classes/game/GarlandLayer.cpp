#include "GarlandLayer.h"
#include "P006Garland.h"
#include "GestureSprite.h"

#define COLLISION_TIME 1000/*Ãë*/

GarlandLayer* GarlandLayer::create(WJLayerJson *layer, PBase *parent)
{
	GarlandLayer* deco = new (std::nothrow) GarlandLayer();
	if (deco && deco->init(layer, parent))
	{
		deco->autorelease();
		return deco;
	}

	CC_SAFE_DELETE(deco);

	return NULL;
}

bool GarlandLayer::init(WJLayerJson *layer, PBase *parent)
{
	if (!WJLayer::init())
	{
		return false;
	}
	m_progressState = ProgressStar::noStar;
	m_parent = parent;
	m_json = layer;

	initDecoLayer(layer);
	initFlower(layer);
	initArrow();

	return true;
}

void  GarlandLayer::onEnterTransitionDidFinish()
{
	WJLayer::onEnterTransitionDidFinish();

}

ProgressStar  GarlandLayer::getProgressState()
{
	return dynamic_cast<P006 *>(m_parent)->getProgressStarNum();
}

void GarlandLayer::setProgressState(ProgressStar state)
{
	m_progressState = state;
	dynamic_cast<P006*>(m_parent)->setProgressStarNum(state);
}

void GarlandLayer::storeMarkLeaf(const std::string &leafName, const char weight)
{
	MarkLeaf *mark = new MarkLeaf(leafName, m_garland->getSlotWorldPosition(leafName.c_str()), weight);

	m_leafVec.push_back(mark);
}

void GarlandLayer::playGarlandFinished()
{
	m_garland->setTimeScale(1.5f);
	m_garland->stopAllAnimation();
	m_garland->playAnimation("aniGarland", false);
	m_garland->setCompleteListener([=](int, int){

		m_garland->runAction(Sequence::create(
			DelayTime::create(0.2f),
			ScaleTo::create(0.1f, 0.01f),
			ScaleTo::create(0.2f, 1.3f),
			ScaleTo::create(0.1f, 0.6f),
			ScaleTo::create(0.2f, 1.3f),
			ScaleTo::create(0.1f, 1.0f),
			CallFunc::create([=](){

			m_scissor->stopAllActions();
			m_scissor->setStaticCutVisible(false);

			this->runAction(Sequence::create(
				CallFunc::create([=](){
				PUtils::playParticle("particles/magic003.plist", m_garland,
					m_garland->convertToNodeSpace(m_garland->getPositionWorld()), ZORDER_PROGRESS);
			}),
				DelayTime::create(1.0f),
				CallFunc::create([=](){
				setProgressState(ProgressStar::oneStar);
				dynamic_cast<P006 *>(m_parent)->showNextButton();

			}),
				nullptr));

		}),
			nullptr
			));

	});

	m_arrow->setGuestureVisible(false);
}

void GarlandLayer::onClick(Node *node, WJTouchEvent *event)
{
	PUtils::playNodeScaleAni(m_garland);
}

void GarlandLayer::initDecoLayer(WJLayerJson *layer)
{
	m_garland = layer->getSubSkeleton("ratten");

	m_garland->noClickMoveEffect();
	m_garland->setOnClick(CC_CALLBACK_2(GarlandLayer::onClick, this));
	m_garland->playAnimation("aniIdle", true);
	m_garland->update(0); // Éî¿Ó.

	// store leaf infomation
	int i = 1;
	do
	{
		std::string temp = WJUtils::stringAddInt("leaf", i, 3);
		spSlot *slot = m_garland->findSlot(temp);
		if (slot)
		{
			if (i == 1 || i == 4 || i == 6 || i == 7)
			{
				storeMarkLeaf(temp, HEIGHTER_WEIGHT);
			}
			else
			{
				storeMarkLeaf(temp);
			}

			i++;
		}
		else
		{
			i = 0;
			break;
		}
	} while (1);

	storeMarkLeaf("shuye2", HEIGHTER_WEIGHT);
	storeMarkLeaf("c3");
	storeMarkLeaf("c4");
}

void GarlandLayer::initFlower(WJLayerJson *layer)
{
	m_flower = layer->getSubLayer("flowers");
	m_flower->getChildByName("base")->setVisible(false);

	int i = 1;
	do
	{
		std::string tempFlower = WJUtils::stringAddInt("", i, 3);
		std::string tempTip = WJUtils::stringAddInt("tips_", i, 3);
		Node *nodeFlower = m_flower->getChildByName(tempFlower);
		Node *nodeTip = m_flower->getChildByName(tempTip);

		if (nodeFlower)
		{
			nodeFlower->setVisible(false);
			nodeTip->setVisible(false);
			nodeTip->setTag(i);

			MarkFlower *mark = new MarkFlower(nodeFlower, nodeTip);
			m_flowerVec.push_back(mark);

			i++;
		}
		else
		{
			break;
		}
	} while (1);
}

void GarlandLayer::initArrow()
{
	m_arrow = GestureSprite::create(this, m_json->getChildByName("arrow"));
	m_json->addChild(m_arrow, 100);
	m_arrow->setPosition(m_json->getChildByName("arrow")->getPosition());
}

MarkLeaf* GarlandLayer::isCutCollidedWithGarland(Node *cut)
{
	if (!m_scissor->isCutFinished())
	{
		return nullptr;
	}
	
	for (auto it = m_leafVec.begin(); it != m_leafVec.end(); it++)
	{
		MarkLeaf *mark = *it;
		//LOGPOINT(m_json->convertToNodeSpace(mark->position));
		if (PUtils::isOneCollideWithOne(m_json->convertToNodeSpace(mark->position), cut, 50.f, 50.f, -120.f, -70.f, true, 15, 15))
		{
			mark->currentTime = WJUtils::millisecond();
			if (mark->currentTime - mark->startTime < COLLISION_TIME)
			{
				return nullptr;
			}
			mark->startTime = mark->currentTime;

			mark->weight--;
			mark->isVisible = false;
			if (mark->weight <= 0)
			{
				m_leafVec.erase(it);
			}

			return mark;
		}
	}

	return nullptr;
}

MarkFlower * GarlandLayer::isFlowerCutWithRect(Node *flower)
{
	for each (MarkFlower *mark in m_flowerVec)
	{
		if (PUtils::isOneCollideWithOne(mark->tip, flower, -100, -100))
		{
			//CCLOG("%d", mark->tip->getTag());
			return mark;
		}
	}

	return nullptr;
}

void GarlandLayer::changeFlower(Node * node, MarkFlower *mark, int index)
{
	WJSprite *flower = dynamic_cast<WJSprite*>(node);

	PUtils::addWJChildToAnotherParent(flower, m_flower);
	//PUtils::playParticle("particles/drop.plist", flower, flower->convertToNodeSpace(flower->getPositionWorld()), ZORDER_PROGRESS);

	flower->setPosition(mark->flower->getPosition());
	flower->setRotationSkewX(mark->flower->getRotationSkewX());
	flower->setRotationSkewY(mark->flower->getRotationSkewY());

	if (mark->flower)
	{
		mark->flower->removeFromParent();
	}

	mark->flower = flower;
	mark->index = index;
	mark->isPlaced = true;

	stopplayCircleAni();
	startplayCircleAni(3.f);
}

void GarlandLayer::startplayCircleAni(float delay)
{
	for each (MarkFlower *flower in m_flowerVec)
	{
		if (!flower->isPlaced)
		{
			PUtils::delayPlayFunc(this, CallFunc::create([=](){
				flower->tip->setVisible(true);
				flower->tip->stopAllActions();
				PUtils::playNodeCircleTipForever(flower->tip);

			}), delay);

		}
	}
}

void GarlandLayer::stopplayCircleAni()
{
	for each (MarkFlower *flower in m_flowerVec)
	{
		flower->tip->stopAllActions();
		flower->tip->setVisible(false);
	}
}
