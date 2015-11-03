
/*项目当中的共通效果和处理事件*/

#ifndef __PUTILS_H__
#define __PUTILS_H__

#include "cocos2d.h"
#include <stdlib.h>
#include "LBLibraryBase.h"

USING_NS_CC;

using namespace std;

#define SHOW_SCALE_ACTION_TAG                  1012               // 缩放提示
#define NODE_BOUNCE_ACTION_TAG                 1013              // 滚动子项q弹
#define NODE_RUN_WAVE_ACTION_TAG               1136             // 节点做旋转的动画

#define GAME_ITEM_MOVE_SPEED                   900              // 小游戏里面物件的移动速度

#define LOGPOINTXY(__posX__, __posY__) CCLOG("x =  %f, y = %f", __posX__, __posY__);
#define LOGPOINT(__pos__) CCLOG("x =  %f, y = %f", __pos__.x, __pos__.y);

typedef std::function<void(Node* node)> ActionEndCallback;

class PUtils
{
	
public:
	/* -------------------- 物品的Q弹效果 ------------------------- */
	/* 返回值为效果播放的总时间 */
	static float playItemBounceEffect(Node* pNode, float time = 0.15f);
	static float playItemBounceEffect(Node* pNode, int num, float time = 0.15f);//播放默认的次数
	// 气泡的呼吸效果
	static void playBubbleEffect(Node *node);
	// 气泡呼吸效果，保持当前的缩放比例
	static void playBubbleEffectWithCurrentScale(Node* node);
	// 物体悬浮的效果
	static void playSuspenEffect(Node *node);
	// 延迟播放物体悬浮的效果
	static void playSuspenEffect(Node *node, float delay);
	// 缩放的提示
	static void playNodeScaleAni(Node *node, int time = 1, float delayTime = 1.5f);
	// 循环做缩放提示
	static void playNodeScaleAniForever(Node *node, float delayTime);
	static void playNodeScaleAniForeverFirst(Node *node, float delayTime);
	// 停止缩放的效果
	static void stopNodeScaleAction(Node *node);
	// 旋转摆动
	static void playNodeWaveEffect(Node *node);
	// 循环提示的效果
	static void playNodeCircleTipForever(Node *node, float delayTime = 5.0f, float interval = 0.3f);

	static Action* delayPlayFunc(Node *node, CallFunc *call, float time = 0.3f);

	/* -------------------------------------------------------------- */
public:
	/**---------------------------碰撞检测-----------------------------*/
	static bool isOneCollideWithOne(Vec2 src, Vec2 des, float raduis = 20.0f);
	static bool isOneCollideWithAnotherOne(Node *src, Node*des, float raduis = 20.0f);
	static bool isOneCollideWithOne(Node *src,
		Node *des,
		bool isPointCheck = false,
		Vec2 srcOffset = Vec2::ZERO,
		Vec2 desOffset = Vec2::ZERO);
	static bool isOneCollideWithOne(Vec2 src,
		Node *des,
		bool isRectCheck = false,
		float halfWidth = 5.0f,
		float halfHeight = 5.0f);
	static float GetDistance(Node* _node1, Node* _node2);
	static bool isOneCollideWithOne(Node *src, Node*dest, float x, float y);
	static bool isOneCollideWithOne(Vec2 src, Node*dest, float offsetX, float offsetY, float x, float y);

	/* -------------------------------------------------------------- */

public:
	// 检查当前图片上面对应的点是否透明
	static bool getPointImageIsTransparent(Image *m_textureImage, const Vec2 &pointImage);

	//获取空白像素的比例
	static float getImgBlankspace(Image* image);
public:
	/* -------------------- 物品移动相关 ------------------------- */
	// 移动节点到世界坐标系的一个目的点，默认会停掉所有的运动
	// 移动开始禁止node以及所有的子节点可用性，移动结束后恢复
	static void runActionMoveTo(Node *node, const Vec2 &pointWorld, bool enableMoving = false, bool stopAllAction = true);
	// 移动完了之后的回调函数
	static void runActionMoveToWithCallBack(Node *node, const Vec2 &pointWorld, const ActionEndCallback &callback = nullptr,
		bool enableMoving = false, bool stopAllAction = true);
	/* -------------------------------------------------------------- */

public:
	/*-----------------------坐标的转换相关------------------------------*/
	static Vec2 getPositionInWorldFromPoint(Node *node, const Vec2 &point);
	/* -------------------------------------------------------------- */

public:
	// 给一个wjbase添加事件处理
	static void addListener(WJBase* base, const WJTouchBoolCallback &touchAble,
		const WJTouchBoolCallback &willMoveto, const WJTouchCallback &touchEnd,
		const WJTouchCallback &click);
	// 通过key添加多个事件(key 必须是字符串加上三位数字)
	static void addListenerWithKey(WJLayerJson *json, const char * key, int fromint, int endInt,
		const WJTouchBoolCallback &touchAble,
		const WJTouchBoolCallback &willMoveto, 
		const WJTouchCallback &touchEnd,
		const WJTouchCallback &click);

	// 设置当前节点在json里面对应的阴影的可见性
	static void setNodeShadowVisible(WJLayerJson *json, Node *node, bool visible);

	// 讲一个节点addchild到另一个节点
	static void addWJChildToAnotherParent(cocos2d::Node *child, cocos2d::Node*newParent);

	// 随即获取vector中的节点
	template<typename type>
	static type getRandomChildOfVector(std::vector<type>& vec)
	{
		if (vec.size() == 0)
		{
			return 0;
		}
		int n = WJUtils::randomInt(vec.size());

		type child = vec.at(n);
		return child;
	}

	// 删除vec中的元素
	template<typename type>
	static void removeChildOfVectorByIndex(const vector<type>& vec, int index)
	{
		int i = 0;
		for (typename vector<type>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if (i == index)
			{
				vec.erase(it);
				break;
			}
			i++;
		}
	}

	template<typename type>
	static void removeChiledOfVector(vector<type>& vec, type child)
	{
		for (typename vector<type>::iterator it = vec.begin(); it != vec.end(); it++) 
		{
			if (child == *it)
			{
				vec.erase(it);
				break;
			}
		}
	}

public:
	/*------------------------------播放粒子效果相关----------------------------------------*/
	static void playParticle(const char * fileName, Node *parent, const Vec2 &pointParent, int zOrder, int tag = -1);
	/* ------------------------------------------------------------------------------------ */

};

#endif // __PUTILS_H__