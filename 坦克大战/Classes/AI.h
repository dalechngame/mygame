
#ifndef __AI_H__
#define __AI_H__

#include "Common.h"

class AI : public CCNode
{
public:
	CREATE_FUNC(AI);
	bool init();
	void onExit();

	CCTMXTiledMap* getMap()
	{
		return (CCTMXTiledMap*)getParent();
	}

	bool _createFlag;
	bool	_pause;

	CCArray* _tanks;
	CCArray* _bullets;

	// �����о�̹��
	void createTank(float);
	// �ƶ��о�̹��
	void moveTank(float);
	// �õо�̹�˷����ӵ�
	void Shoot(float);
	// ��ײ���
	void update(float);

	void resume(float);
	void pause();
};

#endif

