// Created by wangwenjie on 2013/04

#ifndef __ZOOM_LAYER__
#define __ZOOM_LAYER__

#include "cocos2d.h"
#include "WJLayerJson.h"
#include "WJLayerJson2x.h"

USING_NS_CC;

class ZoomLayer : public WJLayerJson2x
{
private:

	Size m_winSize;
	Point m_origion;

	// 移动到最外面
	bool m_moveOutSide;

	// 两点之间的距离
	double m_twoTouchPointLen;
	// 多点触摸
	virtual void touchesBegan(const std::vector<Touch*>& pTouches, Event *pEvent) override;
	virtual void touchesMoved(const std::vector<Touch*>& pTouches, Event *pEvent) override;
	virtual void touchesEnded(const std::vector<Touch*>& pTouches, Event *pEvent) override;

	bool checkSceneAllIn(float scale);

	// 单点触摸事件
	virtual bool doEventTouchAble(Node* node, WJTouchEvent* e) override;
	virtual bool doEventWillMoveTo(Node* node, WJTouchEvent* e) override;
	virtual void doEventTouchEnded(Node* node, WJTouchEvent* e) override;

	// 当前layer是否在屏幕里面
	bool checkLayerInView(const Point &point, float scale);

	float getMinXFromScale();
	float getMaxXFromScale();
	float getMaxYFromScale();
	float getMinYFromScale();

	bool scaleNodeFromZoom(float len);

public:

	ZoomLayer();

	static ZoomLayer* create(const char *jsonFileName);

	virtual bool initFromJsonFile(const char *jsonFileName);
};
#endif // __ROOM_SCENE_H__
