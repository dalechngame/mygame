// Created by yuanzhongji on 2014/05

#include "GameManager.h"
#include "Common.h"

// singleton stuff
static GameManager *s_SharedManager = NULL;

GameManager* GameManager::getInstance(void)
{
	if (!s_SharedManager)
	{
		s_SharedManager = new GameManager();
		s_SharedManager->init();
	}

	return s_SharedManager;
}

bool GameManager::init()
{
	return true;
}

GameManager::GameManager()
{

}

GameManager::~GameManager()
{

}

std::string GameManager::getAtlasSavePath()
{
	return WJUtils::getWritableFolderPath();
}

std::string GameManager::findNoneSavePhotosKey()
{
	for (int i = 1; i <= SAVE_PHOTO_MAX; i++)
	{
		std::string key = WJUtils::stringAddInt(SAVE_PHOTO_PREFIX, i, 1);
		std::string fileName = UserDefault::getInstance()->getStringForKey(key.c_str(), "");
		if (!WJUtils::equals(fileName.c_str(), ""))
		{
			std::string fullpath = GameManager::getAtlasSavePath() + fileName;
			if (!WJUtils::fileExists(fullpath.c_str()))
				return key;
		}
		else
		{
			return key;
		}
	}

	return "";
}

std::string GameManager::getShowPhotosFileFromIndex(int index)
{
	return UserDefault::getInstance()->getStringForKey(WJUtils::stringAddInt(SHOW_PHOTO_PREFIX, index, 1).c_str(), "");
}

std::string GameManager::getSavePhotosFileFromIndex(int index)
{
	return UserDefault::getInstance()->getStringForKey(WJUtils::stringAddInt(SAVE_PHOTO_PREFIX, index, 1).c_str(), "");
}

int GameManager::getShowPhotosFileNum()
{
	// 一共显示6张
	int fileNum = 0;
	for (int i = 1; i <= 6; i++)
	{
		std::string fileName = getShowPhotosFileFromIndex(i);
		std::string fullpath = GameManager::getAtlasSavePath() + fileName;
		//if (WJUtils::fileExists(fullpath.c_str()))
        if (!WJUtils::equals("", fileName.c_str()))
		{
			fileNum++;
		}
	}

	return fileNum;
}

int GameManager::getSavePhotosFileNum()
{
	// 一共显示6张
	int num = 0;
	for (int i = 1; i <= 6; i++)
	{
		std::string fileName = getSavePhotosFileFromIndex(i);
		std::string fullpath = GameManager::getAtlasSavePath() + fileName;
        if (!WJUtils::equals("", fileName.c_str()))
		//if (WJUtils::fileExists(fullpath.c_str()))
		{
			num++;
		}
	}

	return num;
}

bool GameManager::exitPhotosWithAlbumicon()
{
    return GameManager::getShowPhotosFileNum() + GameManager::getSavePhotosFileNum() >= 1;
}

bool GameManager::putSavePhotosIntoShowPhotos(int saveIndex, int showIndex)
{
	// 当前保存的照片的检查
	std::string newFile = UserDefault::getInstance()->getStringForKey(WJUtils::stringAddInt(SAVE_PHOTO_PREFIX, saveIndex, 1).c_str(), "");
	std::string fullNewPath = GameManager::getAtlasSavePath() + newFile;
	if (!WJUtils::fileExists(fullNewPath.c_str()))
		return false;

	// 先移除之前的展示的照片
	std::string oldFile = getShowPhotosFileFromIndex(showIndex);
	if (!WJUtils::equals(getShowPhotosFileFromIndex(showIndex).c_str(), ""))
	{
		std::string fullpath = GameManager::getAtlasSavePath() + oldFile;
		if (WJUtils::fileExists(fullpath.c_str()))
			FileUtils::getInstance()->removeFile(fullpath);
	}

	// 保存最新的照片到当前的展示序列中
	UserDefault::getInstance()->setStringForKey(WJUtils::stringAddInt(SHOW_PHOTO_PREFIX, showIndex, 1).c_str(), newFile);
	UserDefault::getInstance()->flush();

	// 当前保存的照片清空
	UserDefault::getInstance()->setStringForKey(WJUtils::stringAddInt(SAVE_PHOTO_PREFIX, saveIndex, 1).c_str(), "");
	UserDefault::getInstance()->flush();

	return true;
}

void GameManager::pushGameCommandIntoGame(GameCommand *command, bool cleanAll)
{
	if (cleanAll)
		commandVector.clear();

	commandVector.pushBack(command);

	if (commandVector.size() >= 2)
	{
		GameCommand *lastCommand = commandVector.at(commandVector.size() - 2);
		lastCommand->setNextCommand(command);
	}
}

bool GameManager::isExistGameCommand()
{
	return !commandVector.empty();
}

void GameManager::cleanAllGameCommandFromVector()
{
	commandVector.clear();
}

void GameManager::setMakeUpItemKey( const char * name, const char * key )
{
	std::string saveName = "makeup";
	UserDefault::getInstance()->setStringForKey(saveName.append(name).c_str(), key);
	UserDefault::getInstance()->flush();

	removeUnusedDataAndTexture();
}

void GameManager::removeUnusedDataAndTexture()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	LOGD("------------- LOADING: win32/mac don't release unused resources ------------- ");
#else
	//LOGD("------------- LOADING: release unused resources. ------------- ");

	// release skeleton data cache
	WJSkeletonDataCache::getInstance()->removeUnusedSkeletonData();

	// release snapsnot
	WJUtils::releaseLastScreenShot();

	// last, release textures
	Director::getInstance()->purgeCachedData();

#endif
}

std::string GameManager::getMakeUpItemKeyFromName( const char * name )
{
	std::string saveName = "makeup";
	return UserDefault::getInstance()->getStringForKey(saveName.append(name).c_str(), "");
}

std::string GameManager::getFileFromUserSavedWithTypeAndIndex(DiySaveType type, int nameIndex)
{
	int index = (int)type;
	SaveItemTemplate object = SaveItemTemplateTem(WJUtils::stringAddInt("Save_", index, 3).c_str());

	std::string key = object.savePreKey;
	WJUtils::stringAddInt(key, nameIndex);
	std::string fileName = UserDefault::getInstance()->getStringForKey(key.c_str(), "");
	if (WJUtils::equals("", fileName.c_str()))
		return "";

	std::string fullpath = GameManager::getAtlasSavePath() + fileName;
	if (!WJUtils::fileExists(fullpath.c_str()))
		return "";

	return fullpath;
}
