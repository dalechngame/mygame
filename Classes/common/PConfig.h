// 定义了一些常量
#ifndef __P_CONFIG_H__
#define __P_CONFIG_H__

#include "cocos2d.h"

enum class DiySaveType
{
	P002_Dreamcatcher = 1,        // 捕梦网
	P003_Snowman,                // 雪人
	P004_Sandbottle,             // 沙画瓶
	P006_Garland,               // 花环
	P011_Coloring,              // 涂色
	P012_Sugar,                 // 糖球
	other                       // 其他
};

// 保存图片相关的前缀
static const char * SAVE_PHOTO_PREFIX = "photos";
// 展示图片相关的前缀
static const char * SHOW_PHOTO_PREFIX = "showPhotos";
// 保存的图片的最大的值
static const int SAVE_PHOTO_MAX = 6;
// 空白图片
static const char * BASE_NONE_PNG = "base/none.png";
// 指甲应用到全部
static const char * NAIL_APPLY_TO_ALL_MESSAGE = "Are you sure you want to apply this design to all nails？";

// 滚动条自动处理cancel的index
static const int AUTO_DEAL_CANCEL_START_INDEX       =  0;
static const int AUTO_DEAL_CANCEL_END_INDEX         = -1;


enum class GuestModelNo
{
	princessLibii = 1,
	princeWhite,
	princeBlack,
	princessAsian,
	princessBlack,
	princessWhite,
	petDog,
	petCat
};


// 计时器
template <typename T>
struct Counter
{
	T start = 0;
	T current = 0;
	
	// when T is long long
	bool isInInterval(long long time)
	{
		current = WJUtils::millisecond();

		return current - start <= time;
	}
};

#endif