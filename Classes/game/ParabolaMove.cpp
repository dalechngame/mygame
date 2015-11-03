#include "ParabolaMove.h"
#define PIXELS_PER_METER 100

ParabolaMove* ParabolaMove::create(const Vec2& startPosition, const Vec2& endPosition, float angle, float g, float &return_duration)
{
	ParabolaMove *pRet = new ParabolaMove();
	float vx0, vy0, x1, y1;

	//���ص��׵�ת����PIXELS_PER_METERΪת��ϵ������������Ϊ100
	x1 = (endPosition.x - startPosition.x) / PIXELS_PER_METER;
	y1 = (endPosition.y - startPosition.y) / PIXELS_PER_METER;
	angle = angle*3.14 / 180;		//convert t to radian

	if (x1 < 0)
	{
		angle *= -1;
	}
	
	vx0 = x1*sqrt(g / 2 / (x1*tan(angle) - y1)); //������ٶ�
	vy0 = vx0 * tan(angle);

	return_duration = x1 / vx0;				 //��durationͨ����������

	pRet->initWithDuration(return_duration, startPosition, endPosition, angle, g, vx0, vy0);
	pRet->autorelease();

	return pRet;
}

bool ParabolaMove::initWithDuration(float duration, const Vec2& startPosition, const Vec2& endPosition, float angle, float g, float vx0, float vy0)
{
	if (ActionInterval::initWithDuration(duration))
	{
		m_vx0 = vx0;
		m_vy0 = vy0;
		m_startPosition = startPosition;
		m_endPosition = endPosition;
		angle = angle*3.14 / 180;//convert t to radian
		m_angle = angle;
		m_dur = duration;
		m_tan_a = tan(angle);
		m_g = g;

		return true;
	}

	return false;
}

void ParabolaMove::update(float t)
{
	if (_target)
	{
		float elapsed = t * _duration; //��õ�ǰ���˶�ʱ��
		float diff_x = m_vx0 * elapsed;
		float diff_y = m_vy0 * elapsed - 0.5 * m_g * elapsed * elapsed;

		Vec2 newPos = ccpAdd(m_startPosition, ccp(diff_x * PIXELS_PER_METER, diff_y * PIXELS_PER_METER)); //��λ����ת��Ϊ����
		_target->setPosition(newPos);

	}
}