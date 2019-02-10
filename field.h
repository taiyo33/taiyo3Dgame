//=============================================================================
//
// �n�ʏ��� [field.h]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"

#define FIELD_SIZE_X	(400.0f)
#define FIELD_SIZE_Y	(0.0f)
#define FIELD_SIZE_Z	(300.0f)					

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitField(int type);
void UninitField(void);
void UpdateField(void);
void DrawField(void);

#endif
