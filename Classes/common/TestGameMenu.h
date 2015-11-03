// Created by wangwenjie on 2015/01

#ifndef __TESTGAMEMENU_H__
#define __TESTGAMEMENU_H__

#include "LBLibraryBase.h"
#include "Common.h"

#include "cocos2d.h"

class TestGameMenu : public WJLayer
{
private:
    void onMenuItemClick(Ref* node, GameNumber gameNumber);
	void onMenuItemBackClick(Ref* node);

public:
	virtual bool init() override;

	static void show();
};

#endif // __TESTGAMEMENU_H__
