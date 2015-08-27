#ifndef __layerGameStart_H__  		
#define __layerGameStart_H__  	

#include "cocos2d.h"  			
USING_NS_CC;  			

class layerGameStart :public CCLayer 
{  					
public: 					
	static CCScene * scene();		
	CREATE_FUNC(layerGameStart); 		
	bool init();  				

	//! Ԥ����
	void addStartGamePicture();
	void addPreLoadMusic();

	//! ����������
	void toMainGameCallback();
};  				

#endif  					
