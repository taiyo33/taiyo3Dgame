//=============================================================================
//
// �����G�t�F�N�g���� [explosion.h]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct
{
	D3DXMATRIX	mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 scl;		// �X�P�[��
	VERTEX_3D	*Vtx;
	D3DXVECTOR3 rot;		// �ړ���
	bool		use;				// �g�p���Ă��邩�ǂ���
	int			time;
	int			patternAnim;
	int			cntAnim;

} EXPLOSION;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitExplosion(int type);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest);

#endif
