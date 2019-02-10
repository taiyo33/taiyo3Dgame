//=============================================================================
//
// AI���� [ai.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _AI_H_
#define _AI_H_

#include "main.h"

#define MOVE_CMP_MAX			(3)

typedef struct {

	float	patternA;				// ��r�̍��v�l
	float	patternB;				//
	float   patternC;				//
	float	decision;				// ��r����
	float	atc[MOVE_CMP_MAX];		// �U��
	float	chase[MOVE_CMP_MAX];	// �ǐ�
	float	escape[MOVE_CMP_MAX];	// ����
	float	avoid[1];				// ���
	float	wait[MOVE_CMP_MAX];

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
