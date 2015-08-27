#ifndef __SceneTransition_H__  		
#define __SceneTransition_H__  		

#include "common.h"
	
class SceneTransition :public CCLayer 
{  					
public: 					
	static CCScene * scene();		

	CREATE_FUNC(SceneTransition); 		
	bool init();  				

};  					
#endif  					
