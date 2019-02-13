//=============================================================================
//
// AI処理 [ai.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _AI_H_
#define _AI_H_

#include "main.h"

#define MOVE_PATTERN_MAX			(3)
#define CMP_PATTERN_MAX				(4)
#define DECISION_MEMORY_MAX			(5)

typedef struct {

	int		patrolNum;							// 巡回のパターン番号
	float	cmpPattern[CMP_PATTERN_MAX];		// 比較の合計値
	float	decision;							// 比較結果
	float	atc[MOVE_PATTERN_MAX];				// 攻撃
	float	chase[MOVE_PATTERN_MAX];			// 追跡
	float	escape[MOVE_PATTERN_MAX];			// 逃走
	float	avoid[1];							// 回避
	float	wait[MOVE_PATTERN_MAX];				// 待機
	float	patrol[MOVE_PATTERN_MAX];			// 巡回
	int		deciMemory[DECISION_MEMORY_MAX];
	int		cntMemory;
}AI;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitAi(void);
void NonePlayerMove(void);
void NonePlayerAttack(void);
void NonePlayerPatrol(int index);


float FuzzyRightUp(float val, float x0, float x1);
float FuzzyRightDown(float val, float x0, float x1);
float FuzzyTriangle(float val, float x0, float x1, float x2);
float FuzzyTrapezoid(float val, float x0, float x1, float x2, float x3);

float And(float a, float b);
float Or(float a, float b);
float Not(float a, float b);

#endif
