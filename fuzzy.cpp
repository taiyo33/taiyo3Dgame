//=============================================================================
//
// �t�@�W�[�����o�[�V�b�v���� [fuzzy.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "main.h"
#include "fuzzy.h"

//*****************************************************************************
// �t�@�W�[�����o�[�V�b�v�E�オ��^
// ���@���Ffloat val(�Q�l�ɂ��鐔�l), float x0(�͈͂̍ŏ��l), float x1(�͈͂̍ő�l)
// �߂�l�Ffloat�^�@0.0�`1.0�̐��l
//*****************************************************************************
float FuzzyRightUp(float val, float x0, float x1)
{
	if (val <= x0)
	{
		return 0.0f;
	}
	else if (val >= x1)
	{
		return 1.0f;
	}
	else
	{
		// ������v�Z
		float denom = x1 - x0;
		return (val / denom) - (x0 / denom);
	}
}

//*****************************************************************************
// �t�@�W�[�����o�[�V�b�v�E������^
// ���@���Ffloat val(�Q�l�ɂ��鐔�l), float x0(�͈͂̍ŏ��l), float x1(�͈͂̍ő�l)
// �߂�l�Ffloat�^�@0.0�`1.0�̐��l
//*****************************************************************************
float FuzzyRightDown(float val, float x0, float x1)
{
	if (val <= 0)
	{
		return 1;
	}
	else if (val >= x1)
	{
		return 0;
	}
	else
	{
		float denom = x1 - x0;
		return (x1 / denom) - (val / denom);
	}
}

//*****************************************************************************
// �t�@�W�[�����o�[�V�b�v�O�p�^
// ���@���Ffloat val(�Q�l�ɂ��鐔�l), float x0(�͈͂̍ŏ��l), float x1(�Ԃ�l�̍ő�l)
//		   float x2()
// �߂�l�Ffloat�^�@0.0�`1.0�̐��l
//*****************************************************************************
float FuzzyTriangle(float val, float x0, float x1, float x2)
{
	if (val <= x0)
	{
		return 0;
	}
	else if (val == x1)
	{
		return 1;
	}
	else if ((val > x0) && (val < x1))
	{
		float denom = x1 - x0;
		return (val / denom) - (x0 / denom);
	}
	else
	{
		float denom = x2 - x1;
		return (x2 / denom) - (val / denom);
	}
}

//*****************************************************************************
// �t�@�W�[�����o�[�V�b�v��`�^
// ���@���Ffloat val(�Q�l�ɂ��鐔�l), float x0(�͈͂̍ŏ��l), float x1(�͈͂̍ő�l)
//		   float x2(), float x3()
// �߂�l�Ffloat�^�@0.0�`1.0�̐��l
//*****************************************************************************
float FuzzyTrapezoid(float val, float x0, float x1, float x2, float x3)
{
	if (val <= x0)
	{
		return 0;
	}
	else if ((val >= x1) && (val <= x2))
	{
		return 1;
	}
	else if ((val > x0) && (val < x1))
	{
		float denom = x1 - x0;
		return (val / denom) - (x0 / denom);
	}
	else
	{
		float denom = x3 - x2;
		return (x3 / denom) - (val / denom);
	}
}

//*****************************************************************************
// �t�@�W�[AND���Z�q
// ���@���Ffloat a(��r�l1), float b(��r�l2)
// �߂�l�Ffloat�^�@���������̒l��Ԃ�
//*****************************************************************************
float And(float a, float b)
{
	return min(a, b);
}

//*****************************************************************************
// �t�@�W�[OR���Z�q
// ���@���Ffloat a(��r�l1), float b(��r�l2)
// �߂�l�Ffloat�^�@�傫�����̒l��Ԃ�
//*****************************************************************************

float Or(float a, float b)
{
	return max(a, b);
}

//*****************************************************************************
// �t�@�W�[Not���Z�q
// ���@���Ffloat a(��r�l1), float b(��r�l2)
// �߂�l�Ffloat�^�@���]�����l��Ԃ�
//*****************************************************************************
float Not(float a, float b)
{
	return 1.0f - a;
}
