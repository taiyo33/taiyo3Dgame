//=============================================================================
//
// ���胂�f������ [leftArm.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "main.h"
#include "leftArm.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	PLAYER_LEFTARM		"data/MODEL/model_arm.x"	// �ǂݍ��ރ��f����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9	 D3DTexture;			// �e�N�X�`���ǂݍ��ݏꏊ
static LPD3DXMESH			 D3DXMesh;				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
static LPD3DXBUFFER			 D3DXBuffMat;			// ���b�V���̃}�e���A�������i�[
static DWORD				 NumMat;				// �������̑���

LEFTARM						 LeftArm[PLAYER_MAX];	// ���r���f���\����
//=============================================================================
// ����������
//=============================================================================
HRESULT InitLeftArm(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	D3DTexture = NULL;
	D3DXMesh = NULL;
	D3DXBuffMat = NULL;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		LeftArm[i].pos = D3DXVECTOR3(-12.0f, 10.0f, 0.0f);
		LeftArm[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(PLAYER_LEFTARM,
		D3DXMESH_SYSTEMMEM,
		Device,
		NULL,
		&D3DXBuffMat,
		NULL,
		&NumMat,
		&D3DXMesh)))
	{
		return E_FAIL;
	}


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitLeftArm(void)
{
	if (D3DTexture != NULL)
	{// �e�N�X�`���̊J��
		D3DTexture->Release();
		D3DTexture = NULL;
	}

	if (D3DXMesh != NULL)
	{// ���b�V���̊J��
		D3DXMesh->Release();
		D3DXMesh = NULL;
	}

	if (D3DXBuffMat != NULL)
	{// �}�e���A���̊J��
		D3DXBuffMat->Release();
		D3DXBuffMat = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLeftArm(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLeftArm(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *D3DXMat;
	D3DMATERIAL9 matDef;
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->use)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&LeftArm[i].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, LeftArm[i].rot.y, LeftArm[i].rot.x, LeftArm[i].rot.z);
			D3DXMatrixMultiply(&LeftArm[i].mtxWorld, &LeftArm[i].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, LeftArm[i].pos.x, LeftArm[i].pos.y, LeftArm[i].pos.z);
			D3DXMatrixMultiply(&LeftArm[i].mtxWorld, &LeftArm[i].mtxWorld, &mtxTranslate);


			D3DXMatrixMultiply(&LeftArm[i].mtxWorld, &LeftArm[i].mtxWorld, &player->mtxWorld);

			// ���[���h�}�g���b�N�X�̐ݒ�
			Device->SetTransform(D3DTS_WORLD, &LeftArm[i].mtxWorld);


			// ���݂̃}�e���A�����擾
			Device->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			D3DXMat = (D3DXMATERIAL*)D3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)NumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				Device->SetMaterial(&D3DXMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				Device->SetTexture(0, D3DTexture);

				// �`��
				D3DXMesh->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			Device->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// �ʒu�擾
//=============================================================================
LEFTARM *GetLeftArm(int index)
{
	return &LeftArm[index];
}
