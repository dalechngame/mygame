// Created by yuanzhongji on 2015/10

#ifndef __ScrollItem_Diy_H__
#define __ScrollItem_Diy_H__

#include "cocos2d.h"
#include "ScrollItem.h"
#include "TipDialog.h"

class ScrollDiyItem : public ScrollItem
{
protected:

	// 是否需要玩游戏
	CC_SYNTHESIZE(bool, m_needPlayGame, NeedPlayGame);
	// 当前用户保存的图片的类型
	CC_SYNTHESIZE(DiySaveType, m_fileSaveType, FileSaveType);
	// 人物已经移动进入了场景
	CC_SYNTHESIZE(bool, m_modelIsMoveScene, ModelIsMoveScene);

	// 加锁前的逻辑判断
	virtual bool checkItemUsedAbleBeforeCheckLocked(Node *node, WJTouchEvent *event, WJScrollLayer *scroll) override;
	// 判断是否可以移动
	virtual bool checkItemMovingAbleWhenMoving(Node *node, WJTouchEvent *event) override;

	virtual void setModelIntoMoveScene(bool move);

	// 显示需要玩游戏弹窗
	virtual void showDiyPopDialog();
	// 点击了确定
	virtual void onClickShowDiyPopOkButton(TipDialog *dialog);

public:

	ScrollDiyItem();

	CREATE_SCROLLITEM_FUNC(ScrollDiyItem);

	virtual bool init(ScrollLayer *scrollLayer, const char * key, int index, ScrollDragDir drag, bool clickAble) override;

    virtual void onEnterTransitionDidFinish();

	virtual void onEnter();
	virtual void onExit();

};
#endif // __ScrollItem_Diy_H__
