//=============================================================================
//
// �o���b�g�G�t�F�N�g���� [smoke.h]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#ifndef _BULLETEFFECT_H_
#define _BULLETEFFECT_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define BULLETEFFECT_SET_MAX			(6)		// �g�̍ő吔
#define BULLETEFFECT_ONESET_MAX			(30)	// ��g�̍ő吔

//*****************************************************************************
// �\���̐錾
//*****************************************************************************

typedef struct
{
	D3DXMATRIX mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos[BULLETEFFECT_ONESET_MAX];	// �ʒu
	D3DXVECTOR3 scl[BULLETEFFECT_ONESET_MAX];	// �X�P�[��
	bool		use[BULLETEFFECT_ONESET_MAX];	// �g�p���
	int			time[BULLETEFFECT_ONESET_MAX];	// �G�t�F�N�g����
	float		dif[BULLETEFFECT_ONESET_MAX];	// ���ߒl

} BULLETEFFECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBulletEffect(int type);
void UninitBulletEffect(void);
void UpdateBulletEffect(void);
void DrawBulletEffect(void);
void SetBulletEffect(D3DXVECTOR3 pos, int index);

#endif
