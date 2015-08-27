#include "SceneTransition.h"  			

CCScene * SceneTransition::scene() 		
{ 						
	CCScene * scene = CCScene::create(); 	
	SceneTransition * layer = SceneTransition::create(); 
	scene->addChild(layer); 		
	return scene; 				
} 						

bool SceneTransition::init() 			
{ 						
	CCLayer::init(); 			

	return true;				
} 						
