#ifndef __LayerControl_H__  		
#define __LayerControl_H__  		
#include "cocos2d.h"  			

USING_NS_CC;  				

class LayerControl :public CCLayer 
{  					
public: 					
	CREATE_FUNC(LayerControl); 		
	bool init();  				

	void menuCallBack(CCObject *obj);
	void updateScore(int score);
	void updateHeroLife(int life);

private:
	CCMenuItemSprite *m_ppauseMenuItem;
	CCLabelBMFont *m_pscoreItem;
	CCLabelBMFont *m_pheroLife;
};  	

#endif  					
