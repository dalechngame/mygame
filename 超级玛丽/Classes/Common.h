#ifndef __COMMON_H__
#define __COMMON_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define winSize CCDirector::sharedDirector()->getWinSize()

struct Common
{
	enum DIRECTION { LEFT, RIGHT, UP, DOWN, NONE };
	enum OVERTYPE { TIME_OUT, DIE };

	static CCScene* scene(CCLayer* layer)
	{
		CCScene* s = CCScene::create();
		s->addChild(layer);
		return s;
	}

	static void playMusic(std::string musicFile)
	{
			if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			{
				musicFile.append(".wav");
			}
			else if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			{
				musicFile.append(".ogg");
			}
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(musicFile.c_str());
	}

	//! �ƶ�����
	static void moveNode(CCNode* node, CCPoint pt)
	{
		node->setPosition(node->getPosition() + pt);
	}

	//! ��ʽ���ַ���
	static char* format(int v, const char* prefix = "", const char* suffix = "")
	{
		static char buf[2048];
		sprintf(buf, "%s%d%s", prefix, v, suffix);
		return buf;
	}

	//! ��ö���
	static CCAnimation* CreateAnimation(const char* filename, int start, int end, int width, float delay, bool res = false)
	{
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(filename);
		CCArray* arr = CCArray::create();
		if (!res)
		{
			for (int i = start; i <= end; ++i)
			{
				CCSpriteFrame* frame = CCSpriteFrame::createWithTexture(texture, CCRectMake(i*width, 0, width, texture->getContentSize().height));
				arr->addObject(frame);
			}
		}
		else
		{
			for (int i = end; i >= start; --i)
			{
				CCSpriteFrame* frame = CCSpriteFrame::createWithTexture(texture, CCRectMake(i*width, 0, width, texture->getContentSize().height));
				arr->addObject(frame);
			}
		}

		return CCAnimation::createWithSpriteFrames(arr, delay);
	}

	//! ���һ֡
	static CCSpriteFrame* getSpriteFrame(const char* filename, int pos, int width)
	{
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(filename);
		CCSpriteFrame* frame = CCSpriteFrame::createWithTexture(texture, CCRectMake(pos*width, 0, width, texture->getContentSize().height));
		return frame;
	}

	static CCPoint Point2Tile(CCTMXTiledMap* map, CCPoint ptInMap)
	{
		//! ��ȡ���ӵ�����
		int dx = map->getTileSize().width;
		int dy = map->getTileSize().height;

		//! �ڼ�������(�����½���)
		int x = ptInMap.x / dx;
		int y = ptInMap.y / dy;

		//! �������Ͻǵ�����ϵ
		y = map->getMapSize().height - 1 - y;

		return ccp(x, y);
	}

	static CCPoint Tile2PointLB(CCTMXTiledMap* map, CCPoint ptTile)
	{
		//! ������½ǵ�����
		ptTile.y = map->getMapSize().height - 1 - ptTile.y;

		//! ÿ�����ӵ�����*ÿ�����ӵ����ش�С=λ��
		return ccp(ptTile.x * map->getTileSize().width,
			ptTile.y * map->getTileSize().height);
	}

};

#endif