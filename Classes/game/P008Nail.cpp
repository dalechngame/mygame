#include "P008Nail.h"
#include "ScrollItem.h"
#include "SpriteWithHue.h"

INIT_SCENE_LOADER_SYNC(P008);

static bool s_firstNailLayer = true;

P008::P008() :  PBase()
{
	m_scrollLayer = nullptr;
	m_choseWringTime = 0;
}

P008::~P008()
{

}

bool P008::init()
{
	if ( !PBase::init() )
	{
		return false;
	}

	initSceneJson("game/json/p008.json");

	initNailStyle();

	initHand();

	return true;
}

void P008::initNailStyle()
{
	// 选择指甲的样式
	m_scrollLayer = ScrollLayer::create("P008_style_", ScrollDragDir::DragLeft, 6, true, s_firstNailLayer);
	m_scrollLayer->setClickScrollItemEvent(CC_CALLBACK_3(P008::onClickNailStyleScrollItem, this));
	m_scrollLayer->setScrollItemMovingCollisionEvent(CC_CALLBACK_3(P008::onScrollStyleMovingCollision, this));
	m_scrollLayer->setScrollItemCollisonEndedEvent(CC_CALLBACK_3(P008::onScrollStyleCollisonEndedEvent, this));
	SCROLL_POSITION_IN_JSON(m_scrollLayer);
	m_sceneJson->addChild(m_scrollLayer, 600);
	m_scrollLayer->initItemSelectFromIndex(0);
	s_firstNailLayer = false;
}

void P008::initHand()
{
	// libii公主的手
	m_hand = HandLayer::create(m_sceneJson->getSubLayer("layer"));
	m_hand->ignoreAnchorPointForPosition(false);
	m_hand->setClickNormalNailCallBack(CC_CALLBACK_2(P008::onClickHandNail, this));
	m_hand->setChooseWristCallBack(CC_CALLBACK_2(P008::onChooseWristCallBack, this));
	m_sceneJson->addChild(m_hand, 500);
	m_hand->saveCurrentProperties();
	m_sceneJson->getSubLayer("layer")->setVisible(false);
}

void P008::onClickNailStyleScrollItem(Node *node, WJTouchEvent *e, WJScrollLayer *scroll)
{
	ScrollItem *item = dynamic_cast<ScrollItem*>(node);
	m_hand->changeNewStyleNail(item->getItemIndex() + 1);
}

bool P008::onScrollStyleMovingCollision(Node *node, WJTouchEvent *e, WJScrollLayer *scroll)
{
	ScrollItem *item = (ScrollItem*)node;
	return m_hand->checkStyleCalculateAABBInWorld(item) != -1;
}

void P008::onChooseWristCallBack(Node *node, WJTouchEvent *e)
{
	m_choseWringTime++;
	showNextButton();

	// 选择了3个手环后在添加一颗星星
	if (m_choseWringTime == 3)
		setStarNumWhenMakeNail((ProgressStar)((int)getProgressStarNum() + 1));
}

void P008::onScrollStyleCollisonEndedEvent(Node *node, WJTouchEvent *e, WJScrollLayer *scroll)
{
	ScrollItem *item = dynamic_cast<ScrollItem*>(node);
	m_hand->changeNewStyleNail(item->getItemIndex() + 1, m_hand->checkStyleCalculateAABBInWorld(item));

	// 移除掉新产生的花纹
	WJSprite *sprite = item->getMovingSprite();
	WJSprite* newSprite = dynamic_cast<WJSprite*>(sprite->getUserObject());
	newSprite->followNode(nullptr);
	sprite->removeFromParentAndCleanup(true);
	sprite = nullptr;
	newSprite->removeFromParentAndCleanup(true);
	newSprite = nullptr;
} 

void P008::onClickHandNail(Node *node, WJTouchEvent *e)
{
	m_scrollLayer->moveOutSideScene();
}

GameNumber P008::getGameNumber()
{
	return GameNumber::P008;
}

void P008::onEnterTransitionDidFinish()
{
	PBase::onEnterTransitionDidFinish();
	m_scrollLayer->show(1.0f);
}

void P008::setStarNumWhenMakeNail(ProgressStar star)
{
	if (!m_nextButton->isVisible() && star >= ProgressStar::oneStar)
		showNextButton();

	setProgressStarNum(star);
}

void P008::onClickNextButton(Node *node, WJTouchEvent *event)
{
	PBase::onClickNextButton(node, event);
	HandLayerState state = m_hand->getHandstate();
	// 当前显示整个手和对单个指甲操作的时候点击next进入手环装饰
	if (state == HandLayerState::normalShowAllState ||
		state == HandLayerState::drawingSingleNailColorState ||
		state == HandLayerState::drawingSingleNailDecorState ||
		state == HandLayerState::drawingSingleNailPatternState)
	{
		m_hand->gotoWristBandMode();
		hideNextButton();
	}
	// 当前装饰手环
	else if (state == HandLayerState::wristbandHandState)
	{
		showStarDialog();
	}
}
