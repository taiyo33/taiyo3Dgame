//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _HITEFFECT_H_
#define _HITEFFECT_H_

#include "main.h"

#define HITEFFECT_MAX				(255)
#define HITEFFECT_SET_MAX			(20)
#define HITEFFECT_ONESET_MAX		(24)

typedef struct
{
	D3DXMATRIX		mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3		pos[HITEFFECT_ONESET_MAX];		// �ʒu
	D3DXVECTOR3		move[HITEFFECT_ONESET_MAX];		// �ړ���
	float			dif_mi[HITEFFECT_ONESET_MAX];	// ���ߓx
	bool			use;							// �g�p���Ă��邩�ǂ���
	float			angle[HITEFFECT_ONESET_MAX];	// �ړ��p�x
	float			speed[HITEFFECT_ONESET_MAX];	// �ړ����x

} HITEFFECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitHitEffect(int type);
void UninitHitEffect(void);
void UpdateHitEffect(void);
void DrawHitEffect(void);
void SetHitEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest);

#endif
