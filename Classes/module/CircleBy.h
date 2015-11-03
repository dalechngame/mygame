/**************************************************************************
Copyright(c), 2015-2016,LibiiTech.Co.,Ltd.

FileName: D:\cocos2d-x-2.2.3\cocos2d-x-2.2.3\LibiiSVN\86_PrincessVacation\Classes\tool\CircleBy.h

ProjectName: PrincessVacation

Author: PJoe  Version: 1.0  Date: 2015/6/24

Description:

FunctionList:

History:
***************************************************************************/

#ifndef __CircleBy_h__
#define __CircleBy_h__
#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
#define PI 3.1415927

struct EllipseConfig
{
	Vec2 centerPositon;
	float aLength;
	float cLength;
};


class CircleBy: public ActionInterval
{
public:
	bool initWithDuration(float f, const EllipseConfig& c);
	virtual void update(float elpase);

	static CircleBy* createWithDuration(float f, const  EllipseConfig& c);
	static inline float ellipseXat(float a, float bx, float c, float t)
	{
		return -a * cos(2*PI*t) + a;
	}

	static inline float ellipseYat(float a, float by, float c, float t)
	{
		float b = sqrt(powf(a, 2) - powf(c, 2));
		return (-b) * sin(2*PI*t);
	}
	
protected:
	EllipseConfig m_eConfig;
	Vec2 m_startPosition;
	Vec2 s_startPosition;
};

#endif //define __CircleBy_h__
