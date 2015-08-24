
#ifndef __LayerStart_H__
#define __LayerStart_H__
#include "Common.h"

class LayerStart : public CCLayer
{
public:
	CREATE_FUNC(LayerStart);
	bool init();

	//! 开始界面
	void Start(CCObject*);
	void Setup(CCObject*);
	void Quit(CCObject*);
	void About(CCObject*);
};

#endif