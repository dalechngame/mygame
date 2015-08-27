#include "GameStartLayer.h"  			
#include <AI.h>
#include "AppMacros.h"			

CCScene * GameStartLayer::scene() 		
{ 						
	CCScene * scene = CCScene::create(); 	
	GameStartLayer * layer = GameStartLayer::create(); 
	scene->addChild(layer); 		
	return scene; 				
} 						

bool GameStartLayer::init() 			
{ 						
	CCLayer::init(); 			

	CreatePlate();
	Stone::_d = winSize.height / 10;;
	Stone::_offx = Stone::_d;
	Stone::_offy = Stone::_d / 2;

	for (int i = 0; i < 32; i++)
	{
		_s[i] = Stone::create();
		_s[i]->initStone(i);
		addChild(_s[i]);
	}

	setTouchEnabled(true);
	//! 默认是多点触摸
	setTouchMode(kCCTouchesOneByOne);

	_selectid = -1;
	_selectedSprite = CCSprite::create("selected.png");
	_selectedSprite->setVisible(false);
	_selectedSprite->setScale(0.6f);
	_selectedSprite->setZOrder(100);
	addChild(_selectedSprite);

	_bRedTurn = true;
	_steps.clear();

	addCtrlPanel();

	return true;				
} 						

void GameStartLayer::onExit()
{
	CCLayer::onExit();
}

void GameStartLayer::addCtrlPanel()
{
	CCMenu * menu = CCMenu::create();
	addChild(menu);

	CCMenuItemImage *regretItem = CCMenuItemImage::create("regret.jpg",
														  "regret.jpg", this, menu_selector(GameStartLayer::regret));
	menu->addChild(regretItem);

	moveNode(regretItem, ccp(160, 60));


}

void GameStartLayer::regret(CCObject *)
{
	if (_steps.size() ==  0)
	{
		return;
	}
	Step *step = *_steps.rbegin();
	_steps.pop_back();

	//! 具体恢复工作
	Stone *s = _s[step->moveid];
	s->_proper._row = step->rowFrom;
	s->_proper._col = step->colFrom;
	s->setPosition(s->fromPlate());

	Stone *killed;
	if (step->killid != -1)
	{
		killed = _s[step->killid];
		killed->_dead = false;
		killed->setVisible(true);
	}
	delete step;

	//! 隐藏选中框
	_selectid = -1;
	//_selectedSprite->setVisible(false);
	_selectedSprite->setPosition(s->getPosition());
}

void GameStartLayer::recordStep(int moveid, int killid, int rowFrom, int colFrom, int rowTo, int colTo)
{
	Step *s = new Step();
	s->colFrom = colFrom;
	s->colTo = colTo;
	s->killid = killid;
	s->moveid = moveid;
	s->rowFrom = rowFrom;
	s->rowTo = rowTo;

	_steps.push_back(s);
}

void GameStartLayer::CreatePlate()
{
	CCSprite *plate = CCSprite::create("background.png");
	addChild(plate);
	plate->setPosition(ccp(0, 0));
	plate->setAnchorPoint(ccp(0, 0));
	//! 设置比例
	plate->setScale(winSize.height / plate->getContentSize().height);
}

bool GameStartLayer::ccTouchBegan(CCTouch *, CCEvent *)
{
	return true;
}

void GameStartLayer::ccTouchEnded(CCTouch *touch, CCEvent *)
{
	if (_selectid == -1)
	{
		//! 选中
		selectStone(touch);
	}
	else
	{
		//! 移动
		moveStone(touch);
	}
}

void GameStartLayer::selectStone(CCTouch* touch)
{
	CCPoint ptClicked = touch->getLocation();
	int row, col;
	bool bClick = Screen2Plate(ptClicked, row, col);
	//! 点击在棋子间的空隙点击无效
	if (!bClick)
	{
		return;
	}
	int clickid = getStoneFromRowCol(row, col);
	//! 棋子被杀掉点击无效
	if (clickid == -1)
	{
		return;
	}
	//! 如果不是redturn 点击无效
	if (_s[clickid]->_red !=_bRedTurn)
	{
		return;
	}
	_selectid = clickid;
	_selectedSprite->setPosition(Plate2Screen(row, col));
	_selectedSprite->setVisible(true);
}

void GameStartLayer::moveStone(CCTouch *touch)
{
	CCPoint ptClicked = touch->getLocation();
	int row, col;
	bool bClick = Screen2Plate(ptClicked, row, col);
	//! 点击在棋子间的空隙点击无效
	if (!bClick)
	{
		return;
	}

	int clickid = getStoneFromRowCol(row, col);
	if (clickid != -1)
	{
		//! 如果颜色相同,更换选中棋子
		if (isSameColor(clickid, _selectid))
		{
			_selectid = clickid;
			CCPoint pt = Plate2Screen(row, col);
			_selectedSprite->setPosition(pt);
			_selectedSprite->setVisible(true);

			return;
		}
		//else
		//{
		//	//! kill
		//	_s[clickid]->setVisible(false);
		//	_s[clickid]->_dead = true;
		//}
	}

	//! 判断是否可以移动棋子
	if (!canMove(_selectid, row, col, clickid))
	{
		return;
	}
	recordStep(_selectid, clickid, _s[_selectid]->_proper._row, _s[_selectid]->_proper._col, row, col);
	//! kill
	if (clickid != -1)
	{
		// 杀掉
		_s[clickid]->setVisible(false);
		_s[clickid]->_dead = true;
	}

	//! 移动棋子
	_s[_selectid]->_proper._row = row;
	_s[_selectid]->_proper._col = col;
	_s[_selectid]->setPosition(Plate2Screen(row, col));

	//! 重置属性
	_selectid = -1;
	//_selectedSprite->setVisible(false);
	_selectedSprite->setPosition(Plate2Screen(row, col));
	_bRedTurn = !_bRedTurn;



	//! 智能移动
	if (!_bRedTurn)
	{
		//CCDelayTime *delay = CCDelayTime::create(0.01f);
		//CCCallFunc *ai = CCCallFunc::create(this, callfunc_selector(GameStartLayer::computerMove));
		//CCSequence *seq = CCSequence::create(delay, ai, nullptr);
		//this->runAction(seq);
		scheduleOnce(schedule_selector(GameStartLayer::computerMove), 0.01f);
	}
}

void GameStartLayer::computerMove(float)
{
	Step*step = AI::getStepABS(this, 2);

	//Step *step = AI::getStep(this, 2);
	Stone *moveStone = _s[step->moveid];
	moveStone->_proper._row = step->rowTo;
	moveStone->_proper._col = step->colTo;
	moveStone->setPosition(moveStone->fromPlate());

	_bRedTurn = !_bRedTurn;

	if (step->killid != -1)
	{
		Stone *killStone = _s[step->killid];
		killStone->_dead = true;
		killStone->setVisible(false);
	}
	_selectedSprite->setPosition(moveStone->getPosition());
	_steps.push_back(step);
}

bool GameStartLayer::Screen2Plate(const CCPoint& ptScreen, int& row, int& col)
{
	//! 计算格子半径的平方
	int distance = Stone::_d*Stone::_d / 4;
	//! 遍历所有象棋坐标点, 判断是否有点在半径内
	for (row = 0; row <= 9; ++row)
	{
		for (col = 0; col <= 8;++col)
		{
			CCPoint ptCenter = Plate2Screen(row, col);
			//计算当前点击的点和棋子中心点的距离
			if (ptCenter.getDistanceSq(ptScreen) < distance)
			{
				return true;
			}
		}
	}
	return false;
}

//! 转换为真实坐标
CCPoint GameStartLayer::Plate2Screen(int row, int col)
{
	int x = col *Stone::_d + Stone::_offx;
	int y = row*Stone::_d + Stone::_offy;

	return ccp(x, y);
}

int GameStartLayer::getStoneFromRowCol(int row, int col)
{
	for (int i = 0; i < 32; i++)
	{
		if (_s[i]->_proper._row == row && _s[i]->_proper._col == col && !_s[i]->_dead)
			return i;
	}

	return -1;
}

bool GameStartLayer::canMove(int moveid, int row, int col, int killid)
{
	Stone *s = _s[moveid];
	switch (s->_proper._type)
	{
	case Stone::ROOK:
		return canMoveRook(moveid, row, col);

	case Stone::HORSE:
		return canMoveHorse(moveid, row, col);

	case Stone::CANNON:
		return canMoveCannon(moveid, row, col, killid);

	case Stone::PAWN:
		return canMovePawn(moveid, row, col);

	case Stone::KING:
		return canMoveKing(moveid, row, col, killid);

	case Stone::GUARD:
		return canMoveGuard(moveid, row, col);

	case Stone::ELEPH:
		return canMoveEleph(moveid, row, col);
		break;
	}

	return false;
}

int GameStartLayer :: getStoneCount(int row1, int col1, int row2, int col2)
{
	int ret = 0;
	//! 完全相同和完全不同的点
	if (row1 != row2 && col1 != col2) return -1;
	if (row1 == row2 && col1 == col2) return -1;

	int min, max;
	
	if (row1 == row2)
	{
		min = col1 < col2 ? col1 : col2;
		max = col1 > col2 ? col1 : col2;
		for (int col = min + 1; col < max; ++col)
		{
			int id = getStoneFromRowCol(row1, col);
			if (id != -1)
			{
				++ret;
			}
		}
	}
	else
	{
		min = row1 < row2 ? row1 : row2;
		max = row1 > row2 ? row1 : row2;
		for (int row = min + 1; row < max; ++row)
		{
			int id = getStoneFromRowCol(row, col1);
			if (id != -1)
			{
				++ret;
			}
		}
	}
	return ret;
}

bool GameStartLayer::canMoveRook(int moveid, int row, int col)
{
	Stone *s = _s[moveid];
	return getStoneCount(s->_proper._row, s->_proper._col, row, col) == 0;
}

bool GameStartLayer::canMoveCannon(int moveid, int row, int col, int killid)
{
	Stone *s = _s[moveid];
	if (killid == -1)
	{
		return getStoneCount(s->_proper._row, s->_proper._col, row, col) == 0;
	}

	return getStoneCount(s->_proper._row, s->_proper._col, row, col) == 1;
}

bool GameStartLayer::canMoveHorse(int moveid, int row, int col)
{
	Stone *s = _s[moveid];
	//! 求出两点之间的坐标
	int dRow = abs(s->_proper._row - row);
	int dCol = abs(s->_proper._col - col);
	int d = dRow * 10 + dCol;

	if (d == 12 || d == 21)
	{
		int cRow, cCol;
		if (d == 12)
		{
			cCol = (col +s->_proper._col) / 2;
			cRow = s->_proper._row;
		}
		else
		{
			cCol = s->_proper._col;
			cRow = (s->_proper._row +row) / 2;
		}
		// 没有蹩脚的棋子
		if (getStoneFromRowCol(cRow, cCol) == -1)
			return true;
	}

	return false;
}

bool GameStartLayer::canMovePawn(int moveid, int row, int  col)
{
	Stone *s = _s[moveid];
	//! 求出两点之间的坐标
	int dRow = abs(s->_proper._row - row);
	int dCol = abs(s->_proper._col - col);
	int d = dRow * 10 + dCol;

	if (d != 1 && d != 10)
	{
		return false;
	}
	if (s->_red)
	{
		if (row < s->_proper._row)
		{
			return false;
		}
		if (s->_proper._row <= 4 && s->_proper._row == row)
		{
			return false;
		}
	}
	else
	{
		//! 不能后退
		if (row > s->_proper._row)
		{
			return false;
		}
		//! 过河前不能平移
		if (s->_proper._row >= 5 && s->_proper._row == row)
		{
			return false;
		}
	}

	return true;
}

bool GameStartLayer::canMoveKing(int moveid, int row, int col, int killid)
{
	Stone *s = _s[moveid];
	int dRow = abs(s->_proper._row - row);
	int dCol = abs(s->_proper._col - col);
	int d = dRow * 10 + dCol;

	//! 老将照面
	if (killid != -1)
	{
		Stone *kill = _s[killid];
		if (kill->_proper._type == Stone::KING)
		{
			return canMoveRook(moveid, row, col);
		}
	}

	//! 一次走一格
	if (d != 1 && d != 10)
	{
		return false;
	}

	//! 不能出九宫格
	if (col < 3 || col > 5)
	{
		return false;
	}
	if (s->_red)
	{
		if (row < 0 || row > 2)
		{
			return false;
		}
	}
	else
	{
		if (row < 7 || row > 9)
		{
			return false;
		}
	}
}

bool GameStartLayer::canMoveGuard(int moveid, int row, int col)
{
	Stone *s = _s[moveid];
	int dRow = abs(s->_proper._row - row);
	int dCol = abs(s->_proper._col - col);
	int d = dRow * 10 + dCol;
	if (d != 11) return false;

	//! 不能出九宫
	if (col < 3 || col > 5) return false;
	if (s->_red)
	{
		if (row < 0 || row > 2) return false;
	}
	else
	{
		if (row < 7 || row > 9) return false;
	}

	return true;
}

bool GameStartLayer::canMoveEleph(int moveid, int row, int col)
{
	Stone *s = _s[moveid];
	int dRow = abs(s->_proper._row - row);
	int dCol = abs(s->_proper._col - col);
	int d = dRow * 10 + dCol;
	if (d != 22)
	{
		return false; 
	}

	//! 相眼
	int cRow, cCol;
	cRow = (row +s->_proper._row) / 2;
	cCol = (col +s->_proper._col) / 2;
	if (getStoneFromRowCol(cRow, cCol) != -1)
	{
		return false;
	}

	//! 不能过河
	if (s->_red)
	{
		if (row > 4)
		{
			return false;
		}
	}
	else
	{
		if (row < 5)
		{
			return false;
		}
	}
}
