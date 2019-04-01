//=============================================================================
//
// �q�����f������ [ball.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _BALL_H_
#define _BALL_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BALL_SET_MAX	(2)
#define BALL_ONESET_MAX	(100)

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct {

	D3DXVECTOR3				pos[BALL_ONESET_MAX];		// �ʒu
	D3DXVECTOR3				prevPos[BALL_ONESET_MAX];	// �O��ʒu
	D3DXVECTOR3				rot[BALL_ONESET_MAX];		// ����(��])
	D3DXVECTOR3				scl[BALL_ONESET_MAX];		// �傫��(�X�P�[��)
	D3DXVECTOR3				move[BALL_ONESET_MAX];		// �ړ���
	bool					use[BALL_ONESET_MAX];		// �g�p���
	D3DXVECTOR3				vec[BALL_ONESET_MAX];
	D3DXVECTOR3				v1[BALL_ONESET_MAX];
	D3DXVECTOR3				v2[BALL_ONESET_MAX];
	D3DXVECTOR3				v3[BALL_ONESET_MAX];
	int						cnt;						// �q�����f���̐�
	LPDIRECTSOUNDBUFFER8	damageSE;					// ����SE
}BALL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBall(void);
void UninitBall(void);
void UpdateBall(void);
void DrawBall(void);
BALL *GetBall(int index);
void ComprareBall(void);
void SetBall(int index);
void SetInitPosBall(void);
void DamageBall(int index00, int index01, int bno);

#endif
