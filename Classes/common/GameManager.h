// Created by yuanzhongji on 2014/05

#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "cocos2d.h"
#include "LBLibraryBase.h"
#include "ConfigFoolHead.h"
#include "GameCommand.h"
#include "vector"
#include "PConfig.h"

USING_NS_CC;

/* 换装物品的分类 */
#define HAIR_CATEGORY		0
#define CROWN_CATEGORY		1
#define DRESS_CATEGORY		2
#define GLOVE_CATEGORY		3
#define CAPPA_CATEGORY		4
#define ACCESSORY_CATEGORY	5

class GameManager : public Ref
{
public:

	// 当前游戏在进入场景后需要执行的命令
	Vector<GameCommand*> commandVector;
	
	GameManager();
	~GameManager();

	virtual bool init();
	/** returns a shared instance of the GameManager */
	static GameManager* getInstance(void);

public:
	//保存图片的路径
	static std::string getAtlasSavePath();
	// 寻找当前的序列中还没有图片的key
	static std::string findNoneSavePhotosKey();
	// 取得保存的展示图片的图片名称
	static std::string getShowPhotosFileFromIndex(int index);
	// 取得右小角保存的图片的名称
	static std::string getSavePhotosFileFromIndex(int index);
	// 当前的保存图片移动到展示图片集中
	bool putSavePhotosIntoShowPhotos(int saveIndex, int showIndex);

	static int getShowPhotosFileNum();
	static int getSavePhotosFileNum();
	// 图片相册里面是否存在照片
	static bool exitPhotosWithAlbumicon();

public:

	// 添加处理命令,添加进去的按照添加进去的顺序执行
	void pushGameCommandIntoGame(GameCommand *command, bool cleanAll = false);
	// 是否存在需要处理的命令
	bool isExistGameCommand();
	//// 清除所有的命令
	void cleanAllGameCommandFromVector();

public:

	// P005 相关
	std::string getFileFromUserSavedWithTypeAndIndex(DiySaveType type, int nameIndex);

public:

	// 换装相关
	void setMakeUpItemKey(const char * name, const char * key);
	std::string getMakeUpItemKeyFromName(const char * name);

public:

	// 释放内存
	static void removeUnusedDataAndTexture();

};

#define GManager GameManager::getInstance()

#endif // __GAME_MANAGER_H__
