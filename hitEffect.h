//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _HITEFFECT_H_
#define _HITEFFECT_H_

#include "main.h"

#define HITEFFECT_SET_MAX			(20)			// �g�̍ő吔
#define HITEFFECT_ONESET_MAX		(24)			// 1�g�̍ő吔
#define HITEFFECT_MAX				(HITEFFECT_SET_MAX * HITEFFECT_ONESET_MAX)	// ����

typedef struct
{
	D3DXMATRIX		mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3		pos[HITEFFECT_ONESET_MAX];		// �ʒu
	float			dif[HITEFFECT_ONESET_MAX];		// ���ߓx
	bool			use;							// �g�p���Ă��邩�ǂ���

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
