#include "LayerGame.h"
#include "CardItem.h"
#include "stdlib.h"
#include "time.h"
#include "LayerOver.h"

#include "cocos-ext.h"	//cocos 扩展库
#include "SimpleAudioEngine.h"	//声音头文件

using namespace CocosDenshion;	//用于声音的命名空间


//产生scene并将layer挂上,在将scene挂到渲染树上
CCScene * LayerGame::scene(int degree)
{
	CCScene *scene = CCScene::create();
	LayerGame * layer = LayerGame::create(degree);
	scene->addChild(layer);
	return scene;
}

LayerGame * LayerGame::create(int degree)
{
	LayerGame * pRet = new LayerGame;
	if (pRet && pRet->init(degree))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}
bool LayerGame::init(int degree)
{
	CCLayer::init();

	setDegree(degree);
	setClickStart(0);
	getAllPoint();
	getAvailablePoint();
	addCard();

	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);

	//加载声音 
	SimpleAudioEngine::sharedEngine()->preloadEffect("click.wav");

	return  true;
}
//获取所有坐标
void LayerGame::getAllPoint()
{
	allPoints = CCPointArray::create(60);
	allPoints->retain();
	for (int i= 0; i < 10; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			allPoints->addControlPoint(ccp(80*i,80*j));
		}
	}
}
//根据难度获取随机数
void LayerGame::getAvailablePoint()
{
	srand(time(NULL));
	while (1)
	{
		int idx = rand() % 60;
		vector<int>::iterator itr = vec.begin();
		for (; itr != vec.end(); ++itr)
		{
			if (*itr == idx)
				break;
		}
		//如果不重复就往数组填充
		if (itr == vec.end())
		{
			vec.push_back(idx);
			if (vec.size() == getDegree())
				break;
		}
	}
}

//根据难度产生卡片,根据数组内容设置位置
void LayerGame::addCard()
{
	allCards = CCArray::create();
	allCards->retain();
	for (int i = 0; i < getDegree(); i++)
	{
		CardItem * item = CardItem::create(i);
		item->setPosition((CCPoint)allPoints->getControlPointAtIndex(vec[i]));
		addChild(item);
		item->showText();
		allCards->addObject(item);
	}
}

bool LayerGame::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	for (int i = 0; i < allCards->count();i++)
	{
		CardItem *tempCard = (CardItem*)allCards->objectAtIndex(i);
		if (tempCard->boundingBox().containsPoint(pTouch->getLocation()))
		{
				//将所有sprite置为白块
			showCardAllWhite();

			//如果点击的数字正确就从渲染树移除并且从array移除
			//播放声音
			SimpleAudioEngine::sharedEngine()->playEffect("click.wav");
			if (getClickStart() == tempCard->getIdx())
			{
				tempCard->removeFromParentAndCleanup(true);
				allCards->removeObjectAtIndex(i);
				if (allCards->count() == 0)
				{
					CCScene * scene = LayerOver::scene(true);
					CCDirector::sharedDirector()->replaceScene(scene);
					CCLog("you win game");
				}
			}
			else
			{
				CCScene * scene = LayerOver::scene(false);
				CCDirector::sharedDirector()->replaceScene(scene);
				CCLog("you lose game");
			}
			_clickStart++;
		}
	}
	return true;
}



void LayerGame::showCardAllWhite()
{
	CCObject * obj;
	CCARRAY_FOREACH(allCards,obj)
	{
		CardItem * spr = (CardItem *)obj;
		spr->showWhite();
	}
}