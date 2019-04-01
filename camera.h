//=============================================================================
//
// �J�������� [camera.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct {

	D3DXVECTOR3		pos;				// �J�����̎��_
	D3DXVECTOR3		at;					// �J�����̒����_
	D3DXVECTOR3		up;					// �J�����̏����
	D3DXVECTOR3		nor;				// �J�����̌���
	D3DXVECTOR3		rot;				// �J�����̉�]
	D3DXVECTOR3		fw;					// �J�����̑O��
	D3DXMATRIX		mtxView;			// �r���[�}�g���b�N�X
	D3DXMATRIX		mtxProjec;			// �v���W�F�N�V�����}�g���b�N�X

}CAMERA;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
CAMERA *GetCamera(void);
D3DXMATRIX *GetMtxView(void);

#endif
