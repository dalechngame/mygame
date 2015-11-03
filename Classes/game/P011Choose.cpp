#include "P011Choose.h"

INIT_SCENE_LOADER_SYNC(P011_Choose);


bool P011_Choose::init()
{
	if ( !PBase::init() )
	{
		return false;
	}

	initSceneJson("game/json/p011.json");

	PUtils::addListenerWithKey(m_sceneJson, "photo", 1, 6, nullptr, nullptr, nullptr,
		CC_CALLBACK_2(P011_Choose::onClickPhotoButton, this));
	
	return true;
}


GameNumber P011_Choose::getGameNumber()
{
	return GameNumber::P011_Choose;
}

void P011_Choose::onEnterTransitionDidFinish()
{
	PBase::onEnterTransitionDidFinish();
}

void P011_Choose::onClickPhotoButton(Node *node, WJTouchEvent *event)
{
	WJSprite *sprite = (WJSprite*)node;
	int tag = sprite->getUserTag();
	Loading::gotoScene(GameNumber::P011_Draw, (void*)(tag));
}
