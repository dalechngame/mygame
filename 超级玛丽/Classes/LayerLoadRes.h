#ifndef __LayerLoadRes_H__
#define __LayerLoadRes_H__

#include "Common.h"

//! 封装进度条
class LayerLoadRes : public CCLayer
{
public:
	static LayerLoadRes* create(const char* bgFile, const char* bgBarFile, const char* foreFile);
	bool init(const char* bgFile, const char *bgBarFile, const char* foreFile);

	void setPercentage(float per){ _bar->setPercentage(per); }
	float getPercentage(){ return _bar->getPercentage(); }

	void setPosition(float x, float y)
	{
		_bar->setPosition(ccp(x, y));
		_bgBar->setPosition(ccp(x, y));
	}
private:
	enum ZORDER{BG=100, BGBAR= 200, BAR=300};
	CCSprite* _bg;
	CCSprite *_bgBar;
	CCProgressTimer* _bar;
};

#endif
