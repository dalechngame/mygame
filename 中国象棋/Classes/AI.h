#ifndef __AI_H__  		
#define __AI_H__  		

#include "GameStartLayer.h"  			

class AI 
{  					
public: 					

	//! 走棋
	static Step *getStep(GameStartLayer *game);
	//! 获得局面分
	static int getScore(GameStartLayer *game);
	static vector<Step *> getAllPossibleMove(GameStartLayer* game);
	static void fakeMove(GameStartLayer *game, Step *step);
	static void unfakeMove(GameStartLayer* game, Step* step);

	//! 多步智能算法实现
	static Step* getStep(GameStartLayer *game, int level);
	static int getMinScore(GameStartLayer *game, int level, int curMinScore);
	static int getMaxScore(GameStartLayer *game, int level, int curMaxScore);

	//! alphaBeta算法
	static Step *getStepABS(GameStartLayer *game, int level);
	//! alpha代表最小值, beta代表最大值
	static int ABS(GameStartLayer *game, int alpha, int beta, int level);
	static int _maxLevel;
	static Step *_step;

};  					
#endif  					
