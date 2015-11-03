//
//  ArrowPathBezier.h
//  
//
//  Created by dalechn on 15/10/10.
//
//

#ifndef ____ArrowPathBezier__
#define ____ArrowPathBezier__

#include "cocos2d.h"
using namespace cocos2d;

typedef struct _ccQuadBezierConfig
{
    Point endPosition;
    Point controlPoint;
} ccQuadBezierConfig;

class ArrowPathBezier: public BezierBy
{
public:
    static ArrowPathBezier* create(float t, const ccQuadBezierConfig& c);
    bool initWithDuration(float t, const ccQuadBezierConfig& c);
    virtual void update(float time);
    
private:
    ccQuadBezierConfig _config;
};

#endif /* defined(____ArrowPathBezier__) */
