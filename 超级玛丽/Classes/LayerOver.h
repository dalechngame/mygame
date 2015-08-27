#ifndef __LayerOver_H__  		
#define __LayerOver_H__  		

#include "common.h"

class LayerOver :public CCLayer 
{  					
public: 					
	static CCScene * scene(Common::OVERTYPE type);
	static LayerOver *create(Common::OVERTYPE type)
	{
		LayerOver *layer = new LayerOver;
		layer->init(type);
		layer->autorelease();

		return layer;
	}

	void onEnterTransitionDidFinish();

	bool init(Common::OVERTYPE type);
};  					
#endif  					
