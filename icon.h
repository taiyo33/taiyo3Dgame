//=============================================================================
//
// �v���C���[���� [player.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _ICON_H_
#define _ICON_H_

#include "main.h"

//=============================================================================
//�}�N����`
//=============================================================================

#define ICON_MAX				(2)
#define ICON_LIFE_MAX			(100.0f)
#define ICON_FALL_SPEED		(5.0f)

#define ICON01_INITPOS_X		(-300.0f)
#define ICON01_INITPOS_Y		(10.0f)
#define ICON01_INITPOS_Z		(200.0f)

#define ICON02_INITPOS_X		(300.0f)
#define ICON02_INITPOS_Y		(10.0f)
#define ICON02_INITPOS_Z		(-200.0f)

typedef struct {
	
	D3DXMATRIX					mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3					pos;				// ���݂̈ʒu
	bool						use;				// �g�p���
}ICON;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitIcon(int type);
void UninitIcon(void);
void UpdateIcon(void);
void DrawIcon(void);


#endif
