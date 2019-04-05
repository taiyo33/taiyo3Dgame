//=============================================================================
//
// �o���b�g���� [bullet.h]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BULLET_MAX				(128)				// �g�p�\�̃o���b�g��
#define BULLET_ONESET_MAX		(1)					// �o���b�g1�g�̍ő吔
#define BULLET_SET_MAX			(2)					// �o���b�g�̑g��
#define BULLET_CHARGE_FRAME_CNT	(10)				// �`���[�W���x�̒����t���[����
#define BULLET_CHARGE_MAX		(10.0f)				// ���x�`���[�W�̍ő�l
#define INIT_REFLECT_CNT		(2)					// ���ˉ�
#define INIT_BULLET_SPEED		(5.0f)				// �o���b�g���x�̏����l
#define BULLET_DAMAGE			(10.0f)				// �o���b�g�̃_���[�W�l

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct
{
	D3DXMATRIX		mtxWorld;						// ���[���h�}�g���b�N�X
	LPD3DXMESH		D3DXMesh;						// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
	LPD3DXBUFFER	D3DXBuffMat;					// ���b�V���̃}�e���A�������i�[
	DWORD			NumMat;							// �������̑���
	D3DXVECTOR3		pos[BULLET_ONESET_MAX];			// �ʒu
	D3DXVECTOR3		prevPos[BULLET_ONESET_MAX];		// �O��̈ʒu
	D3DXVECTOR3		move[BULLET_ONESET_MAX];		// �ړ���
	D3DXVECTOR3		refVec[BULLET_ONESET_MAX];		// ���˃x�N�g��
	D3DXVECTOR3		rot[BULLET_ONESET_MAX];			// ��]��
	D3DXVECTOR3		scl[BULLET_ONESET_MAX];			// �X�P�[��
	D3DXVECTOR3		size[BULLET_ONESET_MAX];		// �傫��
	D3DXVECTOR3		sclIncrease;					// �X�P�[���̑����l
	LPDIRECTSOUNDBUFFER8	BulletSE;				// �o���b�g���ˎ���SE
	LPDIRECTSOUNDBUFFER8	ReflectSE;				// ���ˎ���SE
	int				time[BULLET_ONESET_MAX];		// ����
	int				cntReflect[BULLET_ONESET_MAX];	// ���ˉ�
	bool			use[BULLET_ONESET_MAX];			// �g�p���
	bool			reflect[BULLET_ONESET_MAX];		// ���˂̑O���ォ
	float			speed[BULLET_ONESET_MAX];		// ����
	float			speedIncrease;					// �����̑���

} BULLET;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(int type);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
BULLET *GetBullet(int bno);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float speed, float Dest, int index);

#endif
