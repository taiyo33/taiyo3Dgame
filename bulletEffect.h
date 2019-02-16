//=============================================================================
//
// �o���b�g�G�t�F�N�g���� [smoke.h]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#ifndef _BULLETEFFECT_H_
#define _BULLETEFFECT_H_

#include "main.h"

#define BULLETEFFECT_ONESET_MAX			(30)
#define BULLETEFFECT_SET_MAX				(6)

typedef struct
{
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos[BULLETEFFECT_ONESET_MAX];		// �ʒu
	D3DXVECTOR3 scl[BULLETEFFECT_ONESET_MAX];		// �X�P�[��
	bool		use[BULLETEFFECT_ONESET_MAX];		// �g�p���Ă��邩�ǂ���
	int			time[BULLETEFFECT_ONESET_MAX];		// �G�t�F�N�g����
	float		Alpha[BULLETEFFECT_ONESET_MAX];
	float		angle;

} BULLETEFFECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBulletEffect(int type);
void UninitBulletEffect(void);
void UpdateBulletEffect(void);
void DrawBulletEffect(void);
bool SetBulletEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, float Dest, float sizeX, float sizeY, int index);
#endif
