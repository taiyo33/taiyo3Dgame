//=============================================================================
//
// AI���� [ai.h]
// Author : GP11A_341_22_�c�����z
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

	int		patrolNum;							// ����̃p�^�[���ԍ�
	float	cmpPattern[CMP_PATTERN_MAX];		// ��r�̍��v�l
	float	decision;							// ��r����
	float	atc[MOVE_PATTERN_MAX];				// �U��
	float	chase[MOVE_PATTERN_MAX];			// �ǐ�
	float	escape[MOVE_PATTERN_MAX];			// ����
	float	wait[MOVE_PATTERN_MAX];				// �ҋ@
	float	patrol[MOVE_PATTERN_MAX];			// ����
	int		deciMemory[DECISION_MEMORY_MAX];
	int		cntMemory;
	bool	atcFlag;							// �U�����
	
	// ����f�[�^�֌W�ϐ�
	float	lenVec[ROUTEDATA_MAX];						// �x�N�g���̑傫��
	int		lapingTime[ROUTEDATA_MAX];					// �o�H�ʉ߂̌o�ߎ���
	float	routeRot[ROUTEDATA_MAX][ROUTEDATA_ARRAY_MAX];	// 
	int		routeIndex;
}AI;

//*****************************************************************************
// �v���g�^�C�v�錾
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
