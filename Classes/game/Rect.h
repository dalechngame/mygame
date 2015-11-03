#ifndef __VISIBLERECT_H__
#define __VISIBLERECT_H__

#include "cocos2d.h"

class VisibleRect
{
public:
    static cocos2d::Rect getVisibleRect();

    static cocos2d::Vec2 left();
    static cocos2d::Vec2 right();
    static cocos2d::Vec2 top();
    static cocos2d::Vec2 bottom();
    static cocos2d::Vec2 center();
    static cocos2d::Vec2 leftTop();
    static cocos2d::Vec2 rightTop();
    static cocos2d::Vec2 leftBottom();
    static cocos2d::Vec2 rightBottom();
private:
    static void lazyInit();
    static cocos2d::Rect s_visibleRect;
};

#define Left VisibleRect::left()
#define Right VisibleRect::right()
#define Top VisibleRect::top()
#define Bottom VisibleRect::bottom()
#define Center VisibleRect::center()
#define LeftTop VisibleRect::leftTop()
#define RightTop VisibleRect::rightTop()
#define LeftBottom VisibleRect::leftBottom()
#define RightBottom VisibleRect::rightBottom()

#endif /* __VISIBLERECT_H__ */
