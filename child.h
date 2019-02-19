//=============================================================================
//
// �q�����f������ [child.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _CHILD_H_
#define _CHILD_H_

#include "main.h"

#define CHILD_SET_MAX		(2)
#define CHILD_ONESET_MAX	(100)

typedef struct {

	D3DXVECTOR3				pos[CHILD_ONESET_MAX];		// �ʒu
	D3DXVECTOR3				prevPos[CHILD_ONESET_MAX];	// �O��ʒu
	D3DXVECTOR3				rot[CHILD_ONESET_MAX];		// ����(��])
	D3DXVECTOR3				scl[CHILD_ONESET_MAX];		// �傫��(�X�P�[��)
	D3DXVECTOR3				move[CHILD_ONESET_MAX];		// �ړ���
	bool					use[CHILD_ONESET_MAX];		// �g�p���
	D3DXVECTOR3				vec[CHILD_ONESET_MAX];
	D3DXVECTOR3				v1[CHILD_ONESET_MAX];
	D3DXVECTOR3				v2[CHILD_ONESET_MAX];
	D3DXVECTOR3				v3[CHILD_ONESET_MAX];
	int						cnt;						// �q�����f���̐�
	LPDIRECTSOUNDBUFFER8	damageSE;					// ����SE


}CHILD;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitChild(void);
void UninitChild(void);
void UpdateChild(void);
void DrawChild(void);
CHILD *GetChild(int index);
void ComprareChild(void);
void SetChild(int index);
void SetInitPosChild(void);

#endif
