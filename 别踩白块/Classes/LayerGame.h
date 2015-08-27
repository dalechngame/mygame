#ifndef __LayerGame_H_#define __LayerGame_H_
#include "cocos2d.h"
USING_NS_CC;

class LayerGame :public CCLayer
{
public:
	static CCScene *scene();
	CREATE_FUNC(LayerGame);
	bool init();

	//���Blocks
	void startGame();
	void addStartLineBlocks();
	void addEndLineBlocks();
	void addNormalBlocks(int lineIndex);

	//��¼�Ƿ��˽�β
	bool showEnd;
	//��ʱ�����
	int startTime;
	bool isRunning;
	void startTimer();
	void stopTimer();
	void update(float dt);


	//��ʾʱ��
	CCLabelTTF * ttf;

	//��¼block�ĸ���(������)
	CC_SYNTHESIZE(int, _lineCount, LineCount);

	//��ʼ��ť�ص�
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) override ;
	void moveDown();
	void keyBackClicked(void);

};

#endif
