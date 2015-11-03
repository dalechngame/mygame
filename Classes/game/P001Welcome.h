#ifndef __P001_SCENE_H__
#define __P001_SCENE_H__

#include "PBase.h"

/**
 * P001 Welcome 场景
 */
class P001 : public PBase
{
protected:
	LBToolbar *m_toolbar;

	virtual GameNumber getGameNumber() override;

	virtual void initSceneJson(const char* jsonFileName) override;
	void initToolbar();
	void onToolbarClick(Node* node, WJTouchEvent* evnet);

	void testSkeleton();
	void testSprite3D();
	void testCanvas();
    
    RenderTexture* m_render;
    void testShader();

	bool m_test3dRotate;
	void test3dRotate();

protected:
	virtual void onStoreClosedAndPurchased() override;

public:
	virtual bool init() override;  

	virtual void onEnter() override;
	virtual void onEnterTransitionDidFinish() override;
	virtual void onExit() override;
    
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags) override;

public:
	CREATE_SCENE_FUNC(P001);

	static void asyncLoadSceneResource();
};

#endif // __P001_SCENE_H__
