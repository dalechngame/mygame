#include "P014Main.h"
#include "GameManager.h"

INIT_SCENE_LOADER_SYNC(P014_Main);

P014_Main::P014_Main() :  PBase()
{
	
}

P014_Main::~P014_Main()
{

}

bool P014_Main::init()
{
	if ( !PBase::init() )
	{
		return false;
	}

	initSceneJson("game/json/main.json");

	return true;
}

GameNumber P014_Main::getGameNumber()
{
	return GameNumber::P014_Main;
}

void P014_Main::onEnterTransitionDidFinish()
{
	PBase::onEnterTransitionDidFinish();
	executeCommand(0.5f, false);
}

void P014_Main::initSceneJson(const char* jsonFileName)
{
	PBase::initSceneJson(jsonFileName);

	m_sceneJson->setMoveAble(true);
	m_sceneJson->setHorizontalMove(false);
	m_sceneJson->setVerticalMove(true);
	m_sceneJson->noClickMoveEffect();
	m_sceneJson->setOnWillMoveTo(CC_CALLBACK_2(P014_Main::onSceneJsonWillMoveTo, this));
	m_sceneJson->setOnTouchEnded(CC_CALLBACK_2(P014_Main::onSceneJsonTouchEnded, this));
	m_sceneJson->setMoveAbleRect(Rect(0, 0, 1363, 950));

	m_star->setPosition(m_star->getPosition() + Vec2(-100, 10));
}

bool P014_Main::onSceneJsonWillMoveTo(Node *node, WJTouchEvent *event)
{
	return true;
}

void P014_Main::onSceneJsonTouchEnded(Node *node, WJTouchEvent *e)
{
	if (e)
	{
		float wh = e->heightInWorld;
		long long time = e->time;

		float y = node->getPosition().y;
		if (time < 300 && (wh > 30 || wh < -30) && time != 0)
		{
			y += ((float)(200 / time) * wh);
		}
		else
		{
			return;
		}

		Rect moveRect = m_sceneJson->getMoveAbleRect();
		if (y <= moveRect.getMinY())
			y = moveRect.getMinY() + 100;

		else if (y >= moveRect.getMaxY())
			y = moveRect.getMaxY() - 100;

		node->stopAllActions();
		node->runAction(Sequence::create(
			EaseSineOut::create(
			MoveTo::create(0.5f, Vec2(node->getPositionX(), y))),
			NULL));
	}
}

void P014_Main::onExit()
{
	PBase::onExit();
	GManager->cleanAllGameCommandFromVector();
}
