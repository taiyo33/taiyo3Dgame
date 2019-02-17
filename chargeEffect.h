//=============================================================================
//
// �`���[�W�G�t�F�N�g���� [chargeEffect.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _CHARGEEFFECT_H_
#define _CHARGEEFFECT_H_

#include "main.h"

#define CHARGEEFFECT_MAX			(50)
#define CHARGEEFFECT_SET_MAX		(2)
#define CHARGEEFFECT_ONESET_MAX		(4)

typedef struct
{
	D3DXMATRIX		mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3		pos[CHARGEEFFECT_ONESET_MAX];		// �ʒu
	D3DXVECTOR3		scl[CHARGEEFFECT_ONESET_MAX];
	float			dif_mi[CHARGEEFFECT_ONESET_MAX];	// ���ߓx
	bool			use[CHARGEEFFECT_ONESET_MAX];							// �g�p���Ă��邩�ǂ���

} CHARGEEFFECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitChargeEffect(int type);
void UninitChargeEffect(void);
void UpdateChargeEffect(void);
void DrawChargeEffect(void);
void SetChargeEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest, int index);

#endif
