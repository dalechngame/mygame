#include "P005Meet.h"
#include "ScrollItem.h"
#include "ScrollDiyItem.h"

INIT_SCENE_LOADER_ASYNC(P005);

// 一共为左侧item个数 + 2 个
static bool P012ShowScrollIsFirstRun[5] = {
	true, true, true, true, true
};

// 拖动中的角色的小图标的zorder
const int ZORDER_ICON_DRAGGING = 200;
// 男客人的zorder
const int ZORDER_MODEL_MAN = 50;
//  女客人
const int ZORDER_MODEL_WOMAN = 70;
// 猫和狗
const int ZORDER_MODEL_ANIMAL = 300;
// 右边ui的zorder
const int ZORDER_UI_SCROLL = 99999;

// 场景中最多放的人不包括libii公主
const int PEOPLE_MAX_NUM = 5;
// 场景中最多放的宠物
const int ANIMAL_MAX_NUM = 2;

// 人物的menu index
#define SCROLL_MAN_INDEX                 2

#define P005_ANIMAL_SCALE_NUM          0.65f     // 猫和狗的缩放比例
#define P005_GUEST_SCALE_NUM           0.9f      // 客人的缩放比例


P005::P005()
{
	m_nowMaxZorder = 350;
}

P005::~P005()
{

}

void P005::asyncLoadSceneResource()
{
	WJLayerJsonPreload::getInstance()->asyncPreload("game/json/p005.json");
	WJSkeletonDataCache::getInstance()->addSkeletonDataAsync("game/skeleton/libii/princess.json", "game/skeleton/libii/princess.atlas");
}

bool P005::init()
{
	if ( !PBase::init())
	{
		return false;
	}

	initSceneJson("game/json/p005.json");

	initEvent();

	initCharas();

	initZorder();

	executeCommand();

	return true;
}


GameNumber P005::getGameNumber()
{
	return GameNumber::P005;
}

void P005::onEnterTransitionDidFinish()
{
	PBase::onEnterTransitionDidFinish();
	m_scrollDoubleLayer->show(1.0f);
}

void P005::initEvent()
{
	// 选择弹窗
	m_scrollDoubleLayer = ScrollDoubleLayer::create("P005_left_", ScrollDragDir::StopDrag, 3, true, P012ShowScrollIsFirstRun);
	m_scrollDoubleLayer->setScrollItemMovingCollisionEvent(CC_CALLBACK_3(P005::onScrollLayerMovingCollision, this));
	m_scrollDoubleLayer->setClickMenuScrollItemEvent(CC_CALLBACK_3(P005::onClickMenuScrollItemCallBack, this));
	m_scrollDoubleLayer->setClickScrollItemEvent(CC_CALLBACK_3(P005::onClickScrollItemCallBack, this));
	m_scrollDoubleLayer->setScrollItemCollisonEndedEvent(CC_CALLBACK_3(P005::onScrollItemCollisonEnded, this));
	m_scrollDoubleLayer->setScrollDragOutInitNodeCallBack(CC_CALLBACK_3(P005::onScrollDragOutInitNodeCallBack, this));
	m_scrollDoubleLayer->setScrollItemTouchEndedEvent(CC_CALLBACK_3(P005::onScrollItemTouchEnded, this));
	m_scrollDoubleLayer->setFailedSelectItemEvent(CC_CALLBACK_3(P005::onFailedSelectItem, this));
	m_scrollDoubleLayer->setScrollItemDragOutCallBack(CC_CALLBACK_3(P005::onScrollItemDragOutCallBack, this));
	SCROLL_POSITION_IN_JSON(m_scrollDoubleLayer);
	m_sceneJson->addChild(m_scrollDoubleLayer, ZORDER_UI_SCROLL);
}

void P005::initZorder()
{
	WJSprite *sprite;

	// 雪人
	sprite = m_sceneJson->getSubSprite("snowman");
	sprite->setLocalZOrder(ZORDER_UI_SCROLL - 10);
	// 桌子
	sprite = m_sceneJson->getSubSprite("table");
	sprite->setLocalZOrder(ZORDER_UI_SCROLL - 10);
	// 桌子上面的糖球
	sprite = m_sceneJson->getSubSprite("cakepop");
	sprite->setLocalZOrder(ZORDER_UI_SCROLL - 10);
	// 沙画瓶
	sprite = m_sceneJson->getSubSprite("bottle");
	sprite->setLocalZOrder(ZORDER_UI_SCROLL - 10);
}

void P005::onClickMenuScrollItemCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollMenuItem *item = (ScrollMenuItem*)node;
	replaceItemToDiyItem(item, touchEvent, scroll);
	clickDiyMenuItemCallBack(item, touchEvent, scroll);
}

void P005::onScrollItemDragOutCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollDiyItem *item = (ScrollDiyItem*)node;
	Node *sprite = dynamic_cast<Node*>(item->getMovingSprite()->getUserObject());
	if (sprite) sprite->setLocalZOrder(999999);
}

void P005::replaceItemToDiyItem(ScrollMenuItem* menuItem, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	// 将scrollItem替换成ScrollDiyItem
	ScrollItem *item;
	for (int i = 0; i < m_scrollDoubleLayer->getItemChildCount(); i++)
	{
		item = (ScrollItem*)m_scrollDoubleLayer->getItemFromIndex(i);

		// 创建新的item
		ScrollDiyItem *newItem = ScrollDiyItem::create(item->getPScrollLayer(), item->getPrimievalKey().c_str(),
			item->getItemIndex(), item->getDragDir(), item->getItemClickAble());
		item->cloneEventTo(newItem);

		item->getParent()->addChild(newItem);
		m_scrollDoubleLayer->replaceScrollItem(item, newItem);

		item->removeFromParentAndCleanup(true);
		item = nullptr;

		// 人物的选择index
		if (m_scrollDoubleLayer->getPreSelectMenuIndex() == SCROLL_MAN_INDEX)
		{
			ModelBase *model = m_charas[newItem->getItemIndex()];
			newItem->setModelIntoMoveScene(model->getModelIsMoveScene());
		}
	}
}

void P005::clickDiyMenuItemCallBack(ScrollMenuItem* item, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	int index = item->getItemIndex();
	ScrollDoubleTemplate object = ScrollDoubleTemplateTem(item->getScrollItemKey());
	if (object.saveType == -1)
		return;

	// 从第二个开始，第一个是默认的
	for (int i = 1; i < m_scrollDoubleLayer->getItemChildCount(); i++)
	{
		ScrollDiyItem *item = (ScrollDiyItem*)m_scrollDoubleLayer->getItemFromIndex(i);
		item->setFileSaveType((DiySaveType)object.saveType);
		std::string fileName = GameManager::getInstance()->getFileFromUserSavedWithTypeAndIndex((DiySaveType)object.saveType, i);
		if (FileUtils::getInstance()->isFileExist(fileName))
		{
			// 替换用户保存的图片
			item->getItemIconSprite()->loadSpriteTexture(fileName.c_str());
			item->setItemIconFile(fileName);
			item->setItemIconScale(object.saveIconScale);
			item->getItemIconSprite()->setScale(object.saveIconScale);
			item->setItemFilePath(fileName);
			item->setItemScaleTo(object.saveFileScale);
			item->getItemIconSprite()->saveCurrentProperties();
			item->setNeedPlayGame(false);
		}
	}

}

Node* P005::onScrollDragOutInitNodeCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	if (m_scrollDoubleLayer->getPreSelectMenuIndex() == SCROLL_MAN_INDEX)
	{
		ScrollDiyItem *item = (ScrollDiyItem*)node;
		ModelBase *model = m_charas[item->getItemIndex()];
		model->playAnimation("aniIdle", true, 1);

		return model;
	}

	return nullptr;
}

void P005::onScrollItemTouchEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{

}

void P005::onFailedSelectItem(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	
}

void P005::onClickScrollItemCallBack(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollDiyItem *item = (ScrollDiyItem*)node;
	//DiySaveType type = item->getFileSaveType();
	int menuindex = m_scrollDoubleLayer->getPreSelectMenuIndex();
	switch (menuindex)
	{
		case 0:
			changeSugar(item);
			break;

		case 1:
			changeDreamCatcher(item);
			break;
		
		case 2:
			onClickModelScrollItem(item);
			break;

		default:
			break;
	}
}

void P005::onClickModelScrollItem(ScrollDiyItem* item)
{
	int index = item->getItemIndex();
	if (!WJUtils::canClick(WJUtils::stringAddInt("item", index, 3).c_str(), 500))
		return;

	Common::sound.play("Common:button");
	ModelBase *base = m_charas[index];

	// 当前人物已经存在在场景中了
	if (base->getModelIsMoveScene())
	{
		// 移除当前的模特出场景
		removeModelFromScene(base, item);
	}
	// 添加人物进入场景
	else
	{
		Point point = Point::ZERO;
		ModelNo no = (ModelNo)base->getModelNo();
		Node *leftNode = m_sceneJson->getSubNode("left");
		Node *rightNode = m_sceneJson->getSubNode("right");
		// 王子(王子比公主放下去40个像素)
		if (no == ModelNo::blackKing || no == ModelNo::whitesKing)
		{
			point.y = leftNode->getPosition().y - 40;
		}
		else if (no == ModelNo::asiaPrince || no == ModelNo::blackPrince || no == ModelNo::whitesPrince)
		{
			point.y = leftNode->getPosition().y;
		}
		else
		{
			point.y = leftNode->getPositionY() - 140;
		}

		point.x = leftNode->getPositionX() + WJUtils::randomInt(rightNode->getPositionX() - leftNode->getPositionX());

		base->setPosition(base->getIsAnimal() ? point : Vec2(findEmptyMostPointFromStayModel(point), point.y));
		m_sceneJson->addChild(base);
		putModelIntoScene(base, item);
	}
}

void P005::putModelIntoScene(ModelBase *base, ScrollDiyItem *item)
{
	// 进入场景
	base->stopAllActions();
	base->followNode(nullptr);
	base->playAnimation("aniIdle", true,  1);
	base->setScale(base->getIsAnimal() ? P005_ANIMAL_SCALE_NUM : P005_GUEST_SCALE_NUM);
	base->setModelIsMoveScene(true);
	base->startPlayStandByAnimationSchedule();
	base->setLocalZOrder(++m_nowMaxZorder);
	item->setModelIntoMoveScene(true);
}

void P005::removeModelFromScene(ModelBase *base, ScrollDiyItem *item)
{
	base->setModelIsMoveScene(false);
	item->setModelIntoMoveScene(false);

	base->stopStandbyAnimationSchedule();
	base->removeFromParentAndCleanup(false);
	base = NULL;
}

float P005::findEmptyMostPointFromStayModel(const Vec2 &point)
{
	Vec2 pointCheck = point;
	float guass = 0.0f;
	float guassPrince = 0.0f;
	float distance = 0.0f;

	// 当前随机的位置和公主的位置的影响系数，正态分布
	distance = m_princess->getPosition().getDistance(point);
	guassPrince = getKerelnFromDistance(distance, 500);

	Node *leftNode = m_sceneJson->getSubNode("left");
	Node *rightNode = m_sceneJson->getSubNode("right");

	// 计算当前随机到的一个点和场景中其他人物的一个
	guass = guassPrince;
	ModelBase *modelBase;
	for (int i = 0; i < 7; i++)
	{
		modelBase = m_charas[i];
		// 宠物在最前面不用来计算和未加入到场景中的
		if (modelBase->getIsAnimal() || !modelBase->getModelIsMoveScene())
			continue;

		distance = modelBase->getPosition().getDistance(point);
		guass += getKerelnFromDistance(distance);
	}

	// 编译当前从左到右所有点的影响系数，近似找出当前的影响最小的及当前能找到的离人物最远的值
	float startPointX = m_sceneJson->convertToNodeSpace(Vec2(130, 0)).x;
	float endPointX = m_sceneJson->convertToNodeSpace(Vec2(m_winSize.width - 170, 0)).x;
	for (float k = startPointX; k <= endPointX; k++)
	{
		Vec2 tempPoint = Vec2(k, point.y);
		float tempGuass = getKerelnFromDistance(m_princess->getPosition().getDistance(tempPoint), 500);
		for (int j = 0; j < 7; j++)
		{
			// 宠物在最前面不用来计算
			if (m_charas[j]->getIsAnimal() || !m_charas[j]->getModelIsMoveScene())
				continue;

			distance = m_charas[j]->getPosition().getDistance(tempPoint);
			tempGuass += getKerelnFromDistance(distance);
		}

		// 找到一个值比当前值小
		if (tempGuass < guass)
		{
			// 场景中没得人的情况不去寻找离公主比较远的点
			if (getModelNumIntoScene() == 0 && m_princess->getPosition().getDistance(tempPoint) > 250)
				continue;

			guass = tempGuass;
			pointCheck.x = k;
		}
	}

	return pointCheck.x;
}

int P005::getModelNumIntoScene()
{
	int result = 0;
	for (int i = 0; i < 7; i++)
	{
		if (m_charas[i]->getModelIsMoveScene())
			result++;
	}

	return result;
}

float P005::getKerelnFromDistance(float distance, float import)
{
	// 正态分布曲线
	static const double SQRT2PI = sqrt(2.0 * 3.14159265358f);
	double sigma = (double)120 / 3.0;
	double sigma2 = 2.0 * sigma * sigma;
	double sigmap = sigma * SQRT2PI;

	return exp(-(double)(distance * distance) / sigma2) / sigmap * import * 100;
}

void P005::changeDreamCatcher(ScrollDiyItem *item)
{
	// 捕梦网
	WJSprite *sprite = m_sceneJson->getSubSprite("dreamcatcher");
	sprite->loadSpriteTexture(item->getItemFilePath().c_str());
}

void P005::changeSugar(ScrollDiyItem *item)
{
	// 糖球
	WJSprite *sprite = m_sceneJson->getSubSprite("cakepop");
	sprite->loadSpriteTexture(item->getItemFilePath().c_str());
}

bool P005::onScrollLayerMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	// 添加人物
	if (m_scrollDoubleLayer->getPreSelectMenuIndex() == SCROLL_MAN_INDEX)
	{
		return onPeopleScrollLayerMovingCollision(node, touchEvent, scroll);
	}

	return false;
}

bool P005::onPeopleScrollLayerMovingCollision(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollDiyItem *item = (ScrollDiyItem*)node;
	WJLayer *layer = m_sceneJson->getSubLayerByKey("manlayer");
	Rect rect = layer->getContentRectInWorld();
	ModelBase *model = m_charas[item->getItemIndex()];
	if (rect.containsPoint(model->getSkeleton()->getPositionWorld()))
	{
		return true;
	}

	return false;
}

void P005::onScrollItemCollisonEnded(Node* node, WJTouchEvent* touchEvent, WJScrollLayer* scroll)
{
	ScrollDiyItem *item = (ScrollDiyItem*)node;
	if (m_scrollDoubleLayer->getPreSelectMenuIndex() == SCROLL_MAN_INDEX)
	{
		// 人物
		ModelBase *model = m_charas[item->getItemIndex()];
		putModelIntoScene(model, item);
	}
}

void P005::initCharas()
{
	// 创建白人王子角色
	GuestModel *princeWhiteModel = GuestModel::create((int)GuestModelNo::princeWhite, false);
	princeWhiteModel->retain();
	princeWhiteModel->initMoveEvent();
	princeWhiteModel->setModelClickEvent();
	princeWhiteModel->setTimerPlaySound(false);
	princeWhiteModel->setModelTouchEndCallBack(CC_CALLBACK_2(P005::onModelTouchEndCallBack, this));
	princeWhiteModel->setStandbyAnimation("start", "aniSmile", "aniNod", "aniIdle01", "aniIdle02", "aniIdle03",
		"aniShake", "aniHappy", "aniShoke", "end");
	//princeWhiteModel->setStandbySound("start", "Common:0022_prince", "end");
	Vec2 manStartPoint = m_sceneJson->convertToNodeSpace(Vec2(120, 55));
	princeWhiteModel->setMoveAbleRect(Rect(manStartPoint.x, manStartPoint.y, m_winSize.width - 210, m_winSize.height - 250));

	// 创建黑人王子角色
	GuestModel *princeBlackModel = GuestModel::create((int)GuestModelNo::princeBlack, false);
	princeBlackModel->retain();
	princeBlackModel->initMoveEvent();
	//princeBlackModel->setScrollItem(m_charaScroll->getItem(0));
	princeBlackModel->setModelClickEvent();
	princeBlackModel->setTimerPlaySound(false);
	princeBlackModel->setModelTouchEndCallBack(CC_CALLBACK_2(P005::onModelTouchEndCallBack, this));
	princeBlackModel->setStandbyAnimation("start", "aniSmile", "aniNod", "aniIdle01", "aniIdle02", "aniIdle03",
		"aniShake", "aniHappy", "aniShoke", "end");
	//princeBlackModel->setStandbySound("start", "Common:0022_prince", "end");
	//MAN_MOVE_RECT(princeBlackModel);
	princeBlackModel->setMoveAbleRect(Rect(manStartPoint.x, manStartPoint.y, m_winSize.width - 210, m_winSize.height - 250));

	// 创建亚洲公主角色
	GuestModel *princessAsianModel = GuestModel::create((int)GuestModelNo::princessAsian, false);
	princessAsianModel->retain();
	princessAsianModel->initMoveEvent();
	//princessAsianModel->setScrollItem(m_charaScroll->getItem(4));
	princessAsianModel->setModelClickEvent();
	princessAsianModel->setTimerPlaySound(false);
	princessAsianModel->setModelTouchEndCallBack(CC_CALLBACK_2(P005::onModelTouchEndCallBack, this));
	princessAsianModel->setStandbyAnimation("start", "aniSmile", "aniNod", "aniIdle01", "aniIdle02", "aniIdle03",
		"aniShake", "aniHappy", "aniShoke", "end");
	//princessAsianModel->setStandbySound("start", "Common:0024_otherprincess", "end");
	Vec2 womanStartPoint = m_sceneJson->convertToNodeSpace(Vec2(120, 110));
	princessAsianModel->setMoveAbleRect(Rect(womanStartPoint.x, womanStartPoint.y, m_winSize.width - 210, m_winSize.height - 250));


	// 创建黑人公主角色
	GuestModel *princessBlackModel = GuestModel::create((int)GuestModelNo::princessBlack, false);
	princessBlackModel->retain();
	princessBlackModel->initMoveEvent();
	princessBlackModel->setModelClickEvent();
	princessBlackModel->setTimerPlaySound(false);
	princessBlackModel->setModelTouchEndCallBack(CC_CALLBACK_2(P005::onModelTouchEndCallBack, this));
	princessBlackModel->setStandbyAnimation("start", "aniSmile", "aniNod", "aniIdle01", "aniIdle02", "aniIdle03",
		"aniShake", "aniHappy", "aniShoke", "end");
	//princessBlackModel->setStandbySound("start", "Common:0024_otherprincess", "end");
	princessBlackModel->setMoveAbleRect(Rect(womanStartPoint.x, womanStartPoint.y, m_winSize.width - 210, m_winSize.height - 250));

	// 创建白人公主角色
	GuestModel *princessWhiteModel = GuestModel::create((int)GuestModelNo::princessWhite, false);
	princessWhiteModel->retain();
	princessWhiteModel->initMoveEvent();
	princessWhiteModel->setModelClickEvent();
	princessWhiteModel->setTimerPlaySound(false);
	princessWhiteModel->setModelTouchEndCallBack(CC_CALLBACK_2(P005::onModelTouchEndCallBack, this));
	princessWhiteModel->setStandbyAnimation("start", "aniSmile", "aniNod", "aniIdle01", "aniIdle02", "aniIdle03",
		"aniShake", "aniHappy", "aniShoke", "end");
	//princessWhiteModel->setStandbySound("start", "Common:0024_otherprincess", "end");
	princessWhiteModel->setMoveAbleRect(Rect(womanStartPoint.x, womanStartPoint.y, m_winSize.width - 210, m_winSize.height - 250));

	// 创建宠物狗角色
	GuestModel *petDogModel = GuestModel::create((int)GuestModelNo::petDog, false);
	petDogModel->retain();
	petDogModel->initMoveEvent();
	petDogModel->setIsAnimal(true);
	petDogModel->setModelClickEvent();
	petDogModel->setTimerPlaySound(false);
	petDogModel->setModelTouchEndCallBack(CC_CALLBACK_2(P005::onModelTouchEndCallBack, this));
	petDogModel->setStandbyAnimation("start", "aniAdd", "aniHappy", "aniIdle01", "aniIdle02", "aniIdle03",
		"aniKiss", "end");
	//petDogModel->setStandbySound("start", "Common:0036", "end");
	Vec2 animalStartPoint = m_sceneJson->convertToNodeSpace(Vec2(120, -10));
	petDogModel->setMoveAbleRect(Rect(animalStartPoint.x, animalStartPoint.y, m_winSize.width - 220, m_winSize.height - 250));

	// 创建宠物猫角色
	GuestModel *petCatModel = GuestModel::create((int)GuestModelNo::petCat, false);
	petCatModel->retain();
	petCatModel->initMoveEvent();
	petCatModel->setIsAnimal(true);
	petCatModel->setModelClickEvent();
	petCatModel->setTimerPlaySound(false);
	petCatModel->setModelTouchEndCallBack(CC_CALLBACK_2(P005::onModelTouchEndCallBack, this));
	petCatModel->setStandbyAnimation("start", "aniAdd", "aniHappy", "aniIdle01", "aniIdle02", "aniIdle03",
		"aniKiss", "end");
	//petCatModel->setStandbySound("start", "Common:0035", "end");
	petCatModel->setMoveAbleRect(Rect(animalStartPoint.x, animalStartPoint.y, m_winSize.width - 220, m_winSize.height - 250));

	// 保存当前创建的人物
	m_charas[0] = princeBlackModel;
	m_charas[1] = princeWhiteModel;

	m_charas[2] = princessBlackModel;
	m_charas[3] = princessWhiteModel;
	m_charas[4] = princessAsianModel;

	m_charas[5] = petDogModel;
	m_charas[6] = petCatModel;

	// libii公主
	m_princess = PrincessModel::create();
	m_princess->setStandbyAnimation("start", "aniHappy", "aniSmile", "aniCoversmile", "aniNod", "aniShoke", "aniLookup", "aniSalute",
		"aniLookdown2", "aniWow", "end");
	m_princess->setSoundIsBindAni(true);
	m_princess->setPrincessModelClickEvent();
	m_princess->initMoveEvent();
	m_princess->setTimerPlaySound(false);
	m_princess->startPlayStandByAnimationSchedule();
	m_princess->setModelTouchEndCallBack(CC_CALLBACK_2(P005::onModelTouchEndCallBack, this));
	m_sceneJson->replaceSubNode("libii", m_princess, true);
	const Size &size = m_princess->getContentSize();
	m_princess->setCustomizeTouchRect(Rect(size.width / 2 - 135, size.height / 2, 280, 580), true);
	Vec2 princessStarPoint = m_sceneJson->convertToNodeSpace(Vec2(110, -25));
	m_princess->setMoveAbleRect(Rect(princessStarPoint.x, princessStarPoint.y, m_winSize.width - 180, m_winSize.height - 230));
}

void P005::onExit()
{
	PBase::onExit();
	for (ModelBase* chara : m_charas)
	{
		CC_SAFE_RELEASE(chara);
	}
}

void P005::onModelTouchEndCallBack(Node *node, WJTouchEvent *event)
{
	ModelBase *model = (ModelBase*)node;
	model->setLocalZOrder(++m_nowMaxZorder);
}
