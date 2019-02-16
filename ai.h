//=============================================================================
//
// AI [ai.h]
// Author : GP11A_341_22_c¾z
//
//=============================================================================
#ifndef _AI_H_
#define _AI_H_

#include "main.h"

#define MOVE_PATTERN_MAX			(3)
#define CMP_PATTERN_MAX				(4)
#define DECISION_MEMORY_MAX			(5)
#define ROUTEDATA_MAX				(9) 
#define ROUTEDATA_ARRAY_MAX			(3)

typedef struct {

	int		patrolNum;							// ñÌp^[Ô
	float	cmpPattern[CMP_PATTERN_MAX];		// ärÌvl
	float	decision;							// ärÊ
	float	atc[MOVE_PATTERN_MAX];				// U
	float	chase[MOVE_PATTERN_MAX];			// ÇÕ
	float	escape[MOVE_PATTERN_MAX];			// ¦
	float	wait[MOVE_PATTERN_MAX];				// Ò@
	float	patrol[MOVE_PATTERN_MAX];			// ñ
	int		deciMemory[DECISION_MEMORY_MAX];
	int		cntMemory;
	bool	atcFlag;							// UóÔ
	
	// ñf[^ÖWÏ
	float	lenVec[ROUTEDATA_MAX];						// xNgÌå«³
	int		lapingTime[ROUTEDATA_MAX];					// oHÊßÌoßÔ
	float	routeRot[ROUTEDATA_MAX][ROUTEDATA_ARRAY_MAX];	// 
	int		routeIndex;
}AI;

//*****************************************************************************
// vg^Cvé¾
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
