// Created by wangwenjie on 2013/12

#include "ModelBase.h"
#include "Common.h"
#include "PBase.h"

USING_NS_CC;


bool ModelBase::init()
{
	if (!WJLayer::init())
        return false;

	this->setContentSize(Size(1363, 768));
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5f, 0.5f));

	m_middlePoint = Vec2(1363 / 2, 768 / 2);

    // init
	ModelTemplate object = ModelTemplateTem(WJUtils::stringAddInt("model", m_modelNo).c_str());
	m_modelType = (modelType)object.type;
	m_isAnimal = object.animal;
	std::string jsonFile = object.norJsonFile;
	std::string atlasFile = object.norAtlasFile;
	m_skeleton = WJSkeletonAnimation::createWithData(
		WJSkeletonDataCache::getInstance()->addSkeletonData(jsonFile.c_str(), atlasFile.c_str()));
    m_skeleton->setPosition(m_middlePoint);
    m_skeleton->setCompleteListener(CC_CALLBACK_2(ModelBase::onCompletedListener, this));
	m_skeleton->setToSetupPose();
	m_skeleton->setSkin(object.skin.c_str());
	m_skeleton->setTouchSwallowsTouches(false);
	m_skeleton->setOnClick(CC_CALLBACK_2(ModelBase::onClickSkeleton, this));
	m_skeleton->noClickMoveEffect();
	this->addChild(m_skeleton, 10);

    return true;
}

void ModelBase::onCompletedListener(int trackIndex, int loopCount)
{

}

void ModelBase::onClickSkeleton(Node *node, WJTouchEvent *event)
{
	// nothing
}

ModelBase::ModelBase()
{
	m_skeleton = NULL;
	m_modelNo = -1;
	m_playIdle = true;
	m_isAnimal = false;
	m_standbyIndex = -1;
	m_playSound = true;
	m_middlePoint = Vec2::ZERO;
	m_playNameWithChannel1 = "";
	m_lastPlayRanAni = "";
	m_soundIsBindAni = false;
	m_modelTouchEndCallBack = nullptr;
	m_timerPlaySound = true;
	m_clickPlaySound = true;
	m_modelIsMoveScene = false;
}

int ModelBase::getModelNo()
{
    return m_modelNo;
}

WJSkeletonAnimation * ModelBase::getSkeleton()
{
    return m_skeleton;
}

void ModelBase::onEnter()
{
    WJLayer::onEnter();
	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ModelBase::showCompletedNotification),
		"showcompleted", NULL);
}

void ModelBase::showCompletedNotification(Ref *object)
{
	m_playSound = false;
}

void ModelBase::onEnterTransitionDidFinish()
{
    WJLayer::onEnterTransitionDidFinish();
}

void ModelBase::playAnimation( const char* animationName, bool loop, int stateIndex )
{
	playModelAniBindSound(animationName);
	m_skeleton->playAnimation(animationName, loop, stateIndex);
}

void ModelBase::playModelAniBindSound(const char * animationName)
{
	// 播放公主绑定的声音
	if (getModelNo() == (int)ModelNo::princess && m_soundIsBindAni)
	{
		if (WJUtils::equals(animationName, "aniSmile"))
		{
			Common::sound.play("Common:0025_princess");
		}
		else if (WJUtils::equals(animationName, "aniWave"))
		{
			Common::sound.play("Common:0026_princess");
		}
		else if (WJUtils::equals(animationName, "aniHappy"))
		{
			Common::sound.play("Common:0027_princess");
		}
		else if (WJUtils::equals(animationName, "aniIdle2"))
		{
			Common::sound.play("Common:0028_princess");
		}
		else if (WJUtils::equals(animationName, "aniCoversmile"))
		{
			Common::sound.play("Common:0029_princess");
		}
		else if (WJUtils::equals(animationName, "aniLookdown"))
		{
			Common::sound.play("Common:0030_princess");
		}
		else if (WJUtils::equals(animationName, "aniLookdown2"))
		{
			Common::sound.play("Common:0031_princess");
		}
		else if (WJUtils::equals(animationName, "aniLookup"))
		{
			Common::sound.play("Common:0032_princess");
		}
		else if (WJUtils::equals(animationName, "aniSalute"))
		{
			Common::sound.play("Common:0033_princess");
		}
		else if (WJUtils::equals(animationName, "aniHair"))
		{
			Common::sound.play("Common:0034_princess");
		}
		else if (WJUtils::equals(animationName, "aniNod"))
		{
			Common::sound.play("Common:0037_princess");
		}
	}
}

void ModelBase::onExit()
{
	NotificationCenter::getInstance()->removeObserver(this, "showcompleted");
	WJLayer::onExit();
}

void ModelBase::stopStandbyAnimationSchedule()
{
	this->unschedule(schedule_selector(ModelBase::playStandbyAnimation));
}

void ModelBase::startPlayClickStandbyAnimation()
{
	if (randomFindStandByAniIndex())
		playCurAggreagteAniAndSound(m_standbyIndex, false, 1, m_clickPlaySound);
}

void ModelBase::playStandbyAnimation(float time)
{
	if (randomFindStandByAniIndex())
		playCurAggreagteAniAndSound(m_standbyIndex, false, 1, m_timerPlaySound);
}

bool ModelBase::randomFindStandByAniIndex()
{
	if (m_curAggregateStr.size() < 1)
		return false;

	std::string name = m_skeleton->getPlayingAnimationName(1);
	if (WJUtils::equals(name, m_playNameWithChannel1.c_str()))
	{
		do 
		{
			if (m_curAggregateStr.size() == 1)
			{
				m_standbyIndex = 0;
				return true;
			}

			int index = WJUtils::randomInt(m_curAggregateStr.size());
			if (index != m_standbyIndex)
			{
				m_standbyIndex = index;
				return true;
			}
		} while (1);
	}

	return false;
}

void ModelBase::playCurAggreagteAniAndSound(int index, bool loop, int stateIndex, bool playSound)
{
	// 通道1没有播放默认的通道1常态动画
	if(!WJUtils::equals(m_skeleton->getPlayingAnimationName(1), m_playNameWithChannel1.c_str()))
		return;

	std::string key = m_curAggregateStr.at(index);
	playAnimation(key.c_str(), false, 1);

	if (!playSound)
		return;

	// 当前每个动画对应一个绑定的声音
	if (m_curSoundVector.size() > 1 && m_curSoundVector.size() == m_curAggregateStr.size())
	{
		std::string sound = m_curSoundVector.at(index);
		playSoundWithPlayAni(sound.c_str());
	}
	// 当前对应的动画序列只是对应一个声音
	else if (m_curSoundVector.size() == 1)
	{
		playSoundWithPlayAni(m_curSoundVector.at(0).c_str());
	}
}

void ModelBase::playSoundWithPlayAni(const char * key)
{
	if (m_playSound)
	{
		Common::sound.play(key);
	}
}

std::string ModelBase::playRandomAnimal(const char * a,... )
{
	vector<std::string> vec;
	va_list ap;
	va_start(ap,a);
	std::string str = "";
	do
	{
		str = va_arg(ap,char*);
		if (!WJUtils::equals(str, "end"))
			vec.push_back(str);

	}while (!WJUtils::equals(str, "end"));
	va_end(ap);
	random_shuffle(vec.begin(), vec.end());

	// 随机播放一个指定的动画序列中的动画、并比较和上一次播放的不一样
	std::string ani = "";
	do 
	{
		ani = vec.at(WJUtils::randomInt(vec.size()));
		if (!WJUtils::equals(m_lastPlayRanAni.c_str(), ani.c_str()))
		{
			m_lastPlayRanAni = ani;
			break;
		}
	} while (1);

	return ani;
}

void ModelBase::setStandbyAnimation( const char *a, ... )
{
	// 初始化默认指定的待机播放动画序列和点击播放序列
	m_curAggregateStr.clear();
	va_list ap;
	va_start(ap,a);
	std::string str = "";
	do
	{
		str = va_arg(ap,char*);
		if (!WJUtils::equals(str, "end"))
			m_curAggregateStr.push_back(str);

	}while (!WJUtils::equals(str, "end"));
	va_end(ap);
}

void ModelBase::setStandbySound( const char* a, ... )
{
	m_curSoundVector.clear();
	va_list ap;
	va_start(ap,a);
	std::string str = "";
	do
	{
		str = va_arg(ap,char*);
		if (!WJUtils::equals(str, "end"))
			m_curSoundVector.push_back(str);

	}while (!WJUtils::equals(str, "end"));
	va_end(ap);
}

void ModelBase::startPlayStandByAnimationSchedule( float IntervalTime )
{
	this->schedule(schedule_selector(ModelBase::playStandbyAnimation), IntervalTime);
}

void ModelBase::playSoundWhenPutIntoScene()
{
	if (getModelNo() == (int)ModelNo::blackKing || getModelNo() == (int)ModelNo::whitesKing)
	{
		Common::sound.play("Common:0039_prince");
	}
	else if (getModelNo() == (int)ModelNo::blackPrince || getModelNo() == (int)ModelNo::whitesPrince ||
		getModelNo() == (int)ModelNo::asiaPrince)
	{
		Common::sound.play("Common:0038_otherprincess");
	}
	else if (getModelNo() == (int)ModelNo::cat)
	{
		Common::sound.play("Common:0035");
	}
	else if (getModelNo() == (int)ModelNo::dog)
	{
		Common::sound.play("Common:0036");
	}
}

void ModelBase::playStandbyOneSoundEach()
{
	if (m_curSoundVector.size() == 1)
	{
		playSoundWithPlayAni(m_curSoundVector.at(0).c_str());
	}
}

void ModelBase::initMoveEvent()
{
	this->setMoveAble(true);
	this->noClickMoveEffect();
	this->setOnTouchAble(CC_CALLBACK_2(ModelBase::onModelTouchAbleWithPhoto, this));
	this->setOnTouchEnded(CC_CALLBACK_2(ModelBase::onModelTouchEndedWithPhoto, this));
}

bool ModelBase::onModelTouchAbleWithPhoto(Node *node, WJTouchEvent *event)
{
	this->setLocalZOrder(99998);
	return true;
}

void ModelBase::onModelTouchEndedWithPhoto(Node *node, WJTouchEvent *event)
{
	doEventModelTouchEndCallBack(node, event);
}

void ModelBase::doEventModelTouchEndCallBack(Node *node, WJTouchEvent *event)
{
	if (m_modelTouchEndCallBack)
		m_modelTouchEndCallBack(node, event);
}

void ModelBase::setModelTouchEndCallBack(const WJTouchCallback& callback)
{
	m_modelTouchEndCallBack = callback;
}
