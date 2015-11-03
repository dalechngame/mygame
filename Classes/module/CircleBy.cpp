#include "CircleBy.h"

CircleBy* CircleBy::createWithDuration(float f, const  EllipseConfig& c)
{
	auto pCircleBy = new (std::nothrow) CircleBy();
	pCircleBy->initWithDuration(f, c);
	pCircleBy->autorelease();

	return pCircleBy;
}

bool CircleBy::initWithDuration(float f, const EllipseConfig& c)
{
	if (ActionInterval::initWithDuration(f))
	{
		m_eConfig = c;
		return true;
	}

	return false;
}

void CircleBy::update(float elpase)
{
	if (_target)
	{
		Vec2 s_startPosition = m_eConfig.centerPositon;
		float a = m_eConfig.aLength;
		float bx = m_eConfig.centerPositon.x;
		float by = m_eConfig.centerPositon.y;
		float c = m_eConfig.cLength;
		float x = ellipseXat(a, bx, c, elpase);
		float y = ellipseYat(a, by, c, elpase);
		_target->setPosition(s_startPosition + Vec2(a - x, y));
	}
}