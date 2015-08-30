#include "SceneGame.h"
#include "Net.h"
#include "canMove.h"

bool SceneGame::init()
{
	CCLayer::init();

	CreatePlate();
	addCtrlPanel();

	_selectSprite = CCSprite::create("selected.png");
	_selectSprite->setVisible(false);
	_selectSprite->setScale(.6f);
	addChild(_selectSprite, Z_STONE);

	Stone::_d = winSize.height/10;
	Stone::_offx = Stone::_d;
	Stone::_offy = Stone::_d/2;

	for (int i = 0; i < 32; i++)
	{
		_s[i] = Stone::create();
		_s[i]->initStone(i);
		addChild(_s[i], Z_STONE);
	}

	_selectid = -1;
	_bRedTurn = true;
	_steps.clear();

	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);

	return true;
}

void SceneGame::CreatePlate()
{
	CCSprite* plate = CCSprite::create("background.png");
	addChild(plate, Z_PLATE);
	plate->setPosition(ccp(0, 0));
	plate->setAnchorPoint(ccp(0, 0));
	plate->setScale(winSize.height / plate->getContentSize().height);
}

void SceneGame::addCtrlPanel()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu, Z_MENU);

	CCMenuItemImage* regretItem = CCMenuItemImage::create("regret.jpg", "regret.jpg",
		this, menu_selector(SceneGame::Regret));
	menu->addChild(regretItem, Z_MENU);
	Common::moveNode(regretItem, ccp(160, 60));

	// 启动服务器的按钮
	CCMenuItem* startServerItem = CCMenuItemFont::create("StartServer", this, menu_selector(SceneGame::startServer));
	menu->addChild(startServerItem, Z_MENU);
	Common::moveNode(startServerItem, ccp(160, 0));

	// 启动客户端的按钮
	CCMenuItem* startClientItem = CCMenuItemFont::create("StartClient", this, menu_selector(SceneGame::startClient));
	menu->addChild(startClientItem, Z_MENU);
	Common::moveNode(startClientItem, ccp(160, -60));

	startServerItem->setUserObject(startClientItem);
	startClientItem->setUserObject(startServerItem);

	regretItem->setScale(0.7f);
	startServerItem->setScale(0.7f);
	startClientItem->setScale(0.7f);
}

void SceneGame::CheckRecv(float)
{
	if (Net::isRecvComplete())
	{
		int len;
		char* data = Net::RecvData(len);
		unschedule(schedule_selector(SceneGame::CheckRecv));
		
		// 悔棋
		if (data[0] == 3)
		{
			doRegret2();
			// 继续接收
			Net::RecvStart();
			schedule(schedule_selector(SceneGame::CheckRecv));
		}
		// 选棋
		else if (data[0] == 1)
		{
			_selectid = data[1];
			_selectSprite->setPosition(_s[_selectid]->fromPlate());
			_selectSprite->setVisible(true);

			// 继续接收
			Net::RecvStart();
			schedule(schedule_selector(SceneGame::CheckRecv));
		}
		else if (data[0] == 2)
		{
			// 接受移动信息
			Stone* s = _s[data[1]];
			int row = 9 - data[2];
			int col = 8 - data[3];
			int killid = Common::getStoneFromRowCol(row, col, _s);

			// 记录走棋信息
			recordStep(_selectid, killid, _s[_selectid]->_row, _s[_selectid]->_col, row, col);

			// 移动棋子
			s->_row = row;
			s->_col = col;
			s->setPosition(s->fromPlate());

			// 杀死棋子
			if (killid != -1)
			{
				Stone* ks = _s[killid];
				ks->_dead = true;
				ks->setVisible(false);
			}

			// 更新数据
			_selectid = -1;
			_selectSprite->setVisible(false);
			_bRedTurn = !_bRedTurn;
		}
	}
}

void SceneGame::startClient(CCObject*obj)
{
	if (Net::Connect("127.0.0.1"))
	{
		// 把棋子倒过来
		for (int i = 0; i < 32; i++)
		{
			Stone* s = _s[i];
			s->_row = 9 - s->_row;
			s->_col = 8 - s->_col;
			s->setPosition(s->fromPlate());
		}

		// 开始接收
		Net::RecvStart();
		schedule(schedule_selector(SceneGame::CheckRecv));

		Net::_isConnected = true;
		_bRedSide = false;
		CCMenuItemFont *font = (CCMenuItemFont *)obj;
		CCMenuItemFont *server = (CCMenuItemFont *)font->getUserObject();
		server->setEnabled(false);
		font->setEnabled(false);
	}
	else
	{
		CCLog("Connect failure....");
	}
}

void SceneGame::startServer(CCObject*obj)
{
	CCMenuItemFont *font = (CCMenuItemFont *)obj;
	CCMenuItemFont *client = (CCMenuItemFont *)font->getUserObject();
	client->setEnabled(false);
	font->setEnabled(false);

	_bRedSide = true;
	Net::Accept();
	schedule(schedule_selector(SceneGame::CheckListen));
}

void SceneGame::CheckListen(float)
{
	// 连接成功了
	if (Net::isConnected())
	{
		unschedule(schedule_selector(SceneGame::CheckListen));

		// 游戏开始了。。做一些初始化的工作
		CCLog("server Start Game ......");
	}
}

void SceneGame::doRegret()
{
	// 游戏还没有开始不能悔棋
	if (_steps.size() == 0)
		return;

	// 恢复最后一步
	Step* step = *_steps.rbegin();
	_steps.pop_back();

	// 具体恢复工作
	Stone* s = _s[step->moveid];
	s->_row = step->rowFrom;
	s->_col = step->colFrom;
	s->setPosition(s->fromPlate());

	Stone* kill;
	if (step->killid != -1)
	{
		kill = _s[step->killid];
		kill->_dead = false;
		kill->setVisible(true);
	}

	_bRedTurn = !_bRedTurn;
	delete step;

	// 避免在选中棋子的情况下，悔棋
	_selectid = -1;
	_selectSprite->setVisible(false);
}

void SceneGame::doRegret2()
{
	for (int i = 0; i < 2; ++i)
	{
		doRegret();
	}
}

void SceneGame::Regret(CCObject*)
{
	// 轮到谁走，谁有权点悔棋，不然不能点
	if (_bRedSide != _bRedTurn)
		return;

	// 走棋还没走两步，不让悔棋
	if (_steps.size() < 2)
		return;

	doRegret2();

	// 发送信息
	char buf = 3;
	Net::Send(&buf, 1);

}

bool SceneGame::ccTouchBegan(CCTouch*, CCEvent*)
{
	// 客户端未连接上无法点击
	return Net::isConnected();
}

void SceneGame::ccTouchEnded(CCTouch* touch, CCEvent*)
{
	// 移动象棋
	if (_selectid == -1)
	{
		// 企图选中棋子
		SelectStone(touch);
	}
	else
	{
		// 企图移动棋子
		MoveStone(touch);
	}
}

void SceneGame::SelectStone(CCTouch* touch)
{
	int row, col;
	bool bClick = Common::Screen2Plate(touch->getLocation(), row, col);
	// 如果点击在棋盘外，该次点击无效
	if (!bClick)
	{
		return;
	}

	// 如果点击位置没有象棋，那么点击也无效
	int clickid = Common::getStoneFromRowCol(row, col, _s);
	if (clickid == -1)
	{
		return;
	}

	// 如果被点中的棋子，和_bRedTurn颜色不一致，不让选中
	if (_s[clickid]->_red != _bRedTurn)
		return;

	// 黑方只能点黑棋，红方只能点红棋，用于网络版
	if (_s[clickid]->_red != _bRedSide)
		return;

	// 记录这次的点击
	_selectid = clickid;

	// 显示该棋子被选中的效果
	_selectSprite->setPosition(Common::Plate2Screen(row, col));
	_selectSprite->setVisible(true);

	// 发送选中棋子的信息给对方
	char buf[2];
	buf[0] = 1;
	buf[1] = _selectid;
	Net::Send(buf, 2);
}

void SceneGame::recordStep(int moveid, int killid, int rowFrom, int colFrom, int rowTo, int colTo)
{
	Step* s = new Step();
	s->colFrom = colFrom;
	s->colTo = colTo;
	s->killid = killid;
	s->moveid = moveid;
	s->rowFrom = rowFrom;
	s->rowTo = rowTo;

	_steps.push_back(s);
}

void SceneGame::MoveStone(CCTouch* touch)
{
	int row, col;
	bool bClick = Common::Screen2Plate(touch->getLocation(), row, col);
	if (!bClick)
	{
		return;
	}

	// 相同棋子更换选择
	int clickid = Common::getStoneFromRowCol(row, col, _s);
	if (clickid != -1)
	{
		if (Common::isSameColor(clickid, _selectid, _s))
		{
			_selectid = clickid;
			CCPoint pt = Common::Plate2Screen(row, col);
			_selectSprite->setPosition(pt);
			_selectSprite->setVisible(true);

			return;
		}
	}

	// 判断棋子是否可以移动
	canMove *move = canMove::createMove(this, _selectid, row, col, clickid);
	if (!move->move())
	{
		return;
	}
	CC_SAFE_DELETE(move);

	// 记录走棋信息
	recordStep(_selectid, clickid, _s[_selectid]->_row, _s[_selectid]->_col, row, col);

	// 杀掉棋子
	if (clickid != -1)
	{
		_s[clickid]->setVisible(false);
		_s[clickid]->_dead = true;
	}

	// 移动棋子
	_s[_selectid]->_row = row;
	_s[_selectid]->_col = col;
	_s[_selectid]->setPosition(Common::Plate2Screen(row, col));

	// 更新数据
	_selectid = -1;
	_selectSprite->setVisible(false);
	_bRedTurn = !_bRedTurn;

	
	// 发送移动棋子报文
	Step* step = *_steps.rbegin();
	char buf[4];
	buf[0] = 2;
	buf[1] = step->moveid;
	buf[2] = step->rowTo;
	buf[3] = step->colTo;
	Net::Send(buf, 4);

	// 接收信息
	Net::RecvStart();
	schedule(schedule_selector(SceneGame::CheckRecv));
}
