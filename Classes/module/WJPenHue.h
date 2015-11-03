
#ifndef __WJPEN_HUE_H__
#define __WJPEN_HUE_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "SpriteWithHue.h"

USING_NS_CC;

class WJPenHue :public WJPen {
private:

	GLfloat m_hue;

	SpriteWithHue *m_spriteWithHue;

	Color4B m_colorHue;

	// 画笔index,从0开始
	CC_SYNTHESIZE(int, m_penIndex, PenIndex);
	// 是否可以改变当前画笔的色相值(默认可以改变)
	CC_SYNTHESIZE(int, m_hueChangeAble, HueChangeAble);

	bool _drawHueLine(const Vec2 &pointFrom, const Vec2 &pointTo);

	virtual void drawLine(const Vec2 &pointFrom, const Vec2 &pointTo) override;

	void _visitHueSprite(const Vec2 &position, const Vec2 &startPoint, const Vec2 &endPoint);

	virtual WJPen* clone() override;

public:
	WJPenHue();
	~WJPenHue();

	bool init(const char* penImageFile);
	static WJPenHue* create(const char* penImageFile);

	// 增加当前画笔的hue
	void addPenHue(GLfloat addHue = 0.25f);
	// 设置当前画笔对应精灵的色相、改变颜色
	void setPenHue(GLfloat hue);

	GLfloat getPenHue();

};


#endif // __WJPEN_HUE_H__
