#ifndef __AI_H__  		
#define __AI_H__  		

#include "GameStartLayer.h"  			

class AI 
{  					
public: 					

	//! ����
	static Step *getStep(GameStartLayer *game);
	//! ��þ����
	static int getScore(GameStartLayer *game);
	static vector<Step *> getAllPossibleMove(GameStartLayer* game);
	static void fakeMove(GameStartLayer *game, Step *step);
	static void unfakeMove(GameStartLayer* game, Step* step);

	//! �ಽ�����㷨ʵ��
	static Step* getStep(GameStartLayer *game, int level);
	static int getMinScore(GameStartLayer *game, int level, int curMinScore);
	static int getMaxScore(GameStartLayer *game, int level, int curMaxScore);

	//! alphaBeta�㷨
	static Step *getStepABS(GameStartLayer *game, int level);
	//! alpha������Сֵ, beta�������ֵ
	static int ABS(GameStartLayer *game, int alpha, int beta, int level);
	static int _maxLevel;
	static Step *_step;

};  					
#endif  					
