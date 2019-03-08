//=============================================================================
//
// �`���[�W�G�t�F�N�g���� [chargeEffect.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _CHARGEEFFECT_H_
#define _CHARGEEFFECT_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CHARGEEFFECT_SET_MAX		(2)		 // �g�̍ő吔
#define CHARGEEFFECT_ONESET_MAX		(4)		 // ��g�̍ő吔
#define MAX_CHARGEEFFECT			(CHARGEEFFECT_SET_MAX * CHARGEEFFECT_ONESET_MAX) // ����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
typedef struct
{
	D3DXMATRIX		mtxWorld;							// ���[���h�}�g���b�N�X
	D3DXVECTOR3		pos[CHARGEEFFECT_ONESET_MAX];		// �ʒu
	D3DXVECTOR3		scl[CHARGEEFFECT_ONESET_MAX];		// �X�P�[��
	float			dif[CHARGEEFFECT_ONESET_MAX];		// ���ߓx
	bool			use[CHARGEEFFECT_ONESET_MAX];		// �g�p���

} CHARGEEFFECT;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitChargeEffect(int type);
void UninitChargeEffect(void);
void UpdateChargeEffect(void);
void DrawChargeEffect(void);
void SetChargeEffect(D3DXVECTOR3 pos, int index);

#endif
