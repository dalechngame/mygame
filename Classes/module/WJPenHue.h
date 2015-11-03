
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

	// ����index,��0��ʼ
	CC_SYNTHESIZE(int, m_penIndex, PenIndex);
	// �Ƿ���Ըı䵱ǰ���ʵ�ɫ��ֵ(Ĭ�Ͽ��Ըı�)
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

	// ���ӵ�ǰ���ʵ�hue
	void addPenHue(GLfloat addHue = 0.25f);
	// ���õ�ǰ���ʶ�Ӧ�����ɫ�ࡢ�ı���ɫ
	void setPenHue(GLfloat hue);

	GLfloat getPenHue();

};


#endif // __WJPEN_HUE_H__
