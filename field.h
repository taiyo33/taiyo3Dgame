//=============================================================================
//
// �n�ʏ��� [field.h]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitField(int type);
void UninitField(void);
void UpdateField(void);
void DrawField(void);
void SetCollarField(int fno, float colR, float colB, float colG);

#endif
