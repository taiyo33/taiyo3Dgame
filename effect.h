//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"


#define DELETEFFECT_ONESET_MAX		(10)

typedef struct
{
	D3DXMATRIX		mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3		pos[DELETEFFECT_ONESET_MAX];		// �ʒu
	float			dif_mi[DELETEFFECT_ONESET_MAX];
	bool			use[DELETEFFECT_ONESET_MAX];				// �g�p���Ă��邩�ǂ���
	int				time[DELETEFFECT_ONESET_MAX];

} EFFECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEffect(int type);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
bool SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest, int index);

#endif
