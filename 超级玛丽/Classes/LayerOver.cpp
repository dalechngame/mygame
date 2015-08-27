#include "LayerOver.h"  			

CCScene * LayerOver::scene(Common::OVERTYPE type)
{ 						
	CCScene * scene = CCScene::create(); 	
	LayerOver * layer = LayerOver::create(type); 
	LayerOver *l = dynamic_cast<LayerOver *>(new CCLayer);
	scene->addChild(layer); 		
	return scene; 				
} 						

bool LayerOver::init(Common::OVERTYPE type)
{ 						
	CCLayer::init(); 			
	CCSprite *spr = CCSprite::create("bg.png");
	spr->setAnchorPoint(ccp(0, 0));
	addChild(spr);
	

	return true;		
} 						

void LayerOver::onEnterTransitionDidFinish()
{

}
