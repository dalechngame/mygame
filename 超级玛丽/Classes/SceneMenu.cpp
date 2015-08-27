#include "SceneMenu.h"
#include "SceneStart.h"
#include "LayerGame.h"
#include "Mario.h"
#include "GameInfo.h"

bool SceneMenu::init()
{
	CCLayer::init();

	// ����ͼƬ
	CCSprite* bg = CCSprite::create("bg.png");
	addChild(bg);
	bg->setPosition(ccp(winSize.width / 2, winSize.height / 2));

	// ����ѡ��
	CCNode* node = CCNode::create();
	for (int i = 0; i < 8; ++i)
	{
		CCSprite* sprite = CCSprite::create(Common::format(i + 1, "select", ".jpg"));
		//! ��ӵ�node �������õ���Ļ��
		node->addChild(sprite);
		sprite->setPosition(ccp(winSize.width / 2 + i*winSize.width, winSize.height / 2));
		sprite->setTag(1000 + i);
	}

	CCScrollView* view = CCScrollView::create(winSize, node);
	//! CCScrollView��CCLayerͬʱ��Ӧ
	addChild(view);
	view->setContentSize(CCSizeMake(8 * winSize.width, winSize.height));
	view->setDirection(kCCScrollViewDirectionHorizontal);
	_view = view;

	// ������Ϸ�Ĵ���
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);

	CCSprite *backA = CCSprite::create("backToMenu.png");
	CCSprite *backB = CCSprite::create("backToMenu.png");
	backB->setScale(0.65f);
	backA->setScale(0.6f);
	// ���ذ�ť
	CCMenuItemSprite* item = CCMenuItemSprite::create(backA, backB, this, menu_selector(SceneMenu::Back));
	CCMenu* menu = CCMenu::createWithItem(item);
	addChild(menu);
	item->setPosition(ccp(winSize.width / 2 - item->getContentSize().width / 6,
		item->getContentSize().height / 2 - winSize.height / 2));

	return true;
}

void SceneMenu::Back(CCObject*)
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.5f, Common::scene(SceneStart::create())));
}

bool SceneMenu::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void SceneMenu::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if (isClick(pTouch))
	{
		CCNode* node = _view->getContainer();
		CCPoint ptInNode = node->convertTouchToNodeSpace(pTouch);
		for (int i = 0; i < 8; ++i)
		{
			CCSprite* sprite = (CCSprite*)node->getChildByTag(1000 + i);
			if (sprite->boundingBox().containsPoint(ptInNode))
			{
				// ��������
				GameInfo::Utime = 300;
				GameInfo::UScore = 0;
				GameInfo::UCoin = 0;
				GameInfo::ULevel = 1;
				Mario::_life = 3;

				CCDirector::sharedDirector()->replaceScene(Common::scene(LayerGame::create(i)));
				break;
			}
		}
	}
}

//! �����ľ���С��5������
bool SceneMenu::isClick(CCTouch* pTouch)
{
	CCPoint ptStartLocation = pTouch->getStartLocation();
	CCPoint ptLoacation = pTouch->getLocation();
	return ptLoacation.getDistanceSq(ptStartLocation) <= 25;
}