//=============================================================================
//
// �J�������� [camera.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "camera.h"
#include "player.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VIEW_ANGLE			(D3DXToRadian(90.0f))	// ����p
#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��
#define	VIEW_NEAR_Z			(10.0f)				// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z			(1000.0f)			// �r���[���ʂ�FarZ�l
#define	VALUE_MOVE_CAMERA	(5.0f)				// �J�����̈ړ���
#define INIT_POS_X			(0.0f)
#define INIT_POS_Y			(450.0f)
#define INIT_POS_Z			(-200.0f)
#define ROT_SPEED_X			(D3DX_PI * 0.01f)	// X����]���x�̏����l
#define ROT_SPEED_Y			(D3DX_PI * 0.005f)	// Y����]���x�̏����l

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

CAMERA				camera;		// �J�����\����
//=============================================================================
// �J�����̏�����
//=============================================================================
HRESULT InitCamera(void)
{
	camera.pos = D3DXVECTOR3(INIT_POS_X, INIT_POS_Y, INIT_POS_Z);		// �ʒu��������
	camera.at = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����_��������
	camera.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// �������������
	camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]��������
	camera.rotSpeedX = ROT_SPEED_X;
	camera.rotSpeedY = ROT_SPEED_Y;

	float vx, vz;
	vx = camera.pos.x - camera.at.x;
	vz = camera.pos.z - camera.at.z;
	camera.fLengthInterval = sqrtf(vx * vx + vz * vz);

	return S_OK;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate,mtxQua;

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&camera.mtxView);

	D3DXMatrixLookAtLH(&camera.mtxView,
								&camera.pos,	// �ʒu
								&camera.at,		// �����_
								&camera.up);	// �����

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &camera.mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&camera.mtxProjec);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&camera.mtxProjec,
								VIEW_ANGLE,		// ����p
								VIEW_ASPECT,	// �A�X�y�N�g��
								VIEW_NEAR_Z,	// �r���[���ʂ�NearZ�l
								VIEW_FAR_Z);	// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &camera.mtxProjec);
}

//=============================================================================
// �J�����̌����̎擾
// ���@���F�Ȃ�
// �߂�l�FCAMERA�^�\����
//=============================================================================
CAMERA *GetCamera(void)
{
	return &camera;
}

//=============================================================================
// �r���[�}�g���b�N�X�̎擾
// ���@���F�Ȃ�
// �߂�l�FD3DXMATRIX�^�\����
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	return camera.mtxView;
}
