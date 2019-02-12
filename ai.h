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

typedef struct {

	int		patrolNum;							// ����̃p�^�[���ԍ�
	float	cmpPattern[CMP_PATTERN_MAX];		// ��r�̍��v�l
	float	decision;							// ��r����
	float	atc[MOVE_PATTERN_MAX];				// �U��
	float	chase[MOVE_PATTERN_MAX];			// �ǐ�
	float	escape[MOVE_PATTERN_MAX];			// ����
	float	avoid[1];							// ���
	float	wait[MOVE_PATTERN_MAX];				// �ҋ@
	float	patrol[MOVE_PATTERN_MAX];			// ����
	int		deciMemory[DECISION_MEMORY_MAX];
	int		cntMemory;
}AI;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitAi(void);
void NonePlayerMove(void);
void NonePlayerAttack(void);
void NonePlayerPatrol(void);


float FuzzyRightUp(float val, float x0, float x1);
float FuzzyRightDown(float val, float x0, float x1);
float FuzzyTriangle(float val, float x0, float x1, float x2);
float FuzzyTrapezoid(float val, float x0, float x1, float x2, float x3);

float And(float a, float b);
float Or(float a, float b);
float Not(float a, float b);

#endif
