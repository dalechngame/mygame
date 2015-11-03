// Created by yuanzhongji on 2013/07
#include "GameCommand.h"


bool GameCommand::init()
{
	// nothing
	return true;
}

void GameCommand::setNextCommand(GameCommand *command)
{
	m_nextCommand = command;
}

void GameCommand::execute( PBase *base )
{
	m_base = base;
}

GameCommand::GameCommand()
{
	m_nextCommand = NULL;
}

void GameCommand::gotoNextCommand(PBase *base)
{
	// 继续执行下一个效果
	if (m_nextCommand)
	{
		m_nextCommand->execute(base);
	}
	// 执行到最后一个动画完了、场景禁用取消
	else
	{
		base->setEnabled(true);
	}
}

void GameCommand::commandCompletedCallBack()
{
	gotoNextCommand(m_base);
}

bool GameStarCommand::init()
{
	if (!GameCommand::init())
	{
		return false;
	}

	return true;
}

GameStarCommand::GameStarCommand()
{
	m_starNum = ProgressStar::noStar;
}

void GameStarCommand::execute( PBase *base )
{
	GameCommand::execute(base);
	// 星星
	Star *star = base->getStar();
	const Size &size = Director::getInstance()->getWinSize();
	Point startPoint = Vec2(size.width / 2, size.height / 2);

	// 星星的结束位置
	WJSprite *toPointSprite = star->getSubSpriteByKey("touming");
	// 星星动画
	for (int i = 0; i < (int)m_starNum; i++)
	{
		WJSprite *spriteStar = WJSprite::create("game/03_minigame/common/ui/endStar.png");
		spriteStar->setPosition(startPoint);
		spriteStar->setScale(0.01f);
		base->addChild(spriteStar, 5000);

		Vec2 point = spriteStar->getParent()->convertToNodeSpace(toPointSprite->getPositionWorld());
		// 星星的旋转
		if (i == (int)m_starNum - 1)
		{
			spriteStar->runAction(Sequence::create(
				DelayTime::create(0.3f * i),
				Spawn::create(
				ScaleTo::create(0.3f, 0.4f),
				RotateBy::create(0.3f, 360),
				NULL),
				MoveTo::create(0.3f, point),
				CCCallFunc::create([&]() {
					Common::sound.play("Common:0012");
					Star::addStar(1);
					m_base->getStar()->playParticle();
			}),
				FadeOut::create(0.3f),
				Hide::create(),
				CCCallFunc::create([&]() {
					gotoNextCommand(m_base);
				}),
			NULL));
		}
		else
		{
			spriteStar->runAction(Sequence::create(
				DelayTime::create(0.3f * i),
				Spawn::create(
				ScaleTo::create(0.3f, 0.4f),
				RotateBy::create(0.3f, 360),
				NULL),
				MoveTo::create(0.3f, point),
				CCCallFunc::create([&]() {
					Common::sound.play("Common:0012");
					Star::addStar(1);
					m_base->getStar()->playParticle();
			}),
				FadeOut::create(0.3f),
				Hide::create(),
				NULL));
		}
	}
}

void GameSugarShowCommand::execute(PBase *base)
{
	GameCommand::execute(base);

	WJLayerJson *json = base->getLayerJson();

	// 替换当前场景显示的糖球棒
	for (int i = 1; i <= 5; i++)
	{
		ScrollItemTemplate object = ScrollItemTemplateTem(WJUtils::stringAddInt("P012_stick_", sugarVector.at(i - 1)->getValue(), 3));
		json->getSubSprite(WJUtils::stringAddInt("s", i, 3).c_str())->loadSpriteTexture(object.itemFile.c_str());
	}
	gotoNextCommand(m_base);
}

void GameShowDiyCommand::execute(PBase *base)
{
	GameCommand::execute(base);

	if (WJUtils::equals(m_diySaveKey, ""))
		gotoNextCommand(m_base);

	ShowDiyDialog *dialog = ShowDiyDialog::create("game/json/savcediy.json", m_diySaveKey.c_str());
	dialog->setOnCloseCallBack(CC_CALLBACK_1(GameShowDiyCommand::onDiyDialogClosed, this));
	base->addChild(dialog, 1000);
	dialog->show(LBModalDialogAnimationType::ScaleEaseBack);
}

void GameShowDiyCommand::onDiyDialogClosed(TipDialog *dialog)
{
	gotoNextCommand(m_base);
}

GameShowDiyCommand::GameShowDiyCommand()
{
	m_diySaveKey = "";
}
