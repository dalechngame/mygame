#ifndef __LayerGame_H_#define __LayerGame_H_
#include "cocos2d.h"
USING_NS_CC;

class LayerGame :public CCLayer
{
public:
	static CCScene *scene();
	CREATE_FUNC(LayerGame);
	bool init();

	//添加Blocks
	void startGame();
	void addStartLineBlocks();
	void addEndLineBlocks();
	void addNormalBlocks(int lineIndex);

	//记录是否到了结尾
	bool showEnd;
	//定时器相关
	int startTime;
	bool isRunning;
	void startTimer();
	void stopTimer();
	void update(float dt);


	//显示时间
	CCLabelTTF * ttf;

	//记录block的个数(多少行)
	CC_SYNTHESIZE(int, _lineCount, LineCount);

	//开始按钮回调
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) override ;
	void moveDown();
	void keyBackClicked(void);

};

#endif
