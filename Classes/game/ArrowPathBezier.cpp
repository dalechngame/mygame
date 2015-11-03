//
//  ArrowPathBezier.cpp
//  
//
//  Created by dalechn on 15/10/10.
//
//

#include "ArrowPathBezier.h"

static int rotation = 0;

static inline float bezierat1( float a, float b, float d, float t )
{
    return (powf(1-t,2) * a +
            2.0f*t*(1-t)*b +
            powf(t,2)*d );
}

ArrowPathBezier* ArrowPathBezier::create(float t, const ccQuadBezierConfig& c)
{
    ArrowPathBezier *pBezierBy = new (std::nothrow) ArrowPathBezier();
    pBezierBy->initWithDuration(t, c);
    pBezierBy->autorelease();
    
    return pBezierBy;
}

bool ArrowPathBezier::initWithDuration(float t, const ccQuadBezierConfig& c)
{
    bool bRet = false;
    
    if (CCActionInterval::initWithDuration(t))
    {
        _config = c;
        return true;
    }
    return bRet;
}

void ArrowPathBezier::update(float time)
{
    // 1
    if (_target) {
        // 2
        float xa = 0;
        float xb = _config.controlPoint.x;
        float xc = _config.endPosition.x;
        
        float ya = 0;
        float yb = _config.controlPoint.y;
        float yc = _config.endPosition.y;
        
        float x = bezierat1(xa, xb, xc, time);
        float y = bezierat1(ya, yb, yc, time);
        
#if CC_ENABLE_STACKABLE_ACTIONS
        // 3
        Vec2 currentPos = _target->getPosition();
        Vec2 diff = currentPos - _previousPosition;
        _startPosition = _startPosition + diff;
        
        Vec2 newPos = _startPosition + Vec2(x,y);
        _target->setPosition(newPos);
        
        _previousPosition = newPos;
		rotation -= 10;
		_target->setRotation(rotation);
        // 4
		/*     if (0 != time) {
				 Vec2 vector = currentPos - newPos;
				 float radians = vector.getAngle();
				 float degrees = CC_RADIANS_TO_DEGREES( -1*radians);
				 _target->setRotation(degrees-180);
				 }*/
        
#else
        _target->setPosition( _startPosition + Vec2(x,y));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
        
    }
}