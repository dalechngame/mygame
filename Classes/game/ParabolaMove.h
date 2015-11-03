#include "cocos2d.h"

USING_NS_CC;

class ParabolaMove : public ActionInterval
{
protected:
	float m_vx0 ;
	float m_vy0 ;
	Vec2 m_startPosition;
	Vec2 m_endPosition;
	float angle;
	float m_angle;
	float m_dur;
	float m_tan_a;
	float	m_g;

public:
	static ParabolaMove* create(const Vec2& startPosition, const Vec2& endPosition, float angle, float g, float &return_duration);
	bool initWithDuration(float duration, const Vec2& startPosition, const Vec2& endPosition, float angle, float g, float vx0, float vy0);

	void update(float t);

};
