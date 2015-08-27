#include "Mario.h"
#include "LayerGame.h"
#include "GameInfo.h"
#include "Bullet.h"

int Mario::_life = 1;

bool Mario::init()
{
	//! ��ʼ����
	CCSpriteFrame *frame = Common::getSpriteFrame("smallWalkRight.png", 0, 14);
	CCSprite::initWithSpriteFrame(frame);
	setZOrder(100);

	_speed = 100;
	_speedUp = 0;
	_speedAcc = 10;
	_speedDown = _speedAcc;

	_dir = Common::NONE;
	_faceDir = Common::RIGHT;
	_status = NORMAL;
	_grow = NONE;

	_bGodMode = false;
	_bAutoRun = false;
	_bflagRunAction = false;
	_onLadder = false;
	_playAudio = false;

	return true;
}

bool Mario::canMoveLeft(float dt)
{

	// �ж�mario�������ƶ�֮���ǲ��ǵ��˵�ͼ����
	CCRect rcMario = boundingBox();
	CCPoint ptMario = ccp(rcMario.getMinX(), rcMario.getMinY());
	CCTMXTiledMap* map = getMap();
	CCPoint ptMarioInWorld = map->convertToWorldSpace(ptMario);
	if (ptMarioInWorld.x - dt*_speed < 0) // - dt*_speed��ʾ�ƶ����ǲ��ǳ�ȥ�ˣ������ǵ�ǰ�Ѿ���ȥ��
		return false;

	CCPoint pt[3];
	pt[0] = ccp(rcMario.getMinX() - dt*_speed, rcMario.getMidY());
	pt[1] = ccp(rcMario.getMinX() - dt*_speed, rcMario.getMinY());
	pt[2] = ccp(rcMario.getMinX() - dt*_speed, rcMario.getMaxY());

	// ����ת������ptת���ɵ�ͼ��������,Ȼ���ȡgid���ж�gid�ǲ��Ǳ��赲
	for (int i = 0; i < 3; ++i)
	{
		if (pt[i].y >= map->getContentSize().height || pt[i].y <= 0)
		{
			continue;
		}

		CCPoint ptTile = Common::Point2Tile(map, pt[i]);
		// ˮ�ܡ�שͷ���ذ�
		static const char* layerName[4] = { "block", "pipe", "land", "coin" };
		for (int j = 0; j < 4; ++j)
		{
			CCTMXLayer* layer = map->layerNamed(layerName[j]);
			int gid = layer->tileGIDAt(ptTile);
			if (gid != 0)
			{
				if (std::string(layer->getLayerName()) == "coin")
				{
					eatCoin(layer, ptTile);

					return true;
				}
				return false;
			}
		}

	}

	return true;
}

bool Mario::canMoveRight(float dt)
{


	CCRect rcMario = boundingBox();
	CCTMXTiledMap* map = getMap();
	CCPoint pt[3];
	pt[0] = ccp(rcMario.getMaxX() + dt*_speed, rcMario.getMidY());
	pt[1] = ccp(rcMario.getMaxX() + dt*_speed, rcMario.getMinY());
	pt[2] = ccp(rcMario.getMaxX() + dt*_speed, rcMario.getMaxY());

	// ����ת������ptת���ɵ�ͼ��������,Ȼ���ȡgid���ж�gid�ǲ��Ǳ��赲
	for (int i = 0; i < 3; ++i)
	{
		if (pt[i].y >= map->getContentSize().height || pt[i].y <= 0)
		{
			continue;
		}

		CCPoint ptTile = Common::Point2Tile(map, pt[i]);
		// ˮ�ܡ�שͷ���ذ�
		static const char* layerName[4] = { "block", "pipe", "land", "coin" };
		for (int j = 0; j < 4; ++j)
		{
			CCTMXLayer* layer = map->layerNamed(layerName[j]);
			int gid = layer->tileGIDAt(ptTile);
			if (gid != 0)
			{
				if (std::string(layer->getLayerName()) == "coin")
				{
					eatCoin(layer, ptTile);
					return true;
				}
				return false;
			}
		}
	}

	return true;
}

bool Mario::canMoveDown(float dt)
{
	if (_onLadder)
	{
		return false;
	}

	CCRect rcMario = boundingBox();
	CCTMXTiledMap* map = getMap();
	CCPoint pt[3];
	pt[0] = ccp(rcMario.getMidX(), rcMario.getMinY() - dt*_speedDown);
	pt[1] = ccp(rcMario.getMinX(), rcMario.getMinY() - dt*_speedDown);
	pt[2] = ccp(rcMario.getMaxX(), rcMario.getMinY() - dt*_speedDown);

	if (pt[0].y >= map->getContentSize().height || pt[0].y <= 0)
	{
		return true;
	}

	for (int i = 0; i < 3; ++i)
	{
		CCPoint ptTile = Common::Point2Tile(map, pt[i]);
		// ˮ�ܡ�שͷ���ذ�
		static const char* layerName[4] = { "block", "pipe", "land", "coin" };
		for (int j = 0; j < 4; ++j)
		{
			CCTMXLayer* layer = map->layerNamed(layerName[j]);
			int gid = layer->tileGIDAt(ptTile);
			if (gid != 0)
			{
				if (std::string(layer->getLayerName()) == "coin")
				{
					eatCoin(layer, ptTile);
					return true;
				}
				// ΢��
				CCPoint ptLB = Common::Tile2PointLB(map, ptTile + ccp(0, -1));
				this->setPositionY(ptLB.y);

				// !���Զ����е������
				if (_bAutoRun && !_bflagRunAction)
				{
					_bflagRunAction = true;
					CCPoint ptFlagEnd = Common::Tile2PointLB(map, ptTile);
					Item::_Flag->runAction(CCMoveTo::create(1, ccp(Item::_Flag->getPositionX(), ptFlagEnd.y + 16)));

					LayerGame *game = (LayerGame *)(getMap()->getParent());
					Common::playMusic(std::string("QiZiLuoXia"));
				}

				return false;
			}
		}
	}

	return true;
}

bool Mario::canMoveUp(float dt)
{

	CCRect rcMario = boundingBox();
	CCTMXTiledMap* map = getMap();
	CCPoint pt[3];
	pt[0] = ccp(rcMario.getMidX(), rcMario.getMaxY() + dt*_speedUp);
	pt[1] = ccp(rcMario.getMinX(), rcMario.getMaxY() + dt*_speedUp);
	pt[2] = ccp(rcMario.getMaxX(), rcMario.getMaxY() + dt*_speedUp);


	if (pt[0].y >= map->getContentSize().height || pt[0].y <= 0)
	{
		return true;
	}

	// ����ת������ptת���ɵ�ͼ��������,Ȼ���ȡgid���ж�gid�ǲ��Ǳ��赲
	for (int i = 0; i < 3; ++i)
	{
		CCPoint ptTile = Common::Point2Tile(map, pt[i]);
		// ˮ�ܡ�שͷ���ذ�
		static const char* layerName[4] = { "block", "pipe", "land", "coin" };
		for (int j = 0; j < 4; ++j)
		{
			CCTMXLayer* layer = map->layerNamed(layerName[j]);
			int gid = layer->tileGIDAt(ptTile);
			if (gid != 0)
			{
				if (std::string(layer->getLayerName()) == "coin")
				{
					eatCoin(layer, ptTile);
					return true;
				}
				hit(layer, gid, ptTile);
				return false;
			}
		}
	}

	return true;
}

void Mario::moveLeft(float dt)
{
	if (!canMoveLeft(dt) || _status == DEAD)
	{
		return;
	}

	if (_dir != Common::LEFT)
	{
		_dir = Common::LEFT;
		_faceDir = Common::LEFT;
		if (_status == SHOOT)
			_status = NORMAL;
		updateStatus();
	}

	Common::moveNode(this, ccp(-dt*_speed, 0));
}

void Mario::moveRight(float dt)
{
	if (!canMoveRight(dt) || _status == DEAD)
		return;

	if (_dir != Common::RIGHT)
	{
		_dir = Common::RIGHT;
		_faceDir = Common::RIGHT;
		if (_status == SHOOT)
			_status = NORMAL;
		updateStatus();
	}

	Common::moveNode(this, ccp(dt*_speed, 0));
	moveMap(dt);

}

void Mario::moveUp(float dt)
{
	if (_speedUp <= 0 || _status == DEAD)
	{
		return;
	}
	if (!canMoveUp(dt))
	{
		// ����Ч��
		_speedDown = 10;
		_speedUp = 0;
		return;
	}

	Common::moveNode(this, ccp(0, dt*_speedUp));
	_speedUp -= _speedAcc;

}

void Mario::moveDown(float dt)
{
	if (_status == DEAD)
	{
		return;
	}
	if (_speedUp <= 0)
	{
		if (canMoveDown(dt))
		{
			_status = FLY;
			updateStatus();

			Common::moveNode(this, ccp(0, -dt*_speedDown));
			_speedDown += _speedAcc;
		}
		else
		{
			if (_status == FLY)
			{
				_status = NORMAL;
				_speedDown = 10;
				updateStatus();
			}
		}
	}
}

void Mario::jump()
{
	if (_status == DEAD || _status == FLY || _status == SHOOT || _bAutoRun || _bflagRunAction)
	{
		return;
	}
	Common::playMusic(std::string("Jump"));

	_speedUp = 300;
	_status = FLY;
	updateStatus();
}

void Mario::stop()
{
	if (_dir != Common::NONE)
	{
		_dir = Common::NONE;
		updateStatus();
	}
}

void Mario::updateStatus()
{
	stopAllActions();
	if (_status == SHOOT)
	{
		CCAnimate *ani = NULL;
		if (_faceDir == Common::LEFT)
		{
			ani = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("FireActionL"));
		}
		else
		{
			ani = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("FireActionR"));
		}
		CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(Mario::changeStaFuncCall));
		CCSequence *seq = CCSequence::create(ani, call, NULL);
		runAction(seq);
		return;
	}
	if (_status == CHANGSTA)
	{
		if (_speedUp != 0)
		{
			setFrame("jump", _faceDir, _grow, false);
		}
		else
		{
			setFrame("walk", _faceDir, _grow);
		}

		CCBlink *blink = CCBlink::create(1.0f, 7);
		CCCallFunc * call = CCCallFunc::create(this, callfunc_selector(Mario::stopActionFuncCall));
		CCSequence * seq = CCSequence::create(blink, call, NULL);

		this->runAction(seq);
		return;
	}

	if (_status == DEAD)
	{
		CCAnimate*animate = NULL;
		if (_grow == NONE)
		{
			animate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("smalldie"));
		}
		else if (_grow = BIG)
		{
			animate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("bigdie"));
		}
		else
		{
			animate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("firedie"));
		}

		CCJumpBy* moveUp = CCJumpBy::create(1.0f, CCPointZero, 30, 1);
		CCMoveBy* moveDown = CCMoveBy::create(1.0f, ccp(0, -winSize.height));
		CCCallFunc* callfunc = CCCallFunc::create(this, callfunc_selector(Mario::dieFuncCall));
		CCSequence* seq = CCSequence::create(animate, moveUp, moveDown, callfunc, NULL);

		runAction(seq);
		return;
	}

	if (_status == FLY)
	{
		setFrame("jump", _faceDir, _grow, false);

		return;
	}
	if (_status == NORMAL &&!_bAutoRun)
	{
		if (_dir != Common::NONE)
		{
			setAction("walk", _dir, _grow);
		}
		else
		{
			setFrame("walk", _faceDir, _grow);
		}
		return;
	}
}

void Mario::eatMushroom(Item::ItemType type)
{
	if (type == Item::IT_RewardGrow || type == Item::IT_RewardFlower)
	{
		LayerGame *game = (LayerGame *)(getMap()->getParent());
		Common::playMusic(std::string("EatMushroomOrFlower"));

		if (_grow == NONE)
		{
			game->unschedule(schedule_selector(LayerGame::moveMarioUpd));
			_grow = BIG;
			_status = CHANGSTA;
			updateStatus();
			setGodMode(3.0f);
		}
		else if (_grow == BIG)
		{
			game->unschedule(schedule_selector(LayerGame::moveMarioUpd));
			_grow = FIRE;
			_status = CHANGSTA;
			updateStatus();
			setGodMode(3.0f);
		}
		else
		{
			//...
		}
	}
	else if (type == Item::IT_RewardAddLife)
	{
		LayerGame *game = (LayerGame *)(getMap()->getParent());
		Common::playMusic(std::string("AddLife"));

		_life++;
	}
}

void Mario::hit(CCTMXLayer* layer, int gid, CCPoint ptTile)
{
	// �ʺ�block gid= 601 ��ͨ block 1 ���� block 431 ��846��ɫblock 31
	const char *name = layer->getLayerName();
	if (gid == 31 || (std::string(layer->getLayerName()) != "block"))
	{
		LayerGame *game = (LayerGame *)(getMap()->getParent());
		Common::playMusic(std::string("DingYingZhuanKuai"));

		return;
	}

	CCSprite* sprite = layer->tileAt(ptTile);
	if (gid == 431 || gid == 846 || gid == 601)
	{
		layer->setTileGID(31, ptTile);
		CCTMXTiledMap *map = this->getMap();
		LayerGame *game = (LayerGame *)map->getParent();
		Common::playMusic(std::string("DingYingZhuanKuai"));

		if (gid == 601 && !_hasReward(sprite))
		{
			CCPoint birthPoint = Common::Tile2PointLB(map, ptTile) + ccp(map->getTileSize().width / 2, map->getTileSize().height);
			// ����blinkcoin�Ķ���
			{
				game->coin->stopAllActions();
				game->coin->setPosition(birthPoint);
				CCJumpBy* jump = CCJumpBy::create(0.8f, ccp(0, -7), 45, 1);
				CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->animationByName("CoinBlink");
				CCAnimate* animate = CCAnimate::create(animation);

				game->coin->runAction(CCSpawn::create(jump, CCRepeat::create(animate, 2), NULL));
				Common::playMusic(std::string("EatCoin"));

			}
			_changeScore(COIN_SCORE, game, birthPoint);
		}
	}

	// (0, 0)����ԭ����Ծ
	CCJumpBy* jump = CCJumpBy::create(0.1f, ccp(0, 0), 20, 1);
	CCCallFuncN* callfunc = CCCallFuncN::create(this, callfuncN_selector(Mario::hasRewardFuncCall));

	sprite->runAction(CCSequence::createWithTwoActions(jump, callfunc));
}

void Mario::hasRewardFuncCall(CCNode* sprite)
{
	_hasReward(sprite);
}

bool Mario::_hasReward(CCNode* sprite)
{
	// �������ص�Ģ������
	CCObject* obj;
	int count = Item::_itemReward->count();
	CCARRAY_FOREACH(Item::_itemReward, obj)
	{
		Item* item = (Item*)obj;
		CCRect rcBlock = sprite->boundingBox();
		rcBlock.origin = rcBlock.origin + ccp(4, 4);
		rcBlock.size.width /= 2;
		rcBlock.size.height /= 2;
		if (item->boundingBox().intersectsRect(rcBlock))
		{
			LayerGame *game = (LayerGame *)(getMap()->getParent());
			Common::playMusic(std::string("DingChuMoGuHuoHua"));

			item->wakeup();
			return true;
		}
	}
	return false;
}

void Mario::die(bool dead)
{
	//return;

	// ������
	if (dead)
	{
		_status = DEAD;
		LayerGame *game = (LayerGame *)(getMap()->getParent());
		Common::playMusic(std::string("DiaoRuXianJingSi"));
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic("OnLand.wav");

		game->GameOver(Common::DIE);
	}

	// �޵�ģʽ�±�ײһ��Ҳû��ϵ
	if (dead == false && _bGodMode)
	{
		return;
	}

	if (dead == false && (_grow == BIG || _grow == FIRE))
	{
		_grow = NONE;
		_status = CHANGSTA;
		updateStatus();
		setGodMode(3.0f);
		CCTMXTiledMap *map = getMap();
		LayerGame *game = (LayerGame *)(map->getParent());
		Common::playMusic(std::string("SuoXiao"));

		game->unschedule(schedule_selector(LayerGame::moveMarioUpd));

		return;
	}

	LayerGame *game = (LayerGame *)(getMap()->getParent());
	Common::playMusic(std::string("YuDaoGuaiWuSi"));
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic("OnLand.wav");
	game->unschedule(schedule_selector(LayerGame::checkMarioDie));
	_status = DEAD;
	updateStatus();
}

void Mario::dieFuncCall()
{
	LayerGame *game = (LayerGame *)(getMap()->getParent());
	game->schedule(schedule_selector(LayerGame::checkMarioDie));
	game->GameOver(Common::DIE);
}

void Mario::stopActionFuncCall()
{
	_status = NORMAL;
	LayerGame *game = (LayerGame *)(getMap()->getParent());
	game->schedule(schedule_selector(LayerGame::moveMarioUpd));
}

void Mario::eatCoin(CCTMXLayer *layer, const CCPoint &ptTile)
{
	if (!_playAudio)
	{
		Common::playMusic(std::string("EatCoin"));

		setPlayAudio(0.99f);
	}
	GameInfo::setCoinNum(1);
	GameInfo::setScore(COIN_SCORE);

	layer->removeTileAt(ptTile);
}

void Mario::_changeScore(const int &score, LayerGame *game, const CCPoint &birthPoint)
{
	game->monsterScore->stopAllActions();
	game->monsterScore->setVisible(true);
	game->monsterScore->setPosition(birthPoint);
	game->monsterScore->setString(CCString::createWithFormat("%d", MUSHROOM_SCORE)->getCString());

	CCMoveBy *move = CCMoveBy::create(0.7f, ccp(0, 50));
	CCCallFunc *disappear = CCCallFunc::create(this, callfunc_selector(LayerGame::disaCall));
	CCSequence *sequ = CCSequence::create(move, disappear, NULL);
	game->monsterScore->runAction(sequ);
	GameInfo::setScore(MUSHROOM_SCORE);
}

void Mario::fire()
{
	if (Item::_pbulletArray->count() >= 2)
	{
		return;
	}
	if (_status == DEAD || _grow != FIRE)
	{
		return;
	}
	Common::playMusic(std::string("RengHuoQiu"));

	CCRect rec = boundingBox();
	CCDictionary *dict = CCDictionary::create();
	dict->setObject(CCString::create("fire"), "type");

	if (_faceDir == Common::RIGHT)
	{
		CCPoint ptFire = ccp(rec.getMaxX(), rec.getMaxY());
		dict->setObject(CCString::create("right"), "dir");
		dict->setObject(CCString::createWithFormat("%f", ptFire.x), "x");
		dict->setObject(CCString::createWithFormat("%f", ptFire.y), "y");
	}
	else
	{
		CCPoint ptFire = ccp(rec.getMinX(), rec.getMaxY());
		dict->setObject(CCString::create("left"), "dir");
		dict->setObject(CCString::createWithFormat("%f", ptFire.x), "x");
		dict->setObject(CCString::createWithFormat("%f", ptFire.y), "y");
	}

	Item* item = Item::create(dict);
	if (item)
	{
		item->_mario = this;
		this->getMap()->addChild(item);
	}
	_status = SHOOT;
	updateStatus();
}

void Mario::moveMap(float dt)
{

	// �ж�Mario�Ѿ����˵�ͼ���ұߣ�����Ҫ����ͼ
	CCPoint ptLastInMap = ccp(getMap()->getContentSize().width, 0);
	CCPoint ptLastInWorld = getMap()->convertToWorldSpace(ptLastInMap);
	if (ptLastInWorld.x <= winSize.width) // ��ͼ���һ�����ڴ�����
		return;

	CCNode* map = getParent();
	CCPoint ptWorld = map->convertToWorldSpace(this->getPosition());

	if (ptWorld.x > winSize.width / 2)
	{
		Common::moveNode(map, ccp(-dt*_speed, 0));
	}
}

void Mario::setFrame(const char *frameName, Common::DIRECTION dir, WEAPON w, bool isAni)
{
	std::string name = "";
	if (dir == Common::LEFT)
	{
		if (w == NONE)
		{
			name = "smallleft";
		}
		else if (w == BIG)
		{
			name = "bigleft";
		}
		else
		{
			name = "fireleft";
		}
	}
	else
	{
		if (w == NONE)
		{
			name = "smallright";
		}
		else if (w == BIG)
		{
			name = "bigright";
		}
		else
		{
			name = "fireright";
		}
	}
	name += frameName;
	const char * frame = name.c_str();
	if (isAni)
	{
		setDisplayFrameWithAnimationName(frame, 0);
	}
	else
	{
		setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frame));
	}
}

void Mario::setAction(const char *actionName, Common::DIRECTION dir, WEAPON w, bool forever)
{
	std::string name = "";
	if (dir == Common::LEFT)
	{
		if (w == NONE)
		{
			name = "smallleft";
		}
		else if (w == BIG)
		{
			name = "bigleft";
		}
		else
		{
			name = "fireleft";
		}
	}
	else
	{
		if (w == NONE)
		{
			name = "smallright";
		}
		else if (w == BIG)
		{
			name = "bigright";
		}
		else
		{
			name = "fireright";
		}
	}
	name += actionName;
	const char * frame = name.c_str();
	CCAnimate *ani = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName(frame));
	if (forever)
	{
		runAction(CCRepeatForever::create(ani));
	}
	else
	{
		runAction(ani);
	}
}
