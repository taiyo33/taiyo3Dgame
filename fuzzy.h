//=============================================================================
//
// �t�@�W�[�����o�[�V�b�v���� [fuzzy.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _FUZZY_H_
#define _FUZZY_H_

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
float FuzzyRightUp(float val, float x0, float x1);
float FuzzyRightDown(float val, float x0, float x1);
float FuzzyTriangle(float val, float x0, float x1, float x2);
float FuzzyTrapezoid(float val, float x0, float x1, float x2, float x3);
float And(float a, float b);
float Or(float a, float b);
float Not(float a, float b);

#endif
