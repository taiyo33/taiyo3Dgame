//=============================================================================
//
// ���f������ [head.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "main.h"
#include "head.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	PLAYER_HEAD		"data/MODEL/model_head.x"	// �ǂݍ��ރ��f����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9	 D3DTexture;		// �e�N�X�`���ǂݍ��ݏꏊ
static LPD3DXMESH			 D3DXMesh;			// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
static LPD3DXBUFFER			 D3DXBuffMat;		// ���b�V���̃}�e���A�������i�[
static DWORD				 NumMat;			// �������̑���

HEAD						 Head[PLAYER_MAX];
//=============================================================================
// ����������
//=============================================================================
HRESULT InitHead(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DTexture = NULL;
	D3DXMesh = NULL;
	D3DXBuffMat = NULL;
	
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		Head[i].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		Head[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Head[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
	
	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(PLAYER_HEAD,
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

#if 0
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME,		// �t�@�C���̖��O
		&D3DTexture);	// �ǂݍ��ރ������[
#endif

	//// �e�̐���
	//g_nIdxShadow = CreateShadow(Head.pos, 25.0f, 25.0f);

	//g_fSizeShadow = 25.0f;
	//g_colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitHead(void)
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
void UpdateHead(void)
{
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHead(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->use)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&Head[i].mtxWorld);

			// �g��̔��f
			D3DXMatrixScaling(&mtxScl, Head[i].scl.x, Head[i].scl.y, Head[i].scl.z);
			D3DXMatrixMultiply(&Head[i].mtxWorld, &Head[i].mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, Head[i].rot.y, Head[i].rot.x, Head[i].rot.z);
			D3DXMatrixMultiply(&Head[i].mtxWorld, &Head[i].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, Head[i].pos.x, Head[i].pos.y, Head[i].pos.z);
			D3DXMatrixMultiply(&Head[i].mtxWorld, &Head[i].mtxWorld, &mtxTranslate);


			D3DXMatrixMultiply(&Head[i].mtxWorld, &Head[i].mtxWorld, &player->mtxWorld);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &Head[i].mtxWorld);


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