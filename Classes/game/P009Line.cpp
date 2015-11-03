#include "P009Line.h"
#include "LineLayer.h"

INIT_SCENE_LOADER_SYNC(P009);

P009::P009()
{
	m_startDrawing = false;
}

P009::~P009()
{

}

bool P009::init()
{
	if ( !PBase::init() )
	{
		return false;
	}

	initSceneJson("game/json/P009.json");

	initEvent();

	initLineLayer();
	
	return true;
}


void P009::initEvent()
{

	// 背景运动
	WJSprite *bgSprite = m_sceneJson->getSubSpriteByKey("bg");
	WJUtils::setNodeAnchorPoint(bgSprite, Vec2::ANCHOR_MIDDLE);
	bgSprite->runAction(RepeatForever::create(
		Sequence::create(
			RotateBy::create(55.0f, 180),
			RotateBy::create(55.0f, 180),
		nullptr)));
}

void P009::initLineLayer()
{
	LineLayer *lastLayer = nullptr;
	for (int i = 1; i <= 9; i++)
	{

		int next = (i != 9 ? i + 1 : 1);
		WJSprite *sprite1 = m_sceneJson->getSubSprite(WJUtils::stringAddInt("line", i, 3).c_str());
		WJSprite *sprite2 = m_sceneJson->getSubSprite(WJUtils::stringAddInt("line", next, 3).c_str());
		sprite1->setLocalZOrder(200);
		sprite1->setVisible(false);
		sprite2->setLocalZOrder(200);
		sprite2->setVisible(false);
		LineLayer *layer = LineLayer::create(sprite1, sprite2);
		m_sceneJson->addChild(layer, 100);
		if (i > 1)
		{
			lastLayer->setNextLine(layer);
		}
		else
		{
			layer->getLineBg()->setVisible(true);
			layer->setIsDrawedAble(true);
		}

		lastLayer = layer;
	}

	// 第一个点和第二个点显示
	m_sceneJson->getSubSprite(WJUtils::stringAddInt("line", 1, 3).c_str())->setVisible(true);
	m_sceneJson->getSubSprite(WJUtils::stringAddInt("line", 2, 3).c_str())->setVisible(true);
}

bool P009::onSceneLayerTouchAble(Node *node, WJTouchEvent *event)
{
	m_startDrawing = true;

	return true;
}

bool P009::onSceneLayerWillMoveTo(Node *node, WJTouchEvent *event)
{
	return false;
}

void P009::onSceneLayerTouchEnded(Node *node, WJTouchEvent *event)
{
	m_startDrawing = false;
}

GameNumber P009::getGameNumber()
{
	return GameNumber::P009;
}

void P009::onEnterTransitionDidFinish()
{
	PBase::onEnterTransitionDidFinish();

	delayStartMoveFingerTipsForever(m_sceneJson->getSubSprite("line001")->getPositionWorld(),
		m_sceneJson->getSubSprite("line002")->getPositionWorld());
}
