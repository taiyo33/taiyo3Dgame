//=============================================================================
//
// ���f������ [RightArm.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "main.h"
#include "rightArm.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	PLAYER_AIRPLANE		"data/MODEL/model_arm.x"	// �ǂݍ��ރ��f����

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

RIGHTARM					RightArm[PLAYER_MAX];	// �E�r���f���\����
//=============================================================================
// ����������
//=============================================================================
HRESULT InitRightArm(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DTexture = NULL;
	D3DXMesh = NULL;
	D3DXBuffMat = NULL;

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		RightArm[i].pos = D3DXVECTOR3(12.0f, 10.0f, 0.0f);
		RightArm[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(PLAYER_AIRPLANE,
		D3DXMESH_SYSTEMMEM,
		pDevice,
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
void UninitRightArm(void)
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
void UpdateRightArm(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawRightArm(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->use)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&RightArm[i].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, RightArm[i].rot.y, RightArm[i].rot.x, RightArm[i].rot.z);
			D3DXMatrixMultiply(&RightArm[i].mtxWorld, &RightArm[i].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, RightArm[i].pos.x, RightArm[i].pos.y, RightArm[i].pos.z);
			D3DXMatrixMultiply(&RightArm[i].mtxWorld, &RightArm[i].mtxWorld, &mtxTranslate);


			D3DXMatrixMultiply(&RightArm[i].mtxWorld, &RightArm[i].mtxWorld, &player->mtxWorld);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &RightArm[i].mtxWorld);


			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat = (D3DXMATERIAL*)D3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)NumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, D3DTexture);

				// �`��
				D3DXMesh->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

