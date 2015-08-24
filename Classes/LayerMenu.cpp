#include "LayerMenu.h"
#include "LayerStart.h"
#include "LayerGame.h"

bool LayerMenu::init()
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

	// ���ذ�ť
	CCMenuItemImage* item = CCMenuItemImage::create("backA.png", "backB.png", this, menu_selector(LayerMenu::Back));
	CCMenu* menu = CCMenu::createWithItem(item);
	addChild(menu);
	item->setPosition(ccp(winSize.width / 2 - item->getContentSize().width / 2,
		item->getContentSize().height / 2 - winSize.height / 2));

	return true;
}

void LayerMenu::Back(CCObject*)
{
	CCDirector::sharedDirector()->replaceScene(Common::scene(LayerStart::create()));
}

bool LayerMenu::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void LayerMenu::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
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
				// ������Ϸ����
				CCDirector::sharedDirector()->replaceScene(Common::scene(LayerGame::create(i)));
				break;
			}
		}
	}
}

//! �����ľ���С��5������
bool LayerMenu::isClick(CCTouch* pTouch)
{
	CCPoint ptStartLocation = pTouch->getStartLocation();
	CCPoint ptLoacation = pTouch->getLocation();
	return ptLoacation.getDistanceSq(ptStartLocation) <= 25;
}