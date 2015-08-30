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

	// �����������İ�ť
	CCMenuItem* startServerItem = CCMenuItemFont::create("StartServer", this, menu_selector(SceneGame::startServer));
	menu->addChild(startServerItem, Z_MENU);
	Common::moveNode(startServerItem, ccp(160, 0));

	// �����ͻ��˵İ�ť
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
		
		// ����
		if (data[0] == 3)
		{
			doRegret2();
			// ��������
			Net::RecvStart();
			schedule(schedule_selector(SceneGame::CheckRecv));
		}
		// ѡ��
		else if (data[0] == 1)
		{
			_selectid = data[1];
			_selectSprite->setPosition(_s[_selectid]->fromPlate());
			_selectSprite->setVisible(true);

			// ��������
			Net::RecvStart();
			schedule(schedule_selector(SceneGame::CheckRecv));
		}
		else if (data[0] == 2)
		{
			// �����ƶ���Ϣ
			Stone* s = _s[data[1]];
			int row = 9 - data[2];
			int col = 8 - data[3];
			int killid = Common::getStoneFromRowCol(row, col, _s);

			// ��¼������Ϣ
			recordStep(_selectid, killid, _s[_selectid]->_row, _s[_selectid]->_col, row, col);

			// �ƶ�����
			s->_row = row;
			s->_col = col;
			s->setPosition(s->fromPlate());

			// ɱ������
			if (killid != -1)
			{
				Stone* ks = _s[killid];
				ks->_dead = true;
				ks->setVisible(false);
			}

			// ��������
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
		// �����ӵ�����
		for (int i = 0; i < 32; i++)
		{
			Stone* s = _s[i];
			s->_row = 9 - s->_row;
			s->_col = 8 - s->_col;
			s->setPosition(s->fromPlate());
		}

		// ��ʼ����
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
	// ���ӳɹ���
	if (Net::isConnected())
	{
		unschedule(schedule_selector(SceneGame::CheckListen));

		// ��Ϸ��ʼ�ˡ�����һЩ��ʼ���Ĺ���
		CCLog("server Start Game ......");
	}
}

void SceneGame::doRegret()
{
	// ��Ϸ��û�п�ʼ���ܻ���
	if (_steps.size() == 0)
		return;

	// �ָ����һ��
	Step* step = *_steps.rbegin();
	_steps.pop_back();

	// ����ָ�����
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

	// ������ѡ�����ӵ�����£�����
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
	// �ֵ�˭�ߣ�˭��Ȩ����壬��Ȼ���ܵ�
	if (_bRedSide != _bRedTurn)
		return;

	// ���廹û�����������û���
	if (_steps.size() < 2)
		return;

	doRegret2();

	// ������Ϣ
	char buf = 3;
	Net::Send(&buf, 1);

}

bool SceneGame::ccTouchBegan(CCTouch*, CCEvent*)
{
	// �ͻ���δ�������޷����
	return Net::isConnected();
}

void SceneGame::ccTouchEnded(CCTouch* touch, CCEvent*)
{
	// �ƶ�����
	if (_selectid == -1)
	{
		// ��ͼѡ������
		SelectStone(touch);
	}
	else
	{
		// ��ͼ�ƶ�����
		MoveStone(touch);
	}
}

void SceneGame::SelectStone(CCTouch* touch)
{
	int row, col;
	bool bClick = Common::Screen2Plate(touch->getLocation(), row, col);
	// �������������⣬�ôε����Ч
	if (!bClick)
	{
		return;
	}

	// ������λ��û�����壬��ô���Ҳ��Ч
	int clickid = Common::getStoneFromRowCol(row, col, _s);
	if (clickid == -1)
	{
		return;
	}

	// ��������е����ӣ���_bRedTurn��ɫ��һ�£�����ѡ��
	if (_s[clickid]->_red != _bRedTurn)
		return;

	// �ڷ�ֻ�ܵ���壬�췽ֻ�ܵ���壬���������
	if (_s[clickid]->_red != _bRedSide)
		return;

	// ��¼��εĵ��
	_selectid = clickid;

	// ��ʾ�����ӱ�ѡ�е�Ч��
	_selectSprite->setPosition(Common::Plate2Screen(row, col));
	_selectSprite->setVisible(true);

	// ����ѡ�����ӵ���Ϣ���Է�
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

	// ��ͬ���Ӹ���ѡ��
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

	// �ж������Ƿ�����ƶ�
	canMove *move = canMove::createMove(this, _selectid, row, col, clickid);
	if (!move->move())
	{
		return;
	}
	CC_SAFE_DELETE(move);

	// ��¼������Ϣ
	recordStep(_selectid, clickid, _s[_selectid]->_row, _s[_selectid]->_col, row, col);

	// ɱ������
	if (clickid != -1)
	{
		_s[clickid]->setVisible(false);
		_s[clickid]->_dead = true;
	}

	// �ƶ�����
	_s[_selectid]->_row = row;
	_s[_selectid]->_col = col;
	_s[_selectid]->setPosition(Common::Plate2Screen(row, col));

	// ��������
	_selectid = -1;
	_selectSprite->setVisible(false);
	_bRedTurn = !_bRedTurn;

	
	// �����ƶ����ӱ���
	Step* step = *_steps.rbegin();
	char buf[4];
	buf[0] = 2;
	buf[1] = step->moveid;
	buf[2] = step->rowTo;
	buf[3] = step->colTo;
	Net::Send(buf, 4);

	// ������Ϣ
	Net::RecvStart();
	schedule(schedule_selector(SceneGame::CheckRecv));
}
