#include "LayerGame.h"
#include "AppMacros.h"
#include "Block.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

//重载了ccColor3B的 ==号
static inline bool operator ==(const ccColor3B &a, const ccColor3B &b)
{
	return a.r == b.r&&a.g == b.g&&a.b == b.b;
}

CCScene *LayerGame::scene()
{
	CCScene * scene = CCScene::create();
	LayerGame * layer = LayerGame::create();
	scene->addChild(layer);
	return scene;
}

bool LayerGame::init()
{
	CCLayer::init();
	startGame();

	setLineCount(0);
	showEnd = false;
	isRunning = false;

	ttf = CCLabelTTF::create("0.0000", "Courier New", 30);
	ttf->setPosition(ccp(winSize.width / 2, winSize.height - 20));
	ttf->setColor(ccBLUE);
	ttf->setZOrder(100);
	addChild(ttf);

	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
	//! 设置安卓返回键
	setKeypadEnabled(true);
	srand(time(NULL));

	return true;
}

void LayerGame::startGame()
{
	addStartLineBlocks();
	addNormalBlocks(1);
	addNormalBlocks(2);
	addNormalBlocks(3);
#ifdef DEBUG
	CCLog("%d", this->getLineCount());
#endif // DEBUG

}

void LayerGame::addStartLineBlocks()
{
	CCSize startBlockSize = CCSize(winSize.width , winSize.height / 4);
	Block *b = Block::create(startBlockSize, ccYELLOW, "start", 30, ccBLACK);
	b->setLineIndex(0);
	_lineCount++;
	addChild(b);
	//类成员变量不会初始化- -
	//CCLog("%d", b->getLineIndex());
	
}

void LayerGame::addEndLineBlocks()
{
	CCSize endBlockSize = CCSize(winSize.width, winSize.height);
	Block *b = Block::create(endBlockSize, ccGREEN, "Game Over", 50, ccBLACK);
	b->setPosition(ccp(0, winSize.height));
	b->setLineIndex(4);
	_lineCount++;
	addChild(b);
}

void LayerGame::addNormalBlocks(int lineIndex)
{
	CCSize norBlackSize = CCSize(winSize.width/4 -1 ,  winSize.height/ 4-1);
	int black = rand() % 4;
	for (int i = 0; i < 4; i++)
	{
		//产生4个block 只有一个黑色的block
		Block * b = Block::create(norBlackSize, i == black?ccBLACK: ccWHITE, "", 20, ccWHITE);
		b->setLineIndex(lineIndex);
		b->setPosition(ccp(i * winSize.width / 4 , lineIndex * winSize.height / 4));
		addChild(b);
	}
	_lineCount++;
}

bool LayerGame::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCArray *array = Block::getBlocksArray();
	CCObject *obj;
	CCARRAY_FOREACH(array, obj)
	{
		Block *b = (Block *)obj;
		if (b->boundingBox().containsPoint(pTouch->getLocation()) &&
			b->getLineIndex() == 1)
		{
			if (b->getColor() == ccBLACK)
			{
				SimpleAudioEngine::sharedEngine()->playEffect("black.wav");
				startTimer();
				//重置黑块的颜色
				b->setColor(ccGRAY);
				this->moveDown();
			}
			else if (b->getColor() ==  ccGREEN)
			{
				SimpleAudioEngine::sharedEngine()->playEffect("over.wav");
				this->moveDown();
				stopTimer();
			}
			else if (b->getColor() == ccWHITE)
			{
				SimpleAudioEngine::sharedEngine()->playEffect("white.wav");
			}
			//因为每次循环都会删除一个元素,防止访问到没用的元素
			break;
		}
	}
	return false;
}

void LayerGame::moveDown()
{
	if (getLineCount() < 20)
	{
		addNormalBlocks(4);
	}
	else if (!showEnd)
	{
		addEndLineBlocks();
		showEnd = true;
	}
	
	CCObject *obj;
	CCARRAY_FOREACH_REVERSE(Block::getBlocksArray(), obj)
	{
		Block * b = (Block *)obj;
		b->moveDownAndCleanUp();
	}
}

void LayerGame::startTimer()
{
	if (!isRunning)
	{
		startTime = clock();
		scheduleUpdate();
		isRunning = true;
	}
}

void LayerGame::stopTimer()
{
	if (isRunning)
	{
		unscheduleUpdate();
	}
}

void LayerGame::update(float dt)
{
	long off = clock() - startTime;
	CCString *str = CCString::createWithFormat("%g", (double)off / 1000);
	ttf->setString(str->getCString());
}

void LayerGame::keyBackClicked(void)
{
	CCDirector::sharedDirector()->end();
}
