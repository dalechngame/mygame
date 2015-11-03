
#ifndef __DailyBonus_H__
#define __DailyBonus_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "TipDialog.h"


class DailyBonus : public TipDialog
{
private:
	
	bool m_completed;

    static int checkDays();
    static int getDays();

    int m_days;

    static DailyBonus *create(int days);

    //void show(Node *owner, int zOrder);

	void setNodeColorAndUsedAble();
	void onClickDaySprite(Node *node, WJTouchEvent *event);
	void onPlayGiftAniTimer(float time);

public:

	DailyBonus();

    bool init(int days);
	
    static bool showItIfNeed(Node *owner, int zOrder);

    virtual void onEnterTransitionDidFinish();
};

#endif
